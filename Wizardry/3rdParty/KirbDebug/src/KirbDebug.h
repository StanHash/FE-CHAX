#ifndef KIRB_DEBUG_H
#define KIRB_DEBUG_H

#include "gbafe.h"

enum { UnitEditor_PAGE1ENTRIES = 12 };
enum { UnitEditor_PAGE2ENTRIES = 4 };

typedef struct {
	PROC_HEADER;

	struct Unit* pUnit;

	u8 CursorIndex;
	u8 StatsPage1[UnitEditor_PAGE1ENTRIES+1];
	u8 KonamiCodeCounter;
	u8 PageIndex;
	u8 StatsPage2[UnitEditor_PAGE2ENTRIES+1];
} UnitEditorProc;

typedef struct {
	u32 x;
	u32 y;
} LocationTable;

extern const struct ProcInstruction Debug6C[];
extern const LocationTable CursorLocationTable[];
extern const u16 ButtonCombo[];

u8 KribDebugMenuEffect();
u8 WaitUpdate();
void DebugScreenSetup(UnitEditorProc* CurrentProc);
void DebugScreenLoop(UnitEditorProc* CurrentProc);
void GenerateBGTsa(u16* MapOffset, u32 NumberOfTiles, u8 PaletteId);
void SetupDebugUnitEditorPage1(UnitEditorProc* CurrentProc);
void UpdateDebugUnitEditorPage1(UnitEditorProc* CurrentProc);
void CheckKonamiCode(UnitEditorProc* CurrentProc);
void PrintConstantsPage1();
void PrintConstantsPage2();
void SetupDebugUnitEditorPage2(UnitEditorProc* CurrentProc);
void UpdateDebugUnitEditorPage2(UnitEditorProc* CurrentProc);

#endif // KIRB_DEBUG_H
