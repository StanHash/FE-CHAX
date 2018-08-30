#ifndef GBAFE_DEBUG_PRINT_H
#define GBAFE_DEBUG_PRINT_H

#include "common.h"

#pragma long_calls

void SetupDebugFontForBG(int bgIndex, unsigned gfxTileBase); //! FE8U = 0x800378D
void DBG_BG_Print(u16* target, const char* string); //! FE8U = 0x8003805
void SetupDebugFontForOBJ(int gfxObjTileBase, unsigned objPalIndex); //! FE8U = 0x8003B25
void PrintDebugStringAsOBJ(int x, int y, const char* string); //! FE8U = 0x8003BB1
void PrintDebugNumberDec(int x, int y, unsigned Number, unsigned DigitCount); //! FE8U = 0x8003BFD
void PrintDebugNumberHex(int x, int y, unsigned Number, unsigned DigitCount); //! FE8U = 0x8003C21

#pragma long_calls_off

#endif // GBAFE_DEBUG_PRINT_H
