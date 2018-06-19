#include "css_mugframe_small.h"

static void css_mugframe_small_on_update(CSSMugFrameSmallProc*);

static const ProcCode sProc_CSSMugFrameSmall[] = {
	PROC_SET_NAME(CSS_PREFIX":MugFrameSmall"),
	PROC_LOOP_ROUTINE(css_mugframe_small_on_update),
	PROC_END
};

CSSMugFrameSmallProc* css_mugframe_small_start(uint16_t* pBgOut, uint16_t rootTileIndex) {
	CSSMugFrameSmallProc* proc;

	proc = (CSSMugFrameSmallProc*) Proc_Create(sProc_CSSMugFrameSmall, ROOT_PROC_3);

	proc->rootTileIndex = rootTileIndex;
	proc->pBgOutput = pBgOut;

	css_mugframe_small_redraw(proc);

	return proc;
}

void css_mugframe_small_end(CSSMugFrameSmallProc* proc) {
	Proc_Delete((Proc*)(proc));
}

void css_mugframe_small_on_update(CSSMugFrameSmallProc* proc) {
	// nothing (yet?)
}

void css_mugframe_small_redraw(CSSMugFrameSmallProc* proc) {
	// void DrawMiniMug(int index, uint16_t* targetBg, int tileIndex, int bgIndex, int flipped);
	static const void(*DrawMiniMug)(int, uint16_t*, int, int, int) = (void (*)(int, uint16_t*, int, int, int))(0x08005989);

	DrawMiniMug(
		GetUnitMiniPortraitId(gActiveUnit), // TODO: gStatScreenUnit
		BG_LOCATED_TILE(proc->pBgOutput, 1, 1),
		proc->rootTileIndex,
		CSS_PAL_BG_MUG,
		FALSE
	);

	SetFont(NULL);

	// TODO: draw actual information
	DrawTextInline(NULL, BG_LOCATED_TILE(proc->pBgOutput, 6, 1), 0, 0, 0x10, "Name");
	DrawTextInline(NULL, BG_LOCATED_TILE(proc->pBgOutput, 6, 3), 0, 0, 0x10, "Class");

	DrawTextInline(NULL, BG_LOCATED_TILE(proc->pBgOutput, 1, 5), 0, 0, 6, "HP42/42");
	DrawTextInline(NULL, BG_LOCATED_TILE(proc->pBgOutput, 7, 5), 0, 0, 6, "L01 E99");
}
