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

	PROC_CALL_ROUTINE(LockGameGraphicsLogic),
	PROC_YIELD,

	PROC_CALL_ROUTINE(VESSetupHBlank),

	PROC_CALL_ROUTINE(VESProcDrawBase),
	PROC_BLOCK,

	PROC_CALL_ROUTINE(UnlockGameGraphicsLogic),
	PROC_END
};

static void VESOnHBlank(void) {
	if (!(VCOUNT % 16))
		return;

	unsigned win0h = gLCDIOBuffer.win0h;

	win0h = (win0h &~ 0xFF) | (0 & 0xFF);

	// TODO: fix
	// REG_WIN0H = win0h;
}

void VESSetupHBlank(struct VESProc* proc) {
	gLCDIOBuffer.win0h = 0xFF00;
	gLCDIOBuffer.win0v = 0xFF20;

	gLCDIOBuffer.winControl.wout_enableBg0 = FALSE;
	gLCDIOBuffer.winControl.wout_enableObj = FALSE;

	gLCDIOBuffer.dispControl.enableWin0 = TRUE;

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
	ProcStartBlocking(gVESProcScr, evProc);
}
