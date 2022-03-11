#include "CSS.h"

// NEXT TODO: generic obj text
// draw mov/aid/trv/status or some other bunch of stats at the bottom of lps1

/*
 * LEFT PANEL STATE 1 (BIG FACE):
 * (we have one extra tile line because the edge tiles are actually cut 4 pixels)
 * ===============================
 *  - - - - - - - - - - - - - - - 
 *  - - - $$$$$$$$$$$$$$$ - - - - <<< Overlap with the character name.
 *  - - - $$$$$$$$$$$$$$$ - - - - <<< Character name centered. Special font?
 *  - - - $$$$$$$$$$$$$$$ - - - - 
 *  - - - $$$$$$$$$$$$$$$ - - - - 
 *  - - - $$$$$$$$$$$$$$$ - - - - 
 *  - - - $$$$$$$$$$$$$$$ - - - - 
 *  - $$$$$$$$$$$$$$$$$$$$$$$ - - 
 *  - $$$$$$$$$$$$$$$$$$$$$$$ - - 
 *  - $$$$$$$$$$$$$$$$$$$$$$$ - - 
 *  - $$$$$$$$$$$$$$$$$$$$$$$ - - 
 *  - ### CCCCCCCCCCCCC - - - - - <<< Far left is MMS.
 *  - ### CCCCCCCCCCCCC - - - - - <<< next is class name. +Something else?
 *  - @@@@@@@@@@@@@@@@@@@@@@@ - - <<< the HP/L/E display
 *  - @@@@@@@@@@@@@@@@@@@@@@@ - - <<< TODO: align L/E right
 *  - - - - - - - - - - - - - - - 
 *  - - - - - - - - - - - - - - - 
 *  - - - - - - - - - - - - - - - 
 *  - - - - - - - - - - - - - - - 
 *  - - - - - - - - - - - - - - - 
 *  - - - - - - - - - - - - - - - 
 */

static void css_lpanel_init(struct CSSLeftPanelProc*);
static void css_lpanel_idle(struct CSSLeftPanelProc*);
static void css_lpanel_on_end(struct CSSLeftPanelProc*);

static int  css_lpanel_moveup(struct CSSLeftPanelProc*);

static void css_lpanel_show_face(struct CSSLeftPanelProc*);
static void css_lpanel_hide_face(struct CSSLeftPanelProc*);

const struct ProcInstruction proc_css_lpanel[] = {
	PROC_SET_NAME(CSS_PREFIX":LPanel"),
	PROC_SET_DESTRUCTOR(css_lpanel_on_end),
	PROC_YIELD,

	PROC_CALL_ROUTINE(css_lpanel_init),

PROC_LABEL(0),
	PROC_LOOP_ROUTINE(css_lpanel_idle),

PROC_LABEL(1),
	PROC_CALL_ROUTINE_2(css_lpanel_moveup),
	PROC_GOTO(0),

	PROC_END
};

/*
static const uint16_t sCssLPanelTextObjLookup[] = {
	0x180, 0x1C0,
	0x186, 0x1C6,
	0x18C, 0x1CC,
	0x186, 0x1C6,
	0x188, 0x1C8,
	0x18A, 0x1CA,
	0x18C, 0x1CC,
	0x18E, 0x1CE
}; //*/

static void css_lpanel_init(struct CSSLeftPanelProc* proc) {
	proc->faceDisplayed = 0;

	SetBgTileDataOffset(2, 0x8000);

	Decompress(css_lpbg_face_img, (void*) (VRAM + 0x8000 + 0x20));
	CopyToPaletteBuffer(css_lpbg_face_pal, 0x20 * 2, 0x20);

	for (int ix = 0; ix < 14; ++ix)
		for (int iy = 0; iy < 20; ++iy)
			*BG_LOCATED_TILE(gBg2MapBuffer, ix, iy) = (1 + ix + iy*14) | 0x2000;

	SetBgPosition(0, 4, 4);

	//*

	u16* const bgOut = gBg0MapBuffer;

	{
		// Drawing the character name text

		const char* const nameString = GetStringFromIndex(gpStatScreenUnit->pCharacterData->nameTextId);

		struct TextHandle text;
		Text_InitClear(&text, 9);

		Text_SetXCursor(&text, Text_GetStringTextCenteredPos(72, nameString));
		Text_DrawString(&text, nameString);

		Text_Display(&text, BG_LOCATED_TILE(bgOut, 3, 1));
	}

	// drawing the class name text
	DrawTextInline(NULL, BG_LOCATED_TILE(bgOut, 4, 12), 0, 0, 7, GetStringFromIndex(gpStatScreenUnit->pClassData->nameTextId));

	struct MUProc* mu = MU_CreateForUI(gpStatScreenUnit);
	MU_SetDisplayPosition(mu, 16, 108);

	css_display_hp_exp_line(BG_LOCATED_TILE(bgOut, 1, 14));

	css_lpanel_show_face(proc);

	// */

	/*

	css_mugframe_small_start(gBg0MapBuffer, 0xF0);

	struct Vec2 stOrigin = {
		proc->origin.x + 4,
		proc->origin.y + 160 - 54
	};

	proc->pStatText = css_stattext_start(stOrigin);

	css_stattext_show(proc->pStatText, 0, (UnitHasMagicRank(gpStatScreenUnit) ? "Mag" : "Str"), gpStatScreenUnit->pow, gpStatScreenUnit->pClassData->maxPow);
	css_stattext_show(proc->pStatText, 2, "Skl", gpStatScreenUnit->skl, gpStatScreenUnit->pClassData->maxSkl);
	css_stattext_show(proc->pStatText, 4, "Spd", gpStatScreenUnit->spd, gpStatScreenUnit->pClassData->maxSpd);
	css_stattext_show(proc->pStatText, 6, "Lck", gpStatScreenUnit->lck, 30);
	css_stattext_show(proc->pStatText, 1, "Def", gpStatScreenUnit->def, gpStatScreenUnit->pClassData->maxDef);
	css_stattext_show(proc->pStatText, 3, "Res", gpStatScreenUnit->res, gpStatScreenUnit->pClassData->maxRes);

	css_stattext_show(proc->pStatText, 5, "Con",
		gpStatScreenUnit->pClassData->baseCon + gpStatScreenUnit->pCharacterData->baseCon + gpStatScreenUnit->conBonus,
		gpStatScreenUnit->pClassData->maxCon
	);

	css_stattext_show(proc->pStatText, 7, "Mov",
		gpStatScreenUnit->pClassData->baseMov + gpStatScreenUnit->movBonus,
		15
	);

	// */

	CopyToPaletteBuffer(css_stattext_pal, CSS_PAL_OBJ_STATTEXT * 0x20, 0x20);
}

static void css_lpanel_idle(struct CSSLeftPanelProc* proc) {
	if (gKeyState.pressedKeys & KEY_BUTTON_A)
		css_lpanel_set_face_displayed(proc, !css_lpanel_is_face_displayed(proc));

	/*
	for (int i = 0; i < 8; ++i)
		css_stattext_display(
			proc->pStatText,
			i,
			proc->origin.x + 8 + 64 - 16,
			proc->origin.y + 48 + (12 * i)
		); //*/
}

static void css_lpanel_on_end(struct CSSLeftPanelProc* proc) {
	css_lpanel_hide_face(proc);

	css_stattext_end(proc->pStatText);
}

static int css_lpanel_moveup(struct CSSLeftPanelProc* proc) {
	return 1;
}

static void css_lpanel_show_face(struct CSSLeftPanelProc* proc) {
	if (!proc->faceDisplayed)
		StartFace(0, GetUnitPortraitId(gpStatScreenUnit), 0x38, 0x08, 3);

	proc->faceDisplayed = 1;
}

static void css_lpanel_hide_face(struct CSSLeftPanelProc* proc) {
	if (proc->faceDisplayed)
		EndFaceById(0);
	
	proc->faceDisplayed = 0;
}

struct CSSLeftPanelProc* css_lpanel_start(struct Vec2 origin, Proc* parent) {
	struct CSSLeftPanelProc* result;

	result = (struct CSSLeftPanelProc*) ProcStart(proc_css_lpanel, parent);

	result->origin = origin;

	return result;
}

void css_lpanel_end(struct CSSLeftPanelProc* proc) {
	EndProc((struct Proc*)(proc));
}

int css_lpanel_is_face_displayed(struct CSSLeftPanelProc* proc) {
	return proc->faceDisplayed;
}

void css_lpanel_set_face_displayed(struct CSSLeftPanelProc* proc, int display) {
	if (display)
		css_lpanel_show_face(proc);
	else
		css_lpanel_hide_face(proc);
}
