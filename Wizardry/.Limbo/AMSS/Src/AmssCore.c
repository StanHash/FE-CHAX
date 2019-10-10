
#include "AmssInternal.h"

// TODO: update reference and add to CLib
static struct Unit* (*GetNextStatScreenUnit)(struct Unit* unit, int direction) = (const void*)(0x08087920+1);

static void AmssMain_DebugRedraw(struct AmssMainProc* proc);

void AmssInitModule(const struct AmssModuleEntry* entry, u16* fg, u16* bg, struct AmssMainProc* proc)
{
	if (entry->module->onInit)
		entry->module->onInit(fg, bg, entry->user, entry->rect, proc);
}

void AmssInitModuleAll(struct AmssMainProc* proc)
{
	if (!proc->boolInitialized)
	{
		FillBgMap(gBg0MapBuffer, 0);
		FillBgMap(gBg2MapBuffer, 0);

		for (const struct AmssModuleEntry* it = AmssCoreModules; it->module; ++it)
			AmssInitModule(it, gBg0MapBuffer, gBg2MapBuffer, proc);

		EnableBgSyncByMask(BG0_SYNC_BIT | BG2_SYNC_BIT);
	}

	proc->boolInitialized = TRUE;
}

void AmssEndModule(const struct AmssModuleEntry* entry, u16* fg, u16* bg, struct AmssMainProc* proc)
{
	if (entry->module->onEnd)
		entry->module->onEnd(fg, bg, entry->user, entry->rect, proc);
}

void AmssEndModuleAll(struct AmssMainProc* proc)
{
	if (proc->boolInitialized)
	{
		FillBgMap(gBg0MapBuffer, 0);
		FillBgMap(gBg2MapBuffer, 0);

		for (const struct AmssModuleEntry* it = AmssCoreModules; it->module; ++it)
			AmssEndModule(it, gBg0MapBuffer, gBg2MapBuffer, proc);

		EnableBgSyncByMask(BG0_SYNC_BIT | BG2_SYNC_BIT);
	}

	proc->boolInitialized = FALSE;
}

struct AmssMainProc* AmssGetMain(void)
{
	return (struct AmssMainProc*) ProcFind(AmssMainProcScr);
}

void AmssNotifyRefresh(void)
{
	struct AmssMainProc* proc = AmssGetMain();

	if (proc)
	{
		AmssMain_DebugRedraw(proc);
		EnableBgSyncByMask(BG0_SYNC_BIT | BG2_SYNC_BIT);
	}
}

void AmssChangeUnit(struct AmssMainProc* proc, struct Unit* unit)
{
	AmssEndModuleAll(proc);
	gStatScreenData.unit = unit;
	AmssInitModuleAll(proc);
}

static void AmssVOff_OnLoop(void)
{
	int off = -gStatScreenData.vOffset;

	SetBgPosition(0, 0, off);
	SetBgPosition(2, 0, off);
}

static
const struct ProcInstruction sProc_VOff[] = {
	PROC_SET_NAME("StanScreen VOff"),
	PROC_LOOP_ROUTINE(AmssVOff_OnLoop),
	PROC_END,
};

static void AmssMain_PreInit(struct AmssMainProc* proc);
static void AmssMain_Init(struct AmssMainProc* proc);
static void AmssMain_Loop(struct AmssMainProc* proc);
static void AmssMain_End(struct AmssMainProc* proc);

const struct ProcInstruction AmssMainProcScr[] = {
	PROC_SET_NAME("StanScreen"),

	PROC_CALL_ROUTINE(AmssMain_PreInit),
	PROC_SET_DESTRUCTOR(AmssMain_End),

	PROC_SLEEP(1),

	PROC_CALL_ROUTINE(0x80884B0+1),

	PROC_CALL_ROUTINE(AmssMain_Init),

	PROC_NEW_CHILD(0x08A00938), // test

	PROC_NEW_CHILD(sProc_VOff),

PROC_LABEL(0),
	PROC_SLEEP(1),

	PROC_CALL_ROUTINE(AmssMain_DebugRedraw),

	PROC_LOOP_ROUTINE(AmssMain_Loop),

	PROC_END,
};

static void AmssMain_PreInit(struct AmssMainProc* proc)
{
	proc->boolIsDebug = TRUE;
	proc->boolInitialized = FALSE;

	// Darken screen

	gLCDIOBuffer.dispControl.enableBg0 = FALSE;
	gLCDIOBuffer.dispControl.enableBg1 = FALSE;
	gLCDIOBuffer.dispControl.enableBg2 = FALSE;
	gLCDIOBuffer.dispControl.enableBg3 = FALSE;
	gLCDIOBuffer.dispControl.enableObj = FALSE;

	SetColorEffectsParameters(BLEND_EFFECT_DARKEN, 0, 0, 0x10);

	SetColorEffectsFirstTarget(FALSE, FALSE, FALSE, FALSE, FALSE);
	SetColorEffectBackdropFirstTarget(TRUE);
	SetColorEffectBackdropSecondTarget(FALSE);

	gPaletteBuffer[0] = 0;
	EnablePaletteSync();

	// Lock game graphics

	LockGameGraphicsLogic();
}

static void AmssMain_Init(struct AmssMainProc* proc)
{
	static const u16 bgconfig[] = {
		0x0000, 0x6000, 0,
		0x0000, 0x6800, 0,
		0x8000, 0x7000, 0,
		0x8000, 0x7800, 0,
	};

	gStatScreenData.inTransition = FALSE;

	LoadBgConfig(bgconfig);

	Text_InitFont();
	ClearIcons();

	AmssInitModuleAll(proc);

	gLCDIOBuffer.dispControl.enableBg0 = TRUE;
	gLCDIOBuffer.dispControl.enableBg1 = TRUE;
	gLCDIOBuffer.dispControl.enableBg2 = TRUE;
	gLCDIOBuffer.dispControl.enableBg3 = TRUE;
	gLCDIOBuffer.dispControl.enableObj = TRUE;

	SetDefaultColorEffects();

	// gStatScreenData.page;
}

static void AmssMain_DebugRedraw(struct AmssMainProc* proc)
{
	if (proc->boolIsDebug)
	{
		AmssDebugInit();
		AmssDebugDisplayHelps();
	}
}

static void AmssMain_Loop(struct AmssMainProc* proc)
{
	if (gStatScreenData.inTransition)
		return; // Modules can put the stat screen in transition; and in this case main doesn't read inputs.

	if (gKeyState.repeatedKeys & KEY_DPAD_DOWN)
	{
		AmssStartUnitChange(+1, GetNextStatScreenUnit(gStatScreenData.unit, +1), proc);
		return;
	}

	if (gKeyState.repeatedKeys & KEY_DPAD_UP)
	{
		AmssStartUnitChange(-1, GetNextStatScreenUnit(gStatScreenData.unit, -1), proc);
		return;
	}

	if (gKeyState.repeatedKeys & KEY_BUTTON_A)
	{
		if (gStatScreenData.unit->state & US_RESCUING)
			AmssStartUnitChange(-1, GetUnit(gStatScreenData.unit->rescueOtherUnit), proc);
		else if (gStatScreenData.unit->state & US_RESCUED)
			AmssStartUnitChange(+1, GetUnit(gStatScreenData.unit->rescueOtherUnit), proc);
	}

	if (gKeyState.pressedKeys & KEY_BUTTON_B)
	{
		PlaySfx(0x6B);

		END_PROC(proc);
		return;
	}

	if (gKeyState.pressedKeys & KEY_BUTTON_R)
	{
		ProcGoto((struct Proc*) proc, 0);
		AmssStartHelpBox(0, 0, proc);

		return;
	}
}

static void AmssMain_End(struct AmssMainProc* proc)
{
	gLCDIOBuffer.dispControl.enableBg0 = FALSE;
	gLCDIOBuffer.dispControl.enableBg1 = FALSE;
	gLCDIOBuffer.dispControl.enableBg2 = FALSE;
	gLCDIOBuffer.dispControl.enableBg3 = FALSE;
	gLCDIOBuffer.dispControl.enableObj = FALSE;

	gPaletteBuffer[0] = 0;
	EnablePaletteSync();

	MU_EndAll();
	EndGreenTextColorManager();
	UnlockGameGraphicsLogic();
}
