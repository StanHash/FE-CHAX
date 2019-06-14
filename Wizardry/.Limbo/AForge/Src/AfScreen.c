
#include "Af.h"

struct AfScreenProc
{
	PROC_HEADER;

	/* 2C */ struct Unit* unit;
};

static void AfScreen_OnInit(struct AfScreenProc* proc);
static void AfScreen_OnEnd(struct AfScreenProc* proc);

static const struct ProcInstruction sProc_AForgeScreen[] = {
	PROC_SET_NAME("Stan : A Forge"),
	PROC_SET_DESTRUCTOR(AfScreen_OnEnd),

	PROC_YIELD,

	PROC_CALL_ROUTINE(AfScreen_OnInit),

	PROC_CALL_ROUTINE(StartFadeOutBlackMedium),
	PROC_LOOP_ROUTINE(WaitForFade),

	PROC_BLOCK,

	PROC_END
};

void AfStartScreen(struct Proc* parent)
{
	struct AfScreenProc* proc = NULL;

	if (parent)
		proc = (struct AfScreenProc*) ProcStartBlocking(sProc_AForgeScreen, parent);
	else
		proc = (struct AfScreenProc*) ProcStart(sProc_AForgeScreen, ROOT_PROC_3);

	proc->unit = gActiveUnit;
}

static void AfScreen_OnInit(struct AfScreenProc* proc)
{
	LockGameLogic();
	LockGameGraphicsLogic();

	// Do shit

	static const unsigned BACKGROUND_BGPAL = 14;

	static const u8* const IMG = (const u8*) (0x08B12DB4);
	static const u16* const PAL = (const u16*) (0x08B1754C);
	static const u16* const TSA = (const u16*) (0x08A295D4);

	Decompress(IMG, VRAM + GetBgTileDataOffset(3));
	ApplyPalettes(PAL, BACKGROUND_BGPAL, 2);
	BgMap_ApplyTsa(gBg3MapBuffer, TSA, TILEREF(0, BACKGROUND_BGPAL));

	EnableBgSyncByMask(BG3_SYNC_BIT);
	SetBgPosition(3, 0, 0);
}

static void AfScreen_OnEnd(struct AfScreenProc* proc)
{
	UnlockGameGraphicsLogic();
	UnlockGameLogic();
}
