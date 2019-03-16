#include "CSS.h"

static void css_mugframe_small_on_update(struct CSSMugFrameSmallProc*);

static const ProcCode sProc_CSSMugFrameSmall[] = {
	PROC_SET_NAME(CSS_PREFIX":MugFrameSmall"),
	PROC_LOOP_ROUTINE(css_mugframe_small_on_update),
	PROC_END
};

struct CSSMugFrameSmallProc* css_mugframe_small_start(uint16_t* pBgOut, uint16_t rootTileIndex) {
	struct CSSMugFrameSmallProc* proc;

	proc = (struct CSSMugFrameSmallProc*) Proc_Create(sProc_CSSMugFrameSmall, ROOT_PROC_3);

	proc->rootTileIndex = rootTileIndex;
	proc->pBgOutput = pBgOut;

	css_mugframe_small_redraw(proc);

	return proc;
}

void css_mugframe_small_end(struct CSSMugFrameSmallProc* proc) {
	Proc_Delete((Proc*)(proc));
}

void css_mugframe_small_on_update(struct CSSMugFrameSmallProc* proc) {
	// nothing (yet?)
}

void css_mugframe_small_redraw(struct CSSMugFrameSmallProc* proc) {
	// void DrawMiniMug(int index, uint16_t* targetBg, int tileIndex, int bgIndex, int flipped);
	static const void(*DrawMiniMug)(int, uint16_t*, int, int, int) = (void (*)(int, uint16_t*, int, int, int))(0x08005989);

	DrawMiniMug(
		GetUnitMiniPortraitId(gpStatScreenUnit), // TODO: gStatScreenUnit
		BG_LOCATED_TILE(proc->pBgOutput, 1, 1),
		proc->rootTileIndex,
		CSS_PAL_BG_MUG,
		FALSE
	);

	Text_SetFont(NULL);

	// TODO: draw actual information
	DrawTextInline(NULL, BG_LOCATED_TILE(proc->pBgOutput, 6, 1), 0, 0, 0x10, GetStringFromIndex(gpStatScreenUnit->pCharacterData->nameTextId));
	DrawTextInline(NULL, BG_LOCATED_TILE(proc->pBgOutput, 6, 3), 0, 0, 0x10, GetStringFromIndex(gpStatScreenUnit->pClassData->nameTextId));

	css_display_hp_exp_line(BG_LOCATED_TILE(proc->pBgOutput, 1, 5));
}
