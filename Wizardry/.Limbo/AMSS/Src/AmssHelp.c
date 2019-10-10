
#include "AmssInternal.h"

// TODO: update reference and add to CLib

struct HelpBoxInfo
{
	/* 00 */ const struct HelpBoxInfo* adj1;
	/* 04 */ const struct HelpBoxInfo* adj2;
	/* 08 */ const struct HelpBoxInfo* adj3;
	/* 0C */ const struct HelpBoxInfo* adj4;
	/* 10 */ u8 xDisplay;
	/* 11 */ u8 yDisplay;
	/* 12 */ u16 msgId;
	/* 14 */ void(*onInitMoveable)(struct Proc*);
	/* 18 */ void(*onInit)(struct Proc*);
};

static void(*ClearHelpBoxText)(void) = (const void*)(0x0808A118+1);
static void(*StartHelpBoxFromInfo)(const struct HelpBoxInfo* info, int boolSilent) = (const void*)(0x08088E9C+1);

// END TODO

struct AmssHelpBoxProc
{
	/* 00 */ PROC_HEADER;

	/* 2C */ struct AmssGeometry rect;
	/* 30 */ struct HelpBoxInfo info;

	/* 4C */ /* end */
};

void AmssModuleForEachHelp(const struct AmssModuleEntry* entry, void(*func)(const struct AmssHelpInfo*, void*), void* funcUser)
{
	if (entry->module->forEachHelp)
	{
		entry->module->forEachHelp(func, funcUser, entry->user, entry->rect);
		return;
	}

	if (entry->module->helpInfo)
	{
		for (const struct AmssHelpInfo* info = entry->module->helpInfo; info->rect.w && info->rect.h; ++info)
		{
			struct AmssHelpInfo tmp = *info;

			tmp.rect.x += entry->rect.x;
			tmp.rect.y += entry->rect.y;

			func(&tmp, funcUser);
		}
	}
}

void AmssForEachHelp(void(*func)(const struct AmssHelpInfo*, void*), void* funcUser)
{
	for (const struct AmssModuleEntry* it = AmssCoreModules; it->module; ++it)
		AmssModuleForEachHelp(it, func, funcUser);
}

struct FindInfoUser
{
	short xCenter, yCenter;
	struct AmssHelpInfo* out;
	unsigned dist;
};

#define DECL_HELP_CHECK(aFnName, aConditionExpr) \
static void aFnName (const struct AmssHelpInfo* info, void* user) \
{ \
	struct FindInfoUser* const data = user; \
	const int xInfo   = info->rect.x + info->rect.w/2; \
	const int yInfo   = info->rect.y + info->rect.h/2; \
	if ((xInfo == data->xCenter && yInfo == data->yCenter) || (aConditionExpr)) \
		return; \
	const unsigned dist = RECT_DISTANCE(data->xCenter, data->yCenter, xInfo, yInfo); \
	if (dist < data->dist) \
	{ \
		*data->out = *info; \
		data->dist = dist; \
	} \
}

#define DECL_HELP_FIND(aFnName, aHelpCheckFn) \
void aFnName (int xCenter, int yCenter, struct AmssHelpInfo* out) \
{ \
	struct FindInfoUser data = { \
		.xCenter = xCenter, .yCenter = yCenter, \
		.out = out, \
		.dist = UINT32_MAX \
	}; \
	AmssForEachHelp((aHelpCheckFn), &data); \
}

// Check if info not left to center OR vertical dist > horizontal dist
DECL_HELP_CHECK(HelpCheckLeft, (xInfo > data->xCenter) || (ABS(data->yCenter - yInfo) > (data->xCenter - xInfo)))
DECL_HELP_FIND(AmssFindLeftHelp, HelpCheckLeft)

// Check if info not right to center OR vertical dist > horizontal dist
DECL_HELP_CHECK(HelpCheckRight, (xInfo < data->xCenter) || (ABS(data->yCenter - yInfo) > (xInfo - data->xCenter)))
DECL_HELP_FIND(AmssFindRightHelp, HelpCheckRight)

// Check if info not above center OR horizontal dist > vertical dist
DECL_HELP_CHECK(HelpCheckUp, (yInfo > data->yCenter) || (ABS(data->xCenter - xInfo) > (data->yCenter - yInfo)))
DECL_HELP_FIND(AmssFindUpHelp, HelpCheckUp)

// Check if info not below center OR horizontal dist > vertical dist
DECL_HELP_CHECK(HelpCheckDown, (yInfo < data->yCenter) || (ABS(data->xCenter - xInfo) > (yInfo - data->yCenter)))
DECL_HELP_FIND(AmssFindDownHelp, HelpCheckDown)

// Check not (always compare)
DECL_HELP_CHECK(HelpCheckAlways, FALSE)
DECL_HELP_FIND(AmssFindClosestHelp, HelpCheckAlways)

#undef DECL_HELP_CHECK
#undef DECL_HELP_FIND

static void AmssHelpBox_DebugRedraw(struct AmssHelpBoxProc* proc)
{
	AmssDebugClear();
	AmssDebugDisplayHelps();

	AmssDebugColorRect(proc->rect, 1);

	const int xCenter = proc->rect.x + proc->rect.w/2;
	const int yCenter = proc->rect.y + proc->rect.h/2;

	{
		struct AmssHelpInfo info = {};
		AmssFindLeftHelp(xCenter, yCenter, &info);

		if (info.rect.w && info.rect.h)
			AmssDebugColorRect(info.rect, 2);
	}

	{
		struct AmssHelpInfo info = {};
		AmssFindRightHelp(xCenter, yCenter, &info);

		if (info.rect.w && info.rect.h)
			AmssDebugColorRect(info.rect, 2);
	}

	{
		struct AmssHelpInfo info = {};
		AmssFindUpHelp(xCenter, yCenter, &info);

		if (info.rect.w && info.rect.h)
			AmssDebugColorRect(info.rect, 2);
	}

	{
		struct AmssHelpInfo info = {};
		AmssFindDownHelp(xCenter, yCenter, &info);

		if (info.rect.w && info.rect.h)
			AmssDebugColorRect(info.rect, 2);
	}
}

static void AmssHelpBox_SetHelp(struct AmssHelpBoxProc* proc, const struct AmssHelpInfo* info)
{
	if (!info || !(info->rect.w && info->rect.h))
	{
		END_PROC(proc);
		return;
	}

	proc->rect = info->rect;

	proc->info.msgId    = info->msgId;
	proc->info.xDisplay = info->rect.x * 8 + 4;
	proc->info.yDisplay = info->rect.y * 8 + info->rect.h * 4 - 4;
	proc->info.onInit   = info->onInit;

	proc->info.onInitMoveable = NULL;

	proc->info.adj1 = NULL;
	proc->info.adj2 = NULL;
	proc->info.adj3 = NULL;
	proc->info.adj4 = NULL;

	StartHelpBoxFromInfo(&proc->info, FALSE);

	// TODO: if (debug)
	AmssHelpBox_DebugRedraw(proc);
}

static void AmssHelpBox_OnLoop(struct AmssHelpBoxProc* proc);
static void AmssHelpBox_OnEnd(struct AmssHelpBoxProc* proc);

static const struct ProcInstruction sProc_AmssHelpBox[] = {
	PROC_SET_NAME("StanScreen HelpBox"),
	PROC_SLEEP(1),

	PROC_SET_DESTRUCTOR(AmssHelpBox_OnEnd),
	PROC_LOOP_ROUTINE(AmssHelpBox_OnLoop),

	PROC_END,
};

static void AmssHelpBox_OnLoop(struct AmssHelpBoxProc* proc)
{
	if (gKeyState.pressedKeys & (KEY_BUTTON_B | KEY_BUTTON_R))
	{
		END_PROC(proc);
		return;
	}

	UpdateHandCursor(
		proc->rect.x * 8 + 4,
		proc->rect.y * 8 + proc->rect.h * 4 - 4);

	struct AmssHelpInfo info = {};

	const int xCenter = proc->rect.x + proc->rect.w/2;
	const int yCenter = proc->rect.y + proc->rect.h/2;

	const unsigned keys = gKeyState.repeatedKeys;

	if (keys & KEY_DPAD_LEFT)
		AmssFindLeftHelp(xCenter, yCenter, &info);

	if (keys & KEY_DPAD_RIGHT)
		AmssFindRightHelp(xCenter, yCenter, &info);

	if (keys & KEY_DPAD_UP)
		AmssFindUpHelp(xCenter, yCenter, &info);

	if (keys & KEY_DPAD_DOWN)
		AmssFindDownHelp(xCenter, yCenter, &info);

	if (info.rect.w && info.rect.h)
	{
		AmssHelpBox_SetHelp(proc, &info);
		PlaySfx(0x67);
	}
}

static void AmssHelpBox_OnEnd(struct AmssHelpBoxProc* proc)
{
	struct Proc* helpBox = ProcFind(gProc_HelpBox);

	if (helpBox)
	{
		ClearHelpBoxText();
		ProcGoto(helpBox, 0x63);
	}

	gStatScreenData.inTransition = FALSE;
}

struct Proc* AmssStartHelpBox(int xAnchor, int yAnchor, struct AmssMainProc* parent)
{
	struct AmssHelpBoxProc* proc = START_PROC_BLOCKING(sProc_AmssHelpBox, parent);

	LoadDialogueBoxGfx(NULL, -1);

	struct AmssHelpInfo help;
	AmssFindClosestHelp(xAnchor, yAnchor, &help);
	AmssHelpBox_SetHelp(proc, &help);

	gStatScreenData.inTransition = TRUE;

	return (struct Proc*) proc;
}
