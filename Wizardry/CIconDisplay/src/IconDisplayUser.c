#include "IconDisplay.h"

#define ICON_SHEET(icon) (((icon) >> 8) & 0xFF)
#define ICON_INDEX(icon) ((icon) & 0xFF)

extern const void* const IDRIconSheets[];

const void* GetIconGfx(unsigned icon) {
	return IDRIconSheets[ICON_SHEET(icon)] + 0x80 * ICON_INDEX(icon);
}
