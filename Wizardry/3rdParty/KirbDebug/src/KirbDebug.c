#include "KirbDebug.h"

// FIXME: Add to fe8u.s
static const void(*DisplayHandCursor)(int x, int y) = (void(*)(int, int))(0x804E848+1);
static const void(*DrawMenuItemHighlight)() = (void(*)(int bgIndex, int, int x, int y, int width))(0x804E98C+1);
static const void(*ClearMenuItemHighlight)() = (void(*)(int bgIndex, int, int x, int y, int width))(0x804EA08+1);

// FIXME: Add to libgbafe
void MakeUIWindowTileMap_BG0BG1(int x, int y, int width, int height, int style) __attribute__((long_call));
void LoadNewUIPal(int palIndex) __attribute__((long_call));

void StartFadeInBlack(int speed, struct Proc* parent) __attribute__((long_call)); //! FE8U = 0x8013D09
void NewFadeOutBlack(int speed, struct Proc* parent) __attribute__((long_call)); //! FE8U = 0x8013D21

void ReloadGameCoreGraphics(void) __attribute__((long_call));

void EndBG3Slider(void) __attribute__((long_call));

extern u8 gGfx_ChapterIntroBackground[];
extern u16 gPal_ChapterIntroBackground[];

u8 KribDebugMenuEffect() {
	StartProc(Debug6C, ROOT_PROC_3);
	return ME_END | ME_PLAY_BEEP | ME_CLEAR_GFX;
}

u8 WaitUpdate() {
	return FALSE;
}

void DebugScreenSetup(UnitEditorProc* proc) {
	enum {
		PALBG_BACKGROUND = 14,

		PALOBJ_FONT = 14,
	};

	//Make it so the bg buffers update
	LoadBgConfig(NULL);

	//Load the background graphics and palette and tsa(generate it)
	Decompress(gGfx_ChapterIntroBackground, (void*)(VRAM + 0x8000));
	CopyToPaletteBuffer(gPal_ChapterIntroBackground, PALBG_BACKGROUND * 0x20, 0x20);

	GenerateBGTsa(gBg3MapBuffer, 0x280, PALBG_BACKGROUND);

	CopyToPaletteBuffer(gPal_MiscUIGraphics, 16 * 0x20, 0x20);

	//Load both debug fone(obj one will be used later on)
	SetupDebugFontForBG(2, 0);
	SetupDebugFontForOBJ(-1, PALOBJ_FONT);
	Font_InitDefault();

	// Text_InitClear(0, 0); // wtf why

	//Print a welcome message
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 0, 1), "Kirb's debug-o-matic");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 0, 2), "Press Select to switch pages.");

	//Prints stats
	PrintConstantsPage1();

	//Setup the unit editor™
	proc->PageIndex = 0;
	proc->CursorIndex = 0;
	proc->UnitPoolIndex = 0;
	proc->CurrentUnitPool = 0;

	SetupDebugUnitEditorPage1(proc, GetUnit(UA_BLUE + 1));
}

void DebugScreenLoop(UnitEditorProc* proc) {
	// UpdateBG3HOffset(); // Let's not do that

	if ((gKeyStatus.heldKeys & B_BUTTON) && proc->KonamiCodeCounter != 10) {
		BreakProcLoop((struct Proc*)(proc));
		PlaySfx(0x6B); 
	}
	
	if ((gKeyStatus.pressedKeys & START_BUTTON)) {
		proc->UnitPoolIndex = 0;

		if (proc->CurrentUnitPool == 0) {
			proc->CurrentUnitPool = 1;

			SetupDebugUnitEditorPage1(proc, GetUnit(UA_RED + 1));
			SetupDebugUnitEditorPage2(proc, GetUnit(UA_RED + 1));
		} else {
			proc->CurrentUnitPool = 0;

			SetupDebugUnitEditorPage1(proc, GetUnit(UA_BLUE + 1));
			SetupDebugUnitEditorPage2(proc, GetUnit(UA_BLUE + 1));
		}
	}
	
	if ((gKeyStatus.pressedKeys & SELECT_BUTTON)) {
		if (proc->PageIndex == 0) {
			proc->CursorIndex = 0;
			proc->PageIndex   = 1;

			if (proc->CurrentUnitPool == 0)
				SetupDebugUnitEditorPage2(proc, GetUnit(UA_BLUE + 1));
			else
				SetupDebugUnitEditorPage2(proc, GetUnit(UA_RED + 1));

			PrintConstantsPage2();
		} else {
			proc->CursorIndex = 0;
			proc->PageIndex   = 0;

			if (proc->CurrentUnitPool == 0)
				SetupDebugUnitEditorPage1(proc, GetUnit(UA_BLUE + 1));
			else
				SetupDebugUnitEditorPage1(proc, GetUnit(UA_RED + 1));

			PrintConstantsPage1();
		}
	} else {
		if (proc->PageIndex == 0) {
			if (proc->CurrentUnitPool == 0)
				UpdateDebugUnitEditorPage1(proc, GetUnit(UA_BLUE + 1));
			else
				UpdateDebugUnitEditorPage1(proc, GetUnit(UA_RED + 1));
		} else {
			if (proc->CurrentUnitPool == 0)
				UpdateDebugUnitEditorPage2(proc, GetUnit(UA_BLUE + 1));
			else
				UpdateDebugUnitEditorPage2(proc, GetUnit(UA_RED + 1));
		}

		CheckKonamiCode(proc);
	}
}



void SetupDebugUnitEditorPage1(UnitEditorProc *proc, struct Unit* CurrentUnitPool) {
	proc->StatsPage1[0] = CurrentUnitPool[proc->UnitPoolIndex].pow;
	proc->StatsPage1[1] = CurrentUnitPool[proc->UnitPoolIndex].skl;
	proc->StatsPage1[2] = CurrentUnitPool[proc->UnitPoolIndex].spd;
	proc->StatsPage1[3] = CurrentUnitPool[proc->UnitPoolIndex].lck;
	proc->StatsPage1[4] = CurrentUnitPool[proc->UnitPoolIndex].def;
	proc->StatsPage1[5] = CurrentUnitPool[proc->UnitPoolIndex].res;
	proc->StatsPage1[6] = CurrentUnitPool[proc->UnitPoolIndex].maxHP;
	proc->StatsPage1[7] = CurrentUnitPool[proc->UnitPoolIndex].curHP;
	proc->StatsPage1[8] = CurrentUnitPool[proc->UnitPoolIndex].conBonus;
	proc->StatsPage1[9] = CurrentUnitPool[proc->UnitPoolIndex].movBonus;
	proc->StatsPage1[10] = CurrentUnitPool[proc->UnitPoolIndex].statusIndex;
	proc->StatsPage1[11] = CurrentUnitPool[proc->UnitPoolIndex].statusDuration;
	proc->StatsPage1[12] = CurrentUnitPool[proc->UnitPoolIndex].pCharacterData->number;
}

void UpdateDebugUnitEditorPage1(UnitEditorProc *proc, struct Unit* CurrentUnitPool) {
	//Check for R
	if ((gKeyStatus.pressedKeys & R_BUTTON)) {
		if (CurrentUnitPool[proc->UnitPoolIndex + 1].pCharacterData != 0) {
			proc->UnitPoolIndex++;
			SetupDebugUnitEditorPage1(proc, CurrentUnitPool);
		}
	}
	
	//Check for L
	if ((gKeyStatus.pressedKeys & R_BUTTON)) {
		if (CurrentUnitPool[proc->UnitPoolIndex - 1].pCharacterData  != 0) {
			proc->UnitPoolIndex--;
			SetupDebugUnitEditorPage1(proc, CurrentUnitPool);
		}
	}	
	
	//Check for down
	if ((gKeyStatus.pressedKeys & DPAD_DOWN)) {
		if (proc->CursorIndex < UnitEditor_PAGE1ENTRIES) {
			proc->CursorIndex++;

			ClearMenuItemHighlight(1,0,2,(CursorLocationTable[proc->CursorIndex - 1].y / 8), 23);
			DrawMenuItemHighlight(1,0,2,(CursorLocationTable[proc->CursorIndex].y / 8), 23);
		} else {
			proc->CursorIndex = 0;

			ClearMenuItemHighlight(1,0,2,(CursorLocationTable[UnitEditor_PAGE1ENTRIES].y / 8), 23);
			DrawMenuItemHighlight(1,0,2,(CursorLocationTable[proc->CursorIndex].y / 8), 23);
		}
	}	
	
	//Check for up
	if ((gKeyStatus.pressedKeys & DPAD_UP)) {
		if (proc->CursorIndex != 0) {
			proc->CursorIndex--;

			ClearMenuItemHighlight(1,0,2,(CursorLocationTable[proc->CursorIndex + 1].y / 8), 23);
			DrawMenuItemHighlight(1,0,2,(CursorLocationTable[proc->CursorIndex].y / 8), 23);
		} else {
			proc->CursorIndex = UnitEditor_PAGE1ENTRIES;

			ClearMenuItemHighlight(1,0,2,(CursorLocationTable[0].y / 8), 23);
			DrawMenuItemHighlight(1,0,2,(CursorLocationTable[proc->CursorIndex].y / 8), 23);
		}
	}	
	
	//Check for right
	if ((gKeyStatus.pressedKeys & DPAD_RIGHT)) {
		proc->StatsPage1[proc->CursorIndex]++;
	}	
	
	//Check for left
	if ((gKeyStatus.pressedKeys & DPAD_LEFT) != 0) {
		proc->StatsPage1[proc->CursorIndex]--;
	}	

	//Check for a
	if ((gKeyStatus.pressedKeys & A_BUTTON) != 0) {
		CurrentUnitPool[proc->UnitPoolIndex].pow = proc->StatsPage1[0];
		CurrentUnitPool[proc->UnitPoolIndex].skl = proc->StatsPage1[1];
		CurrentUnitPool[proc->UnitPoolIndex].spd = proc->StatsPage1[2];
		CurrentUnitPool[proc->UnitPoolIndex].lck = proc->StatsPage1[3];
		CurrentUnitPool[proc->UnitPoolIndex].def = proc->StatsPage1[4];
		CurrentUnitPool[proc->UnitPoolIndex].res = proc->StatsPage1[5];
		CurrentUnitPool[proc->UnitPoolIndex].maxHP = proc->StatsPage1[6];
		CurrentUnitPool[proc->UnitPoolIndex].curHP = proc->StatsPage1[7];
		CurrentUnitPool[proc->UnitPoolIndex].conBonus = proc->StatsPage1[8];
		CurrentUnitPool[proc->UnitPoolIndex].movBonus = proc->StatsPage1[9];
		CurrentUnitPool[proc->UnitPoolIndex].statusIndex = proc->StatsPage1[10];
		CurrentUnitPool[proc->UnitPoolIndex].statusDuration = proc->StatsPage1[11];
		CurrentUnitPool[proc->UnitPoolIndex].pClassData = GetClassData(proc->StatsPage1[12]);
	}

	//Get the unit index/name and print it
	char UnitName[0x20];

	GetStringFromIndexInBuffer(CurrentUnitPool[proc->UnitPoolIndex].pCharacterData->nameTextId, UnitName);
	PrintDebugNumberDec(10, 25, proc->UnitPoolIndex, 3);
	PrintDebugStringAsOBJ(48, 25, UnitName);
	
	//Get the class name and prints it
	char ClassName[0x20];

	GetStringFromIndexInBuffer(CurrentUnitPool[proc->UnitPoolIndex].pClassData->nameTextId, ClassName);
	PrintDebugStringAsOBJ(48, 128, ClassName);
	
	//Prints the stats
	PrintDebugNumberDec(10, 40, proc->StatsPage1[0], 3);
	PrintDebugNumberDec(10, 48, proc->StatsPage1[1], 3);
	PrintDebugNumberDec(10, 56, proc->StatsPage1[2], 3);
	PrintDebugNumberDec(10, 64, proc->StatsPage1[3], 3);
	PrintDebugNumberDec(10, 72, proc->StatsPage1[4], 3);
	PrintDebugNumberDec(10, 80, proc->StatsPage1[5], 3);
	PrintDebugNumberDec(10, 88, proc->StatsPage1[6], 3);
	PrintDebugNumberDec(10, 96, proc->StatsPage1[7], 3);
	PrintDebugNumberDec(10, 104, proc->StatsPage1[8], 3);
	PrintDebugNumberDec(10, 112, proc->StatsPage1[9], 3);
	PrintDebugNumberDec(10, 120, proc->StatsPage1[10], 3);
	PrintDebugNumberDec(112, 120, proc->StatsPage1[11], 2);
	PrintDebugNumberHex(10, 128, proc->StatsPage1[12], 3);

	DisplayHandCursor(
		CursorLocationTable[proc->CursorIndex].x,
		CursorLocationTable[proc->CursorIndex].y
	);
}

void GenerateBGTsa(u16* MapOffset, u32 NumberOfTiles, u8 PaletteId) {
	for (int i = 0; i < NumberOfTiles; i++)
		MapOffset[i] = (i | (PaletteId << 12));
}

void CheckKonamiCode(UnitEditorProc* proc) {
	if ((gKeyStatus.pressedKeys & ButtonCombo[proc->KonamiCodeCounter]))
		proc->KonamiCodeCounter++;

	if (proc->KonamiCodeCounter == 11)
		DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 10, 18), "Konami Code");
}

void PrintConstantsPage1() {
	Font_InitDefault();

	FillBgMap(gBg0MapBuffer, 0);
	FillBgMap(gBg1MapBuffer, 0);

	MakeUIWindowTileMap_BG0BG1(0, 2, 26, 16, 0);
	LoadNewUIPal(-1);

	DrawTextInline(NULL, BG_LOCATED_TILE(gBg0MapBuffer, 0, 0), 0, 0, 12, "Kirb's debug-o-matic.");
	DrawTextInline(NULL, BG_LOCATED_TILE(gBg0MapBuffer, 13, 0), 3, 0, 17, "Press Select to switch pages.");

	DrawTextInline(NULL, BG_LOCATED_TILE(gBg0MapBuffer, 0, 18), 4, 0, 20, " Donate to those that need it.");

	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 5), "Power");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 6), "Skill");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 7), "Speed");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 8), "Luck");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 9), "Defense");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 10), "Resistance");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 11), "Max HP");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 12), "Current HP");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 13), "Constitution");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 14), "Move");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 15), "Status");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 17, 15), "Duration");
}

void PrintConstantsPage2() {
	Font_InitDefault();

	FillBgMap(gBg0MapBuffer, 0);
	FillBgMap(gBg1MapBuffer, 0);

	MakeUIWindowTileMap_BG0BG1(0, 2, 29, 9, 3);
	LoadNewUIPal(-1);

	DrawTextInline(NULL, BG_LOCATED_TILE(gBg0MapBuffer, 0, 0), 0, 0, 12, "Kirb's debug-o-matic.");
	DrawTextInline(NULL, BG_LOCATED_TILE(gBg0MapBuffer, 13, 0), 3, 0, 16, "Press Select to switch pages.");
	DrawTextInline(NULL, BG_LOCATED_TILE(gBg0MapBuffer, 0, 18), 4, 0, 9, "Donate To Kirb.");
	DrawTextInline(NULL, BG_LOCATED_TILE(gBg0MapBuffer, 9, 18), 4, 0, 20, "https://www.patreon.com/Kirb1337");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 5), "Item 1:");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 6), "Item 2:");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 7), "Item 3:");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 8), "Item 4:");
	DBG_BG_Print(BG_LOCATED_TILE(gBg0MapBuffer, 6, 9), "Item 5:");
}

void SetupDebugUnitEditorPage2(UnitEditorProc* proc, struct Unit* CurrentUnitPool) {
	proc->StatsPage2[0] = CurrentUnitPool[proc->UnitPoolIndex].items[0] & 0xFF;
	proc->StatsPage2[1] = CurrentUnitPool[proc->UnitPoolIndex].items[1] & 0xFF;
	proc->StatsPage2[2] = CurrentUnitPool[proc->UnitPoolIndex].items[2] & 0xFF;
	proc->StatsPage2[3] = CurrentUnitPool[proc->UnitPoolIndex].items[3] & 0xFF;
	proc->StatsPage2[4] = CurrentUnitPool[proc->UnitPoolIndex].items[4] & 0xFF;
}

void UpdateDebugUnitEditorPage2(UnitEditorProc* proc, struct Unit* CurrentUnitPool) {
	//Check for R
	if ((gKeyStatus.pressedKeys & R_BUTTON)) {
		if (CurrentUnitPool[proc->UnitPoolIndex + 1].pCharacterData  != 0) {
			proc->UnitPoolIndex++;
			SetupDebugUnitEditorPage2(proc, CurrentUnitPool);
		}
	}
	
	//Check for L
	if ((gKeyStatus.pressedKeys & L_BUTTON)) {
		if (CurrentUnitPool[proc->UnitPoolIndex - 1].pCharacterData  != 0) {
			proc->UnitPoolIndex--;
			SetupDebugUnitEditorPage2(proc, CurrentUnitPool);
		}
	}	
	
	//Check for down
	if ((gKeyStatus.pressedKeys & DPAD_DOWN)) {
		if (proc->CursorIndex < UnitEditor_PAGE2ENTRIES) {
			proc->CursorIndex++;

			ClearMenuItemHighlight(1, 0,
				((CursorLocationTable[proc->CursorIndex - 1].x + 6) / 8),
				(CursorLocationTable[proc->CursorIndex - 1].y / 8),
				26
			);

			DrawMenuItemHighlight(1, 0,
				((CursorLocationTable[proc->CursorIndex].x + 6) / 8),
				(CursorLocationTable[proc->CursorIndex].y / 8),
				26
			);
		} else {
			proc->CursorIndex = 0;

			ClearMenuItemHighlight(1, 0,
				((CursorLocationTable[UnitEditor_PAGE2ENTRIES].x + 6) / 8),
				(CursorLocationTable[UnitEditor_PAGE2ENTRIES].y / 8),
				26
			);

			DrawMenuItemHighlight(1, 0,
				((CursorLocationTable[proc->CursorIndex].x + 6) / 8),
				(CursorLocationTable[proc->CursorIndex].y / 8),
				26
			);
		}
	}	
	
	//Check for up
	if ((gKeyStatus.pressedKeys & DPAD_UP)) {
		if (proc->CursorIndex != 0) {
			proc->CursorIndex--;
			ClearMenuItemHighlight(1,0,((CursorLocationTable[proc->CursorIndex + 1].x + 6) / 8),(CursorLocationTable[proc->CursorIndex + 1].y / 8), 26);
			DrawMenuItemHighlight(1,0,((CursorLocationTable[proc->CursorIndex].x + 6) / 8),(CursorLocationTable[proc->CursorIndex].y / 8), 26);
		}
		else {
			proc->CursorIndex = UnitEditor_PAGE2ENTRIES;
			ClearMenuItemHighlight(1,0,((CursorLocationTable[0].x + 6) / 8),(CursorLocationTable[0].y / 8), 26);
			DrawMenuItemHighlight(1,0,((CursorLocationTable[proc->CursorIndex].x + 6) / 8),(CursorLocationTable[proc->CursorIndex].y / 8), 26);
		}
	}	
	
	//Check for right
	if ((gKeyStatus.pressedKeys & DPAD_RIGHT) != 0) {
		proc->StatsPage2[proc->CursorIndex]++;
	}	
	
	//Check for left
	if ((gKeyStatus.pressedKeys & DPAD_LEFT) != 0) {
		proc->StatsPage2[proc->CursorIndex]--;
	}	

	//Check for a
	if ((gKeyStatus.pressedKeys & A_BUTTON) != 0) {
		CurrentUnitPool[proc->UnitPoolIndex].items[0] = proc->StatsPage2[0];
		CurrentUnitPool[proc->UnitPoolIndex].items[1] = proc->StatsPage2[1];
		CurrentUnitPool[proc->UnitPoolIndex].items[2] = proc->StatsPage2[2];
		CurrentUnitPool[proc->UnitPoolIndex].items[3] = proc->StatsPage2[3];
		CurrentUnitPool[proc->UnitPoolIndex].items[4] = proc->StatsPage2[4];
	}	
	
	//Get the unit index/name and print it
	char UnitName[0x20];

	GetStringFromIndexInBuffer(CurrentUnitPool[proc->UnitPoolIndex].pCharacterData->nameTextId, UnitName);
	PrintDebugNumberDec(10, 25, proc->UnitPoolIndex, 3);
	PrintDebugStringAsOBJ(48, 25, UnitName);
	
	//Get Items names
	char ItemName[0x20];

	GetStringFromIndexInBuffer(GetItemData(CurrentUnitPool[proc->UnitPoolIndex].items[0] & 0xFF)->nameTextId, ItemName);
	PrintDebugStringAsOBJ(104, 40, ItemName);
	
	GetStringFromIndexInBuffer(GetItemData(CurrentUnitPool[proc->UnitPoolIndex].items[1] & 0xFF)->nameTextId, ItemName);
	PrintDebugStringAsOBJ(104, 48, ItemName);
	
	GetStringFromIndexInBuffer(GetItemData(CurrentUnitPool[proc->UnitPoolIndex].items[2] & 0xFF)->nameTextId, ItemName);
	PrintDebugStringAsOBJ(104, 56, ItemName);
	
	GetStringFromIndexInBuffer(GetItemData(CurrentUnitPool[proc->UnitPoolIndex].items[3] & 0xFF)->nameTextId, ItemName);
	PrintDebugStringAsOBJ(104, 64, ItemName);
	
	GetStringFromIndexInBuffer(GetItemData(CurrentUnitPool[proc->UnitPoolIndex].items[4] & 0xFF)->nameTextId, ItemName);
	PrintDebugStringAsOBJ(104, 72, ItemName);

	//Prints the stats
	PrintDebugNumberHex(10, 40, proc->StatsPage2[0], 3);
	PrintDebugNumberHex(10, 48, proc->StatsPage2[1], 3);
	PrintDebugNumberHex(10, 56, proc->StatsPage2[2], 3);
	PrintDebugNumberHex(10, 64, proc->StatsPage2[3], 3);
	PrintDebugNumberHex(10, 72, proc->StatsPage2[4], 3);
	
	//Draw cursor according to location table down there
	DisplayHandCursor(CursorLocationTable[proc->CursorIndex].x,CursorLocationTable[proc->CursorIndex].y);
}

const ProcInstruction Debug6C[] = {
	PROC_SET_NAME("Kirb:DebugUnitEditor"),

	PROC_CALL_ROUTINE(LockGameLogic),

	PROC_CALL_ROUTINE_ARG(StartFadeInBlack, 0x10),
	PROC_YIELD,

	PROC_CALL_ROUTINE(BlockGameGraphicsLogic),

//	PROC_CALL_ROUTINE(ClearOAM),
//	PROC_WHILE_ROUTINE(ClearPalette),

	PROC_CALL_ROUTINE(DebugScreenSetup),
	PROC_NEW_CHILD(gProc_BG3HSlide),

	PROC_CALL_ROUTINE_ARG(NewFadeOutBlack, 0x10),
	PROC_YIELD,

	//Main Logic
	PROC_LOOP_ROUTINE(DebugScreenLoop),

	//After Getting Out Of Main Updater

//	PROC_CALL_ROUTINE_2(FixBG3Cam), //Replacing the BG3 Offset where it needs to be
//	PROC_CALL_ROUTINE(LoadMapSprites),
//	PROC_CALL_ROUTINE(LoadMap),

	PROC_CALL_ROUTINE_ARG(StartFadeInBlack, 0x10),
	PROC_YIELD,

	PROC_CALL_ROUTINE(EndBG3Slider),

	PROC_CALL_ROUTINE(ReloadGameCoreGraphics),
	PROC_CALL_ROUTINE(UnblockGameGraphicsLogic),

	PROC_CALL_ROUTINE_ARG(NewFadeOutBlack, 0x10),
	PROC_YIELD,

	PROC_CALL_ROUTINE(UnlockGameLogic),

	PROC_END
};	

const LocationTable CursorLocationTable[] = {
	{ 10,  40-4  },
	{ 10,  48-4  },
	{ 10,  56-4  },
	{ 10,  64-4  },
	{ 10,  72-4  },
	{ 10,  80-4  },
	{ 10,  88-4  },
	{ 10,  96-4  },
	{ 10,  104-4 },
	{ 10,  112-4 },
	{ 10,  120-4 },
	{ 112, 120-4 },
	{ 10,  128-4 },
};

const u16 ButtonCombo[] = {
	DPAD_UP,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	DPAD_LEFT,
	DPAD_RIGHT,
	A_BUTTON,
	B_BUTTON,
	START_BUTTON,
};
