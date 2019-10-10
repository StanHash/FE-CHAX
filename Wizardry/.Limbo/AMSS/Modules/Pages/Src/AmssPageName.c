
#include "AmssPage.h"

struct AmssPageNameItem
{
	const void* img;
	const void* palette;
	const void* obj;
};

struct AmssPageNameInfo
{
	const struct AmssPageNameItem back;
	const struct AmssPageNameItem items[];
};

struct AmssPageNameProc
{
	/* 00 */ PROC_HEADER;

	/* 2C */ unsigned cnt;
	/* 30 */ unsigned tileid;
	/* 34 */ unsigned palid;
	/* 38 */ unsigned affineid;

	/* 3C */ struct AmssGeometry rect;
	/* 40 */ const struct AmssPageNameInfo* info;
};

static void AmssPageName_OnIdle(struct AmssPageNameProc* proc);
static void AmssPageName_ChangeOutLoop(struct AmssPageNameProc* proc);
static void AmssPageName_ChangeInLoop(struct AmssPageNameProc* proc);

static
const struct ProcInstruction sPageNameProcScr[] =
{
	PROC_SET_NAME("StanScreen PageName Module"),
	PROC_YIELD,

PROC_LABEL(0), // Idle
	PROC_LOOP_ROUTINE(AmssPageName_OnIdle),

PROC_LABEL(1), // Change page
	PROC_LOOP_ROUTINE(AmssPageName_ChangeOutLoop),
	PROC_LOOP_ROUTINE(AmssPageName_ChangeInLoop),

	PROC_GOTO(0),

	PROC_END,
};

static
void AmssPageName_Display(struct AmssPageNameProc* proc)
{
	ObjInsertSafe(
		4,
		proc->rect.x*16,
		proc->rect.y*16,
		proc->info->items[gStatScreenData.page].obj,
		TILEREF(proc->tileid, proc->palid));
}

static void AmssPageName_OnIdle(struct AmssPageNameProc* proc)
{

}

static void AmssPageName_ChangeOutLoop(struct AmssPageNameProc* proc)
{

}

static void AmssPageName_ChangeInLoop(struct AmssPageNameProc* proc)
{

}

