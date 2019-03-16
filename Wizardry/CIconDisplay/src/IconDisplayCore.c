#include "IconDisplay.h"

extern u16* const IDRIconUsageLookup;

static unsigned GetIconUsedSlot(unsigned icon);
static unsigned GetIconNewSlot(unsigned icon);

u16 GetIconTileIndex(int icon) {
	unsigned slot, tile;

	// Check if icon is already loaded

	if ((slot = GetIconUsedSlot(icon)))
		return GetIconGfxTileIndex(slot);

	// Register new icon

	slot = GetIconNewSlot(icon);
	tile = GetIconGfxTileIndex(slot);

	// Register icon graphics

	RegisterTileGraphics(
		GetIconGfx(icon),
		(void*) (VRAM + tile * 0x20),
		0x80
	);

	return tile;
}

void LoadIconObjectGraphics(int icon, int tile) {
	void* target = VRAM_OBJ + (tile * 0x20);

	if (icon >= 0) {
		const void* source = GetIconGfx(icon);

		RegisterTileGraphics(source,        target,         0x40);
		RegisterTileGraphics(source + 0x40, target + 0x400, 0x40);
	} else {
		// no icon, we clear the target graphics

		RegisterFillTile(0, target,         0x40);
		RegisterFillTile(0, target + 0x400, 0x40);
	}
}

void ClearIcons(void) {
	CpuFill32(-1, IDRIconUsageLookup, MAX_SIMULTANEOUS_ICONS * sizeof(u16));
}

void ClearIcon(unsigned icon) {
	u16* it  = IDRIconUsageLookup;
	u16* end = IDRIconUsageLookup + MAX_SIMULTANEOUS_ICONS;

	for (; it != end; ++it) {
		if (*it == icon) {
			*it = -1;
			break;
		}
	}
}

unsigned GetIconUsedSlot(unsigned icon) {
	u16* it  = IDRIconUsageLookup;
	u16* end = IDRIconUsageLookup + MAX_SIMULTANEOUS_ICONS;

	for (; it != end; ++it)
		if (*it == icon)
			return (it - IDRIconUsageLookup) + 1;

	return 0;
}

unsigned GetIconNewSlot(unsigned icon) {
	u16* it  = IDRIconUsageLookup;
	u16* end = IDRIconUsageLookup + MAX_SIMULTANEOUS_ICONS;

	for (; it != end; ++it) {
		if (*it == (u16)(-1)) {
			*it = icon;
			return (it - IDRIconUsageLookup) + 1;
		}
	}

	return 0;
}
