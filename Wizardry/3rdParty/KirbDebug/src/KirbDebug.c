#include "Main.h"
#include "lib.h"

extern u32 DonateToKirbBGFX[];
extern u32 DonateToKirbBGPal[];

u8 EndEffect() {
	New6C((u32 *)Debug6C, 3);
	return KillMenu|PlayBeep|ClearMenuGfx;
}

u8 WaitUpdate() {
	return FALSE;
}

void DebugScreenSetup(UnitEditorProc *CurrentProc) {
	//Make it so the bg buffers update
	SetupBG(0);
	
	//Load the background graphics and palette and tsa(generate it)
	LZ77UncompVram((u32)DonateToKirbBGFX, 0x6008000);
	MemCopy((u32)DonateToKirbBGPal, (0x020228A8 + 14 * 0x20), 0x20);
	GenerateBGTsa((u16 *)BG3Buffer, 0x280, 14);
	
	MemCopy(0x859ED70, (0x020228A8 + 16 * 0x20), 0x20);
	//Load both debug fone(obj one will be used later on)
	LoadDebugFont(2, 0);
	LoadeOBJDebugFont(-1, 14);
	InitDefaultFont();
	InitText(0, 0);
	
	//Print a welcome message
	DebugPrint(BGLoc(BG0Buffer, 0, 1), "Kirb's debug-o-matic");
	DebugPrint(BGLoc(BG0Buffer, 0, 2), "Press Select to switch pages.");
	
	//Prints stats
	PrintConstantsPage1();
	
	//Setup the unit editor™
	CurrentProc->PageIndex = 0;
	CurrentProc->CursorIndex = 0;
	CurrentProc->UnitPoolIndex = 0;
	CurrentProc->CurrentUnitPool = 0;
	SetupDebugUnitEditorPage1(CurrentProc, sPlayerUnits);
}

void DebugScreenLoop(UnitEditorProc *CurrentProc) {
	UpdateBG3HOffset();
	if ((sInput->currentPress & InputB) != 0 && CurrentProc->KonamiCodeCounter != 10) {
		Clear6CLoop((Proc *)CurrentProc);
		PlaySound(0x6B); 
	}
	
	if ((sInput->newPress & InputStart) != 0) {
		CurrentProc->UnitPoolIndex = 0;
		if (CurrentProc->CurrentUnitPool == 0) {
			CurrentProc->CurrentUnitPool = 1;
			SetupDebugUnitEditorPage1(CurrentProc, sEnemyUnits);
			SetupDebugUnitEditorPage2(CurrentProc, sEnemyUnits);
		}
		else {
			CurrentProc->CurrentUnitPool = 0;
			SetupDebugUnitEditorPage1(CurrentProc, sPlayerUnits);
			SetupDebugUnitEditorPage2(CurrentProc, sPlayerUnits);
		}

	}	
	
	if ((sInput->newPress & InputSelect) != 0) {
		if (CurrentProc->PageIndex == 0) {
			CurrentProc->CursorIndex = 0;
			CurrentProc->PageIndex = 1;
			if (CurrentProc->CurrentUnitPool == 0) SetupDebugUnitEditorPage2(CurrentProc, sPlayerUnits);
			else SetupDebugUnitEditorPage2(CurrentProc, sEnemyUnits);
			PrintConstantsPage2();
		}
		else {
			CurrentProc->CursorIndex = 0;
			CurrentProc->PageIndex = 0;
			if (CurrentProc->CurrentUnitPool == 0) SetupDebugUnitEditorPage1(CurrentProc, sPlayerUnits);
			else SetupDebugUnitEditorPage1(CurrentProc, sEnemyUnits);
			PrintConstantsPage1();
		}
	}
	
	else {
	if (CurrentProc->PageIndex == 0) {
		
	if (CurrentProc->CurrentUnitPool == 0) UpdateDebugUnitEditorPage1(CurrentProc, sPlayerUnits);
	else UpdateDebugUnitEditorPage1(CurrentProc, sEnemyUnits);
	}
	else {
	if (CurrentProc->CurrentUnitPool == 0) UpdateDebugUnitEditorPage2(CurrentProc, sPlayerUnits);
	else UpdateDebugUnitEditorPage2(CurrentProc, sEnemyUnits);
	}
	CheckKonamiCode(CurrentProc);
	}
}



void SetupDebugUnitEditorPage1(UnitEditorProc *CurrentProc, UnitStruct *CurrentUnitPool) {
	CurrentProc->StatsPage1[0] = CurrentUnitPool[CurrentProc->UnitPoolIndex].pow;
	CurrentProc->StatsPage1[1] = CurrentUnitPool[CurrentProc->UnitPoolIndex].skl;
	CurrentProc->StatsPage1[2] = CurrentUnitPool[CurrentProc->UnitPoolIndex].spd;
	CurrentProc->StatsPage1[3] = CurrentUnitPool[CurrentProc->UnitPoolIndex].lck;
	CurrentProc->StatsPage1[4] = CurrentUnitPool[CurrentProc->UnitPoolIndex].def;
	CurrentProc->StatsPage1[5] = CurrentUnitPool[CurrentProc->UnitPoolIndex].res;
	CurrentProc->StatsPage1[6] = CurrentUnitPool[CurrentProc->UnitPoolIndex].maxHP;
	CurrentProc->StatsPage1[7] = CurrentUnitPool[CurrentProc->UnitPoolIndex].curHP;
	CurrentProc->StatsPage1[8] = CurrentUnitPool[CurrentProc->UnitPoolIndex].conBonus;
	CurrentProc->StatsPage1[9] = CurrentUnitPool[CurrentProc->UnitPoolIndex].movBonus;
	CurrentProc->StatsPage1[10] = CurrentUnitPool[CurrentProc->UnitPoolIndex].status;
	CurrentProc->StatsPage1[11] = CurrentUnitPool[CurrentProc->UnitPoolIndex].statusDuration;
	CurrentProc->StatsPage1[12] = CurrentUnitPool[CurrentProc->UnitPoolIndex].classDataPtr->number;
}

void UpdateDebugUnitEditorPage1(UnitEditorProc *CurrentProc, UnitStruct *CurrentUnitPool) {
	//Check for R
	if ((sInput->newPress & InputR) != 0) {
		if (CurrentUnitPool[CurrentProc->UnitPoolIndex + 1].unitDataPtr  != 0) {
		CurrentProc->UnitPoolIndex++;
		SetupDebugUnitEditorPage1(CurrentProc, CurrentUnitPool);
		}
	}
	
	//Check for L
	if ((sInput->newPress & InputL) != 0) {
		if (CurrentUnitPool[CurrentProc->UnitPoolIndex - 1].unitDataPtr  != 0) {
		CurrentProc->UnitPoolIndex--;
		SetupDebugUnitEditorPage1(CurrentProc, CurrentUnitPool);
		}
	}	
	
	//Check for down
	if ((sInput->newPress & InputDown) != 0) {
		if (CurrentProc->CursorIndex < UnitEditor_PAGE1ENTRIES) {
			CurrentProc->CursorIndex++;
			RemoveHighlight(1,0,2,(CursorLocationTable[CurrentProc->CursorIndex - 1].y / 8), 23);
			DrawHighlight(1,0,2,(CursorLocationTable[CurrentProc->CursorIndex].y / 8), 23);
		}
		else {
			CurrentProc->CursorIndex = 0;
			RemoveHighlight(1,0,2,(CursorLocationTable[UnitEditor_PAGE1ENTRIES].y / 8), 23);
			DrawHighlight(1,0,2,(CursorLocationTable[CurrentProc->CursorIndex].y / 8), 23);
		}
	}	
	
	//Check for up
	if ((sInput->newPress & InputUp) != 0) {
		if (CurrentProc->CursorIndex != 0) {
			CurrentProc->CursorIndex--;
			RemoveHighlight(1,0,2,(CursorLocationTable[CurrentProc->CursorIndex + 1].y / 8), 23);
			DrawHighlight(1,0,2,(CursorLocationTable[CurrentProc->CursorIndex].y / 8), 23);
		}
		else {
			CurrentProc->CursorIndex = UnitEditor_PAGE1ENTRIES;
			RemoveHighlight(1,0,2,(CursorLocationTable[0].y / 8), 23);
			DrawHighlight(1,0,2,(CursorLocationTable[CurrentProc->CursorIndex].y / 8), 23);
		}
	}	
	
	//Check for right
	if ((sInput->newPress & InputRight) != 0) {
		CurrentProc->StatsPage1[CurrentProc->CursorIndex]++;
	}	
	
	//Check for left
	if ((sInput->newPress & InputLeft) != 0) {
		CurrentProc->StatsPage1[CurrentProc->CursorIndex]--;
	}	

	//Check for a
	if ((sInput->newPress & InputA) != 0) {
		CurrentUnitPool[CurrentProc->UnitPoolIndex].pow = CurrentProc->StatsPage1[0];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].skl = CurrentProc->StatsPage1[1];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].spd = CurrentProc->StatsPage1[2];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].lck = CurrentProc->StatsPage1[3];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].def = CurrentProc->StatsPage1[4];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].res = CurrentProc->StatsPage1[5];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].maxHP = CurrentProc->StatsPage1[6];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].curHP = CurrentProc->StatsPage1[7];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].conBonus = CurrentProc->StatsPage1[8];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].movBonus = CurrentProc->StatsPage1[9];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].status = CurrentProc->StatsPage1[10];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].statusDuration = CurrentProc->StatsPage1[11];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].classDataPtr = (ClassData *)GetClassOffset(CurrentProc->StatsPage1[12]);
	}	

	//Get the unit index/name and print it
	char UnitName[0x20];
	UncompTID(CurrentUnitPool[CurrentProc->UnitPoolIndex].unitDataPtr->NameTID, UnitName);
	OBJPrintDec(10, 25, CurrentProc->UnitPoolIndex, 3);
	OBJPrintString(48, 25, UnitName);
	
	//Get the class name and prints it
	char ClassName[0x20];
	UncompTID(CurrentUnitPool[CurrentProc->UnitPoolIndex].classDataPtr->nameTextId, ClassName);
	OBJPrintString(48, 128, ClassName);
	
	//Prints the stats
	OBJPrintDec(10, 40, CurrentProc->StatsPage1[0], 3);
	OBJPrintDec(10, 48, CurrentProc->StatsPage1[1], 3);
	OBJPrintDec(10, 56, CurrentProc->StatsPage1[2], 3);
	OBJPrintDec(10, 64, CurrentProc->StatsPage1[3], 3);
	OBJPrintDec(10, 72, CurrentProc->StatsPage1[4], 3);
	OBJPrintDec(10, 80, CurrentProc->StatsPage1[5], 3);
	OBJPrintDec(10, 88, CurrentProc->StatsPage1[6], 3);
	OBJPrintDec(10, 96, CurrentProc->StatsPage1[7], 3);
	OBJPrintDec(10, 104, CurrentProc->StatsPage1[8], 3);
	OBJPrintDec(10, 112, CurrentProc->StatsPage1[9], 3);
	OBJPrintDec(10, 120, CurrentProc->StatsPage1[10], 3);
	OBJPrintDec(112, 120, CurrentProc->StatsPage1[11], 2);
	OBJPrintHex(10, 128, CurrentProc->StatsPage1[12], 3);
	DrawCursor(CursorLocationTable[CurrentProc->CursorIndex].x,CursorLocationTable[CurrentProc->CursorIndex].y);
}

void GenerateBGTsa(u16 *MapOffset, u32 NumberOfTiles, u8 PaletteId) {
	for(int i = 0; i < NumberOfTiles; i++) {
		MapOffset[i] = (i | (PaletteId << 12));
	}
}

void CheckKonamiCode(UnitEditorProc *CurrentProc) {
	if ((sInput->newPress & ButtonCombo[CurrentProc->KonamiCodeCounter]) != 0) {
		CurrentProc->KonamiCodeCounter++;
	}
	if (CurrentProc->KonamiCodeCounter == 11) {
	DebugPrint(BGLoc(BG0Buffer, 10, 18), "Konami Code");
	}
}

void PrintConstantsPage1() {
	InitDefaultFont();
	FillBGMap(BG0Buffer, 0);
	FillBGMap(BG1Buffer, 0);
	MakeUIWindow(0,2,26,16,0);
	MakeUIPalette(-1);
	PrintInline(0, BGLoc(BG0Buffer, 0, 0), 0, 0, 12, "Kirb's debug-o-matic.");
	PrintInline(0, BGLoc(BG0Buffer, 13, 0), 3, 0, 17, "Press Select to switch pages.");
	PrintInline(0, BGLoc(BG0Buffer, 0, 18), 4, 0, 9, "Donate To Kirb.");
	PrintInline(0, BGLoc(BG0Buffer, 9, 18), 4, 0, 20, "https://www.patreon.com/Kirb1337");
	DebugPrint(BGLoc(BG0Buffer, 6, 5), "Power");
	DebugPrint(BGLoc(BG0Buffer, 6, 6), "Skill");
	DebugPrint(BGLoc(BG0Buffer, 6, 7), "Speed");
	DebugPrint(BGLoc(BG0Buffer, 6, 8), "Luck");
	DebugPrint(BGLoc(BG0Buffer, 6, 9), "Defense");
	DebugPrint(BGLoc(BG0Buffer, 6, 10), "Resistance");
	DebugPrint(BGLoc(BG0Buffer, 6, 11), "Max HP");
	DebugPrint(BGLoc(BG0Buffer, 6, 12), "Current HP");
	DebugPrint(BGLoc(BG0Buffer, 6, 13), "Constitution");
	DebugPrint(BGLoc(BG0Buffer, 6, 14), "Move");
	DebugPrint(BGLoc(BG0Buffer, 6, 15), "Status");
	DebugPrint(BGLoc(BG0Buffer, 17, 15), "Duration");
}

void PrintConstantsPage2() {
	InitDefaultFont();
	FillBGMap(BG0Buffer, 0);
	FillBGMap(BG1Buffer, 0);
	MakeUIWindow(0,2,29,9,3);
	MakeUIPalette(-1);
	PrintInline(0, BGLoc(BG0Buffer, 0, 0), 0, 0, 12, "Kirb's debug-o-matic.");
	PrintInline(0, BGLoc(BG0Buffer, 13, 0), 3, 0, 16, "Press Select to switch pages.");
	PrintInline(0, BGLoc(BG0Buffer, 0, 18), 4, 0, 9, "Donate To Kirb.");
	PrintInline(0, BGLoc(BG0Buffer, 9, 18), 4, 0, 20, "https://www.patreon.com/Kirb1337");
	DebugPrint(BGLoc(BG0Buffer, 6, 5), "Item 1:");
	DebugPrint(BGLoc(BG0Buffer, 6, 6), "Item 2:");
	DebugPrint(BGLoc(BG0Buffer, 6, 7), "Item 3:");
	DebugPrint(BGLoc(BG0Buffer, 6, 8), "Item 4:");
	DebugPrint(BGLoc(BG0Buffer, 6, 9), "Item 5:");
}

void SetupDebugUnitEditorPage2(UnitEditorProc *CurrentProc, UnitStruct *CurrentUnitPool) {
	CurrentProc->StatsPage2[0] = CurrentUnitPool[CurrentProc->UnitPoolIndex].items[0] & 0xFF;
	CurrentProc->StatsPage2[1] = CurrentUnitPool[CurrentProc->UnitPoolIndex].items[1] & 0xFF;
	CurrentProc->StatsPage2[2] = CurrentUnitPool[CurrentProc->UnitPoolIndex].items[2] & 0xFF;
	CurrentProc->StatsPage2[3] = CurrentUnitPool[CurrentProc->UnitPoolIndex].items[3] & 0xFF;
	CurrentProc->StatsPage2[4] = CurrentUnitPool[CurrentProc->UnitPoolIndex].items[4] & 0xFF;
}

void UpdateDebugUnitEditorPage2(UnitEditorProc *CurrentProc, UnitStruct *CurrentUnitPool) {
	//Check for R
	if ((sInput->newPress & InputR) != 0) {
		if (CurrentUnitPool[CurrentProc->UnitPoolIndex + 1].unitDataPtr  != 0) {
		CurrentProc->UnitPoolIndex++;
		SetupDebugUnitEditorPage2(CurrentProc, CurrentUnitPool);
		}
	}
	
	//Check for L
	if ((sInput->newPress & InputL) != 0) {
		if (CurrentUnitPool[CurrentProc->UnitPoolIndex - 1].unitDataPtr  != 0) {
		CurrentProc->UnitPoolIndex--;
		SetupDebugUnitEditorPage2(CurrentProc, CurrentUnitPool);
		}
	}	
	
	//Check for down
	if ((sInput->newPress & InputDown) != 0) {
		if (CurrentProc->CursorIndex < UnitEditor_PAGE2ENTRIES) {
			CurrentProc->CursorIndex++;
			RemoveHighlight(1,0,((CursorLocationTable[CurrentProc->CursorIndex - 1].x + 6) / 8),(CursorLocationTable[CurrentProc->CursorIndex - 1].y / 8), 26);
			DrawHighlight(1,0,((CursorLocationTable[CurrentProc->CursorIndex].x + 6) / 8),(CursorLocationTable[CurrentProc->CursorIndex].y / 8), 26);
		}
		else {
			CurrentProc->CursorIndex = 0;
			RemoveHighlight(1,0,((CursorLocationTable[UnitEditor_PAGE2ENTRIES].x + 6) / 8),(CursorLocationTable[UnitEditor_PAGE2ENTRIES].y / 8), 26);
			DrawHighlight(1,0,((CursorLocationTable[CurrentProc->CursorIndex].x + 6) / 8),(CursorLocationTable[CurrentProc->CursorIndex].y / 8), 26);
		}
	}	
	
	//Check for up
	if ((sInput->newPress & InputUp) != 0) {
		if (CurrentProc->CursorIndex != 0) {
			CurrentProc->CursorIndex--;
			RemoveHighlight(1,0,((CursorLocationTable[CurrentProc->CursorIndex + 1].x + 6) / 8),(CursorLocationTable[CurrentProc->CursorIndex + 1].y / 8), 26);
			DrawHighlight(1,0,((CursorLocationTable[CurrentProc->CursorIndex].x + 6) / 8),(CursorLocationTable[CurrentProc->CursorIndex].y / 8), 26);
		}
		else {
			CurrentProc->CursorIndex = UnitEditor_PAGE2ENTRIES;
			RemoveHighlight(1,0,((CursorLocationTable[0].x + 6) / 8),(CursorLocationTable[0].y / 8), 26);
			DrawHighlight(1,0,((CursorLocationTable[CurrentProc->CursorIndex].x + 6) / 8),(CursorLocationTable[CurrentProc->CursorIndex].y / 8), 26);
		}
	}	
	
	//Check for right
	if ((sInput->newPress & InputRight) != 0) {
		CurrentProc->StatsPage2[CurrentProc->CursorIndex]++;
	}	
	
	//Check for left
	if ((sInput->newPress & InputLeft) != 0) {
		CurrentProc->StatsPage2[CurrentProc->CursorIndex]--;
	}	

	//Check for a
	if ((sInput->newPress & InputA) != 0) {
		CurrentUnitPool[CurrentProc->UnitPoolIndex].items[0] = CurrentProc->StatsPage2[0];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].items[1] = CurrentProc->StatsPage2[1];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].items[2] = CurrentProc->StatsPage2[2];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].items[3] = CurrentProc->StatsPage2[3];
		CurrentUnitPool[CurrentProc->UnitPoolIndex].items[4] = CurrentProc->StatsPage2[4];
	}	
	
	//Get the unit index/name and print it
	char UnitName[0x20];
	UncompTID(CurrentUnitPool[CurrentProc->UnitPoolIndex].unitDataPtr->NameTID, UnitName);
	OBJPrintDec(10, 25, CurrentProc->UnitPoolIndex, 3);
	OBJPrintString(48, 25, UnitName);
	
	//Get Items names
	char ItemName[0x20];
	UncompTID(((ItemData *)GetItemOffset(CurrentUnitPool[CurrentProc->UnitPoolIndex].items[0] & 0xFF))->NameTID, ItemName);
	OBJPrintString(104, 40, ItemName);
	
	UncompTID(((ItemData *)GetItemOffset(CurrentUnitPool[CurrentProc->UnitPoolIndex].items[1] & 0xFF))->NameTID, ItemName);
	OBJPrintString(104, 48, ItemName);
	
	UncompTID(((ItemData *)GetItemOffset(CurrentUnitPool[CurrentProc->UnitPoolIndex].items[2] & 0xFF))->NameTID, ItemName);
	OBJPrintString(104, 56, ItemName);
	
	UncompTID(((ItemData *)GetItemOffset(CurrentUnitPool[CurrentProc->UnitPoolIndex].items[3] & 0xFF))->NameTID, ItemName);
	OBJPrintString(104, 64, ItemName);
	
	UncompTID(((ItemData *)GetItemOffset(CurrentUnitPool[CurrentProc->UnitPoolIndex].items[4] & 0xFF))->NameTID, ItemName);
	OBJPrintString(104, 72, ItemName);
	
	//Prints the stats
	OBJPrintHex(10, 40, CurrentProc->StatsPage2[0], 3);
	OBJPrintHex(10, 48, CurrentProc->StatsPage2[1], 3);
	OBJPrintHex(10, 56, CurrentProc->StatsPage2[2], 3);
	OBJPrintHex(10, 64, CurrentProc->StatsPage2[3], 3);
	OBJPrintHex(10, 72, CurrentProc->StatsPage2[4], 3);
	
	//Draw cursor according to location table down there
	DrawCursor(CursorLocationTable[CurrentProc->CursorIndex].x,CursorLocationTable[CurrentProc->CursorIndex].y);
}


const _ProcCode Debug6C[] = {
	_6C_SET_NAME("E_Debug"),
	
	//Setting Up
	_6C_CALL_ROUTINE(LockGameLogic),
	_6C_CALL_ROUTINE_ARG(FadeToBlack, 0x10),
	_6C_CALL_ROUTINE(ClearOAM),
	_6C_WHILE_ROUTINE(ClearPalette),
	_6C_CALL_ROUTINE_ARG(FadeOut, 0x10),
	_6C_CALL_ROUTINE(DebugScreenSetup),
	
	//Main Logic
	_6C_LOOP_ROUTINE(DebugScreenLoop),
	
	//After Getting Out Of Main Updater
	_6C_CALL_ROUTINE_2(FixBG3Cam), //Replacing the BG3 Offset where it needs to be
	_6C_CALL_ROUTINE(LoadMapSprites),
	_6C_CALL_ROUTINE(LoadMap),
	_6C_CALL_ROUTINE(UnlockGameLogic),
	_6C_END
	};	

const LocationTable CursorLocationTable[] = {
	{	10, 40-4	},
	{	10, 48-4	},
	{	10, 56-4	},
	{	10, 64-4	},
	{	10, 72-4	},
	{	10, 80-4	},
	{	10, 88-4	},
	{	10, 96-4	},
	{	10, 104-4	},
	{	10, 112-4	},
	{	10, 120-4	},
	{	112, 120-4	},
	{	10, 128-4	},
};

const u16 ButtonCombo[] = {
	InputUp,
	InputUp,
	InputDown,
	InputDown,
	InputLeft,
	InputRight,
	InputLeft,
	InputRight,
	InputA,
	InputB,
	InputStart
};
