
#include "AmssPage.h"

static void AmssPageInit(u16* fg, u16* bg, u32 user, struct AmssGeometry rect, struct AmssMainProc* proc);
static void AmssPageEnd(u16* fg, u16* bg, u32 user, struct AmssGeometry rect, struct AmssMainProc* proc);
static void AmssPageIterHelp(void(*func)(const struct AmssHelpInfo*, void*), void* funcUser, u32 user, struct AmssGeometry  rect);

const struct AmssModule AmssPageModule =
{
	.onInit = AmssPageInit,
	.onEnd  = AmssPageEnd,
	.forEachHelp = AmssPageIterHelp,
};

static inline
const struct AmssModuleEntry** GetPages(u32 user)
{
	return (const struct AmssModuleEntry**) (user);
}

static inline
unsigned GetPageCount(u32 user)
{
	const struct AmssModuleEntry** pages = GetPages(user);

	unsigned count = 0;

	while (*pages++)
		count++;

	return count;
}

struct PageProc
{
	/* 00 */ PROC_HEADER;

	/* 29 */ s8 changeDirection;
	/* 2A */ u8 nextPage;
	/* 2B */ u8 changeFrame;

	/* 2C */ const struct AmssModuleEntry** pages;
	/* 30 */ struct AmssGeometry rect;
	/* 34 */ u16* fg;
	/* 38 */ u16* bg;
};

static void AmssPageInitInterior(struct PageProc* proc);
static void AmssPageEndInterior(struct PageProc* proc);

static void AmssPageStartTransition(struct PageProc* proc, int nextPage, int direction);

static void AmssPage_OnIdle(struct PageProc* proc);

static void AmssPage_ChangeOutLoop(struct PageProc* proc);
static void AmssPage_ChangePoint(struct PageProc* proc);
static void AmssPage_ChangeInLoop(struct PageProc* proc);
static void AmssPage_ChangeEnd(struct PageProc* proc);

static
const struct ProcInstruction sPageProcScr[] =
{
	PROC_SET_NAME("StanScreen Page Module"),
	PROC_YIELD,

PROC_LABEL(0), // Idle
	PROC_LOOP_ROUTINE(AmssPage_OnIdle),

PROC_LABEL(1), // Change page
	PROC_LOOP_ROUTINE(AmssPage_ChangeOutLoop),
	PROC_CALL_ROUTINE(AmssPage_ChangePoint),
	PROC_LOOP_ROUTINE(AmssPage_ChangeInLoop),
	PROC_CALL_ROUTINE(AmssPage_ChangeEnd),

	PROC_GOTO(0),

	PROC_END,
};

static void AmssPage_OnIdle(struct PageProc* proc)
{
	if (gStatScreenData.inTransition)
		return; // Don't do anything while transitioning

	if (gKeyState.repeatedKeys & KEY_DPAD_LEFT)
	{
		int nextPage = gStatScreenData.page - 1;

		if (nextPage < 0)
			nextPage += gStatScreenData.pageCount;

		AmssPageStartTransition(proc, nextPage, -1);
	}

	if (gKeyState.repeatedKeys & KEY_DPAD_RIGHT)
	{
		int nextPage = gStatScreenData.page + 1;

		if (nextPage >= gStatScreenData.pageCount)
			nextPage -= gStatScreenData.pageCount;

		AmssPageStartTransition(proc, nextPage, +1);
	}
}

static void AmssPageChangeCommon(struct PageProc* proc, int off, int direction)
{
	const int len = proc->rect.w - off;

	const int xSource = proc->rect.x + (direction > 0 ? 0 : off);
	const int xTarget = proc->rect.x + (direction > 0 ? off : 0);

	BgMapFillRect(proc->bg + TILEMAP_INDEX(proc->rect.x, proc->rect.y), proc->rect.w, proc->rect.h, 0);
	BgMapFillRect(proc->fg + TILEMAP_INDEX(proc->rect.x, proc->rect.y), proc->rect.w, proc->rect.h, 0);

	if (len > 0)
	{
		BgMapCopyRect(
			gpStatScreenPageBg0Map + TILEMAP_INDEX(xSource, proc->rect.y),
			proc->fg + TILEMAP_INDEX(xTarget, proc->rect.y),
			len, proc->rect.h);

		BgMapCopyRect(
			gpStatScreenPageBg2Map + TILEMAP_INDEX(xSource, proc->rect.y),
			proc->bg + TILEMAP_INDEX(xTarget, proc->rect.y),
			len, proc->rect.h);
	}

	AmssNotifyRefresh();
}

static void AmssPage_ChangeOutLoop(struct PageProc* proc)
{
	static const u8 offLookup[] = { 4, 7, 10, 12, 14 };

	AmssPageChangeCommon(proc, offLookup[proc->changeFrame], proc->changeDirection);

	if ((++proc->changeFrame >= sizeof(offLookup)) || (offLookup[proc->changeFrame] >= proc->rect.w))
		BREAK_PROC(proc);
}

static void AmssPage_ChangePoint(struct PageProc* proc)
{
	AmssPageEndInterior(proc);

	gStatScreenData.page = proc->nextPage;
	AmssPageInitInterior(proc);

	proc->changeFrame = 0;
}

static void AmssPage_ChangeInLoop(struct PageProc* proc)
{
	static const u8 offLookup[] = { 13, 9, 7, 5, 3, 2, 1, 0 };

	while (offLookup[proc->changeFrame] >= proc->rect.w)
		++proc->changeFrame;

	AmssPageChangeCommon(proc, offLookup[proc->changeFrame], -proc->changeDirection);

	if (offLookup[proc->changeFrame++] == 0)
		BREAK_PROC(proc);
}

static void AmssPage_ChangeEnd(struct PageProc* proc)
{
	gStatScreenData.inTransition = FALSE;
}

static void AmssPageStartTransition(struct PageProc* proc, int nextPage, int direction)
{
	proc->nextPage = nextPage;
	proc->changeDirection = direction;
	proc->changeFrame = 0;

	gStatScreenData.inTransition = TRUE;
	ProcGoto((struct Proc*) proc, 1);

	PlaySfx(0x6F);
}

static void AmssPageInitInterior(struct PageProc* proc)
{
	BgMapFillRect(gpStatScreenPageBg0Map + TILEMAP_INDEX(proc->rect.x, proc->rect.y), proc->rect.w, proc->rect.h, 0);
	BgMapFillRect(gpStatScreenPageBg2Map + TILEMAP_INDEX(proc->rect.x, proc->rect.y), proc->rect.w, proc->rect.h, 0);

	const struct AmssModuleEntry* it = proc->pages[gStatScreenData.page];

	while (it->module)
	{
		struct AmssModuleEntry tmp = *it++;

		tmp.rect.x += proc->rect.x;
		tmp.rect.y += proc->rect.y;

		AmssInitModule(&tmp, gpStatScreenPageBg0Map, gpStatScreenPageBg2Map, (struct AmssMainProc*) proc->parent);
	}

	AmssNotifyRefresh();
}

static void AmssPageEndInterior(struct PageProc* proc)
{
	const struct AmssModuleEntry* it = proc->pages[gStatScreenData.page];

	while (it->module)
	{
		struct AmssModuleEntry tmp = *it++;

		tmp.rect.x += proc->rect.x;
		tmp.rect.y += proc->rect.y;

		AmssEndModule(&tmp, gpStatScreenPageBg0Map, gpStatScreenPageBg2Map, (struct AmssMainProc*) proc->parent);
	}
}

static void AmssPageInit(u16* fg, u16* bg, u32 user, struct AmssGeometry rect, struct AmssMainProc* proc)
{
	struct PageProc* page = START_PROC(sPageProcScr, proc);

	page->fg = fg;
	page->bg = bg;
	page->rect = rect;
	page->pages = GetPages(user);

	gStatScreenData.pageCount = GetPageCount(user);

	AmssPageInitInterior(page);

	BgMapCopyRect(
		gpStatScreenPageBg0Map + TILEMAP_INDEX(rect.x, rect.y),
		page->fg + TILEMAP_INDEX(rect.x, rect.y),
		rect.w, rect.h);

	BgMapCopyRect(
		gpStatScreenPageBg2Map + TILEMAP_INDEX(rect.x, rect.y),
		page->bg + TILEMAP_INDEX(rect.x, rect.y),
		rect.w, rect.h);

	EnableBgSyncByMask(BG0_SYNC_BIT | BG2_SYNC_BIT);
}

static void AmssPageEnd(u16* fg, u16* bg, u32 user, struct AmssGeometry rect, struct AmssMainProc* proc)
{
	struct PageProc* page = (struct PageProc*) ProcFind(sPageProcScr);

	AmssPageEndInterior(page);
	END_PROC(page);
}

static void AmssPageIterHelp(void(*func)(const struct AmssHelpInfo*, void*), void* funcUser, u32 user, struct AmssGeometry  rect)
{
	const struct AmssModuleEntry* it = GetPages(user)[gStatScreenData.page];

	while (it->module)
	{
		struct AmssModuleEntry tmp = *it++;

		tmp.rect.x += rect.x;
		tmp.rect.y += rect.y;

		AmssModuleForEachHelp(&tmp, func, funcUser);
	}
}
