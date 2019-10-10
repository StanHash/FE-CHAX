
#include "AmssInternal.h"

void AmssDebugClear(void)
{
	FillBgMap(gBg3MapBuffer, TILEREF(1, 12));
	EnableBgSyncByMask(BG3_SYNC_BIT);
}

void AmssDebugInit(void)
{
	CpuFastFill16(0, VRAM + 0x8000, 0x20);

	for (unsigned i = 0; i < 4; ++i)
	{
		u16 val = 0x1111 + (i + (i << 4) + (i << 8) + (i << 12));
		CpuFastFill16(val, VRAM + 0x8020 + i*0x20, 0x20);

		gPaletteBuffer[0x10*12 + i+1] = COLOR_RGB(i*6, i*6, i*6); // Regular
		gPaletteBuffer[0x10*13 + i+1] = COLOR_RGB(i*2, i*5, i*8); // Selected
		gPaletteBuffer[0x10*14 + i+1] = COLOR_RGB(i*2, i*8, i*5); // Neighbor
	}

	EnablePaletteSync();
	AmssDebugClear();
}

void AmssDebugDisplayRect(struct AmssGeometry rect)
{
	for (unsigned iy = rect.y; iy < rect.y + rect.h; ++iy)
		for (unsigned ix = rect.x; ix < rect.x + rect.w; ++ix)
			gBg3MapBuffer[TILEMAP_INDEX(ix, iy)] += TILEREF(1, 0);

	EnableBgSyncByMask(BG3_SYNC_BIT);
}

void AmssDebugColorRect(struct AmssGeometry rect, int color)
{
	for (unsigned iy = rect.y; iy < rect.y + rect.h; ++iy)
	{
		for (unsigned ix = rect.x; ix < rect.x + rect.w; ++ix)
		{
			gBg3MapBuffer[TILEMAP_INDEX(ix, iy)] &= 0x0FFF;
			gBg3MapBuffer[TILEMAP_INDEX(ix, iy)] += TILEREF(0, 12 + color);
		}
	}
}

static
void AmssDebugDisplayHelp(const struct AmssHelpInfo* info, void* _)
{
	AmssDebugDisplayRect(info->rect);
}

void AmssDebugDisplayHelps(void)
{
	AmssForEachHelp(AmssDebugDisplayHelp, NULL);
}
