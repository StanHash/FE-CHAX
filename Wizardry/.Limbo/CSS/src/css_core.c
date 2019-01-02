#include "css_core.h"

void css_disable_game_gfx(CSSCoreProc* proc);
void css_load_gfx(CSSCoreProc* proc);
void css_disable_gfx(CSSCoreProc* proc);

const ProcCode proc_css[] = {
	PROC_SET_NAME(CSS_PREFIX":Core"),

	// Clearing game graphics
	PROC_CALL_ROUTINE(LockGameGraphicsLogic),
	PROC_CALL_ROUTINE(css_disable_game_gfx),
	
	// 2 frames because this might get executed twice on first frame
	// Or something
	PROC_SLEEP(2),

	// Loading graphics (while lcd is not displaying)
	PROC_CALL_ROUTINE(css_load_gfx),
	PROC_SLEEP(0x20),

	// For testing
	PROC_BLOCK,

	PROC_CALL_ROUTINE(css_disable_gfx),

	// Restoring game graphics
	PROC_CALL_ROUTINE(UnlockGameGraphicsLogic),

	PROC_END
};

static const uint16_t css_bg_config[] = {
	0x0000, // bg0 tile data offset
	0x6000, // bg0 map data offset
	0x0000, // bg0 screen size

	0x0000,
	0x6800,
	0x0000,

	0x8000,
	0x7000,
	0x0000,

	0x8000,
	0x7800,
	0x0000
};

void css_disable_game_gfx(CSSCoreProc* proc) {
	// disabling every layer
	gLCDIOBuffer.dispControl.bg0_on = 0;
	gLCDIOBuffer.dispControl.bg1_on = 0;
	gLCDIOBuffer.dispControl.bg2_on = 0;
	gLCDIOBuffer.dispControl.bg3_on = 0;
	gLCDIOBuffer.dispControl.obj_on = 0;

	// resetting color effects?
	SetColorEffectParameters(3, 0, 0, 0x10);

	SetColorEffectFirstTarget(0, 0, 0, 0, 0);
	SetColorEffectBackdropFirstTarget(1);
	SetColorEffectBackdropSecondTarget(0);

	// resetting palette?
	gPaletteBuffer[0] = 0;
	EnablePaletteSync();
}

void css_load_gfx(CSSCoreProc* proc) {
	LoadBgConfig(css_bg_config);

	Font_InitDefault();

	const Vector2 zeroVec = { 0, 0 };

	proc->pLPanel = css_lpanel_start(zeroVec, (Proc*)(proc));

	Decompress(css_mugframe_small_gfx, (void*)(VRAM + 0x20));

	for (int ix = 0; ix < 14; ++ix)
		for (int iy = 0; iy < 8; ++iy)
			*BG_LOCATED_TILE(gBg1MapBuffer, ix, iy) = 14*iy + ix + 1;

	// enable every layer
	gLCDIOBuffer.dispControl.bg0_on = 1;
	gLCDIOBuffer.dispControl.bg1_on = 1;
	gLCDIOBuffer.dispControl.bg2_on = 1;
	gLCDIOBuffer.dispControl.bg3_on = 1;
	gLCDIOBuffer.dispControl.obj_on = 1;
}

void css_disable_gfx(CSSCoreProc* proc) {
	css_lpanel_end(proc->pLPanel);
}
