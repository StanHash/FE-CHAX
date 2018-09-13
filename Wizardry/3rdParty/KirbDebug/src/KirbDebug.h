#ifndef KIRB_DEBUG_H
#define KIRB_DEBUG_H

#include "gbafe.h"

enum { UnitEditor_PAGE1ENTRIES = 12 };
enum { UnitEditor_PAGE2ENTRIES = 5 };

enum { // General Config
	// (inner) frame size (aka the area where pages should draw in)
	KD_PAGE_FRAME_X      = 1,
	KD_PAGE_FRAME_Y      = 3,
	KD_PAGE_FRAME_WIDTH  = 28,
	KD_PAGE_FRAME_HEIGHT = 16,

	// frame "style" (0-3)
	KD_PAGE_FRAME_STYLE = 0,
};

typedef struct {
	PROC_HEADER;

	u8 CursorIndex;
	u8 StatsPage1[UnitEditor_PAGE1ENTRIES];
	u8 StatsPage2[UnitEditor_PAGE2ENTRIES];
	u8 KonamiCodeCounter;
	u8 PageIndex;

	struct Unit* pUnit;
	struct Proc* pPageProc;
} UnitEditorProc;

typedef struct {
	u32 x;
	u32 y;
} LocationTable;

struct KDPageDefinition {
	struct Proc* (*start) (UnitEditorProc*); // starts the page proc

	void (*onLoadUnit) (struct Proc*, UnitEditorProc*, struct Unit*); // notifies page proc of unit switch in
	void (*onSaveUnit) (struct Proc*, UnitEditorProc*, struct Unit*); // notifies page proc of unit switch out
};

extern const struct KDPageDefinition gKDStatPageDefinition;

extern const struct ProcInstruction Debug6C[];
extern const LocationTable CursorLocationTable[];
extern const u16 ButtonCombo[];

u8 KribDebugMenuEffect(void);

void KDSwitchPage(UnitEditorProc*, unsigned id);
void KDSwitchUnit(UnitEditorProc*, struct Unit* unit);
void KDClearPage(UnitEditorProc*);
void KDClearHeader(UnitEditorProc*);

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
