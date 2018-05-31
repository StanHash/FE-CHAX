#ifndef GBAFE_ARMFUNC_H
#define GBAFE_ARMFUNC_H

#include "common.h"
#include "hiobj.h"

#pragma long_calls

void InitRamFuncs(void); //! FE8U = 0x8002AF9

void HuffmanTextDecomp(const char* source, char* target); //! FE8U = 0x8002BA5
void FillMovementMapCore(void); //! FE8U = 0x8002BF5

void CallARM_FillTileRect(uint16_t* target, const void* source, uint16_t tileBase); //! FE8U = 0x80D74A1
void TileMap_FillRect(uint16_t* target, int width, int height); //! FE8U = 0x80D74A9
void TileMap_CopyRect(const uint16_t* source, uint16_t* target, int width, int height); //! FE8U = 0x80D74B9

#pragma long_calls_off

// better names
#define BgTileMap_ApplyTSA CallARM_FillTileRect
#define BgTileMap_FillRect TileMap_FillRect
#define BgTileMap_CopyRect TileMap_CopyRect

// decomp compat
#define StoreRoutinesToIRAM InitRamFuncs
#define CallARM_DecompText HuffmanTextDecomp
#define CallARM_FillMovementMap FillMovementMapCore

#endif // GBAFE_ARMFUNC_H
