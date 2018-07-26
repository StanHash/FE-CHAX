#ifndef GBAFE_ARMFUNC_H
#define GBAFE_ARMFUNC_H

#include "common.h"
#include "hiobj.h"

#pragma long_calls

void InitRamFuncs(void); //! FE8U = 0x8002AF9

void HuffmanTextDecomp(const char* source, char* target); //! FE8U = 0x8002BA5
void FillMovementMapCore(void); //! FE8U = 0x8002BF5

void BgTileMap_ApplyTSA(u16* target, const void* source, u16 tileBase); //! FE8U = 0x80D74A1
void BgTileMap_FillRect(u16* target, int width, int height); //! FE8U = 0x80D74A9
void BgTileMap_CopyRect(const u16* source, u16* target, int width, int height); //! FE8U = 0x80D74B9

#pragma long_calls_off

// decomp compat
#define StoreRoutinesToIRAM InitRamFuncs
#define CallARM_DecompText HuffmanTextDecomp
#define CallARM_FillMovementMap FillMovementMapCore

#endif // GBAFE_ARMFUNC_H
