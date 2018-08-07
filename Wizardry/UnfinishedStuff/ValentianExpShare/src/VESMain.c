#include "VES.h"

/*

	VES tile layout, THE PLAN:

	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	- - |-------------------------------------------------| - -
	- - | @@@ - ############### - - - - - - - - - - - - - | - -
	- - | @@@ - ############### - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - | - - - - - - - - - - - - - - - - - - - - - - - - | - -
	- - |-------------------------------------------------| - -
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

*/

void SetPrimaryHBlankCallback(void(*)(void));

static void VESSetupHBlank(struct VESProc* proc);
static void VESProcDrawBase(struct VESProc* proc);
static void VESCleanup(struct VESProc* proc);

static const ProcCode gVESProcScr[] = {
	PROC_SET_NAME(VES_PREFIX":Main"),
	PROC_SET_DESTRUCTOR(VESCleanup),

	PROC_CALL_ROUTINE(BlockGameGraphicsLogic),
	PROC_YIELD,

	PROC_CALL_ROUTINE(VESSetupHBlank),

	PROC_CALL_ROUTINE(VESProcDrawBase),
	PROC_BLOCK,

	PROC_CALL_ROUTINE(UnblockGameGraphicsLogic),
	PROC_END
};

static void VESOnHBlank(void) {
	if (!(REG_VCOUNT % 16))
		return;

	unsigned win0h = gLCDIOBuffer.win0h;

	win0h = (win0h &~ 0xFF) | (0 & 0xFF);

	REG_WIN0H = win0h;
}

void VESSetupHBlank(struct VESProc* proc) {
	gLCDIOBuffer.win0h = 0xFF00;
	gLCDIOBuffer.win0v = 0xFF20;

	gLCDIOBuffer.winControl.wout_bg0_on = FALSE;
	gLCDIOBuffer.winControl.wout_obj_on = FALSE;

	gLCDIOBuffer.dispControl.win0_on = TRUE;

	SetPrimaryHBlankCallback(VESOnHBlank);
}

void VESProcDrawBase(struct VESProc* proc) {
	FillBgMap(gBg0MapBuffer, 0);
	FillBgMap(gBg1MapBuffer, 0);

	MakeUIWindowTileMap_BG0BG1(2, 2, 26, 16, 0);

	VESStartUnitDisplay(proc, 0, GetUnit(0x01));

	EnableBgSyncByIndex(0);
	EnableBgSyncByIndex(1);
}

void VESCleanup(struct VESProc* proc) {
	SetPrimaryHBlankCallback(NULL);
}

void VESCallFromEvents(Proc* evProc) {
	StartBlockingProc(gVESProcScr, evProc);
}
