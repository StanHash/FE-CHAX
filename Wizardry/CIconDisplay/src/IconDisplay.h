#ifndef ICON_DISPLAY_H
#define ICON_DISPLAY_H

#include "gbafe.h"

#define MAX_SIMULTANEOUS_ICONS 0x20

// Core functions
u16 GetIconTileIndex(int icon);
void LoadIconObjectGraphics(int icon, int tile);
void ClearIcons(void);
void ClearIcon(unsigned icon);

// User function
const void* GetIconGfx(unsigned icon);

#endif // ICON_DISPLAY_H
