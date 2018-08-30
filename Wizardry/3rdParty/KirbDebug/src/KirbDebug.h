#ifndef KIRB_DEBUG_H
#define KIRB_DEBUG_H

#include "gbafe.h"

#define UnitEditor_PAGE1ENTRIES 12
#define UnitEditor_PAGE2ENTRIES 4

typedef struct {
	Proc Header;
	u8 UnitPoolIndex;
	u8 CursorIndex;
	u8 StatsPage1[UnitEditor_PAGE1ENTRIES+1];
	u8 KonamiCodeCounter;
	u8 CurrentUnitPool;
	u8 PageIndex;
	u8 StatsPage2[UnitEditor_PAGE2ENTRIES+1];
} UnitEditorProc;

typedef struct {
	u32 x;
	u32 y;
} LocationTable;

const _ProcCode Debug6C[];
const LocationTable CursorLocationTable[];
const u16 ButtonCombo[];

u8 EndEffect();
u8 WaitUpdate();
void DebugScreenSetup(UnitEditorProc *CurrentProc);
void DebugScreenLoop(UnitEditorProc *CurrentProc);
void GenerateBGTsa(u16 *MapOffset, u32 NumberOfTiles, u8 PaletteId);
void SetupDebugUnitEditorPage1(UnitEditorProc *CurrentProc, UnitStruct *CurrentUnitPool);
void UpdateDebugUnitEditorPage1(UnitEditorProc *CurrentProc, UnitStruct *CurrentUnitPool);
void CheckKonamiCode(UnitEditorProc *CurrentProc);
void PrintConstantsPage1();
void PrintConstantsPage2();
void SetupDebugUnitEditorPage2(UnitEditorProc *CurrentProc, UnitStruct *CurrentUnitPool);
void UpdateDebugUnitEditorPage2(UnitEditorProc *CurrentProc, UnitStruct *CurrentUnitPool);

#endif // KIRB_DEBUG_H
