
#include "gbafe.h"

// Use those
void SlInit(unsigned textTileBase, unsigned bgPal, unsigned portrait, struct Proc* proc);
void SlEnd(void);
void SlStartDialogue(const char* cstring, struct Proc* proc);

// TODO: add to CLib reference
static void(* const InitShopScreenGfx)(void) = (void(*)(void))(0x080B52CC+1);
static void(* const StartDialogueFace)(unsigned, int, int, unsigned, unsigned) = (void(*)(unsigned, int, int, unsigned, unsigned))(0x08007938+1);
static void(* const StartUiGoldBox)(struct Proc*) = (void(*)(struct Proc*))(0x080B4E24+1);
static void(* const Dialogue_ClearLines)(void) = (void(*)(void))(0x08008250+1);

static const struct ProcInstruction* const gProc_GoldBox = (const void*)(0x08A394C0);
static const void* const gTsa_ShopBackground = (const void*)(0x08A295D4);

// TODO: add to CLib headers
extern const u16 gPal_ChapterIntroBackground[];
extern const u8 gGfx_ChapterIntroBackground[];

void WriteUIWindowTileMap(u16* bg, int x, int y, int width, int height, int tileBase, int style);

void SlInit(unsigned textTileBase, unsigned bgPal, unsigned portrait, struct Proc* proc)
{
	FOR_EACH_PROC(gProc_MoveUnit, MU_Hide);

	InitShopScreenGfx();

	gLCDIOBuffer.bgControl[0].priority = 0;
	gLCDIOBuffer.bgControl[1].priority = 2;
	gLCDIOBuffer.bgControl[2].priority = 0;
	gLCDIOBuffer.bgControl[3].priority = 3;

	Dialogue_InitGfx(textTileBase, 2, FALSE);

	Face_Init();
	StartDialogueFace(portrait, 32, 8, 3, 1);

	WriteUIWindowTileMap(gBg1MapBuffer, 0, 0, 8, 8, TILEREF(0, 0), 2);

	// Decompress(gcTsa_ShopThing, gGenericBuffer);
	// BgMap_ApplyTsa(gBg1MapBuffer, gGenericBuffer, TILEREF(0, AF_BPAL_WFRM));

	StartUiGoldBox(proc);

	// Setup window and color effects (all that for dark bg of text)

	gLCDIOBuffer.dispControl.enableWin0   = FALSE;
	gLCDIOBuffer.dispControl.enableWin1   = TRUE;
	gLCDIOBuffer.dispControl.enableObjWin = FALSE;

	gLCDIOBuffer.winControl.win1_enableBg0   = TRUE;
	gLCDIOBuffer.winControl.win1_enableBg1   = TRUE;
	gLCDIOBuffer.winControl.win1_enableBg2   = TRUE;
	gLCDIOBuffer.winControl.win1_enableBg3   = TRUE;
	gLCDIOBuffer.winControl.win1_enableObj   = TRUE;
	gLCDIOBuffer.winControl.win1_enableBlend = TRUE;

	gLCDIOBuffer.winControl.wout_enableBg0   = TRUE;
	gLCDIOBuffer.winControl.wout_enableBg1   = TRUE;
	gLCDIOBuffer.winControl.wout_enableBg2   = TRUE;
	gLCDIOBuffer.winControl.wout_enableBg3   = TRUE;
	gLCDIOBuffer.winControl.wout_enableObj   = TRUE;
	gLCDIOBuffer.winControl.wout_enableBlend = FALSE;

	SetWin1Box( 0,8, 240,56 );

	SetColorEffectsParameters(BLEND_EFFECT_DARKEN, 0, 0, 8);

	SetColorEffectsFirstTarget(  FALSE, FALSE, FALSE, TRUE,  FALSE );
	SetColorEffectsSecondTarget( FALSE, FALSE, FALSE, FALSE, FALSE );

	Decompress(gGfx_ChapterIntroBackground, VRAM + GetBgTileDataOffset(3));
	ApplyPalettes(gPal_ChapterIntroBackground, bgPal, 2);
	BgMap_ApplyTsa(gBg3MapBuffer, gTsa_ShopBackground, TILEREF(0, bgPal));
}

void SlEnd(void)
{
	EndEachProc(gProc_GoldBox);
	FOR_EACH_PROC(gProc_MoveUnit, MU_Show);
}

void SlStartDialogue(const char* cstring, struct Proc* proc)
{
	Dialogue_InitFont();
	Dialogue_ClearLines();

	StartDialogue(8, 2, cstring, proc);

	Dialogue_SetDefaultTextColor(0);
	Dialogue_SetActiveFacePosition(1);
	SetDialogueFlag(DIALOGUE_1 | DIALOGUE_2 | DIALOGUE_NOSKIP);
}
