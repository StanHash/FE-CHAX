
#include "gbafe.h"

extern const unsigned gUsHiredFlag;
extern const unsigned gCaMercenary;

extern const u16 gPal_LmCoinBit[];
extern const u8  gImg_LmCoinBit[];

extern int GetUnitDeployCost(struct Unit* unit);

// TODO: move to CLib

int IsCharacterForceDeployed(int charId);
int GetPartyGoldAmount(void);
void AddPartyGoldAmount(int amt);
void WriteUIWindowTileMap(u16* bg, int x, int y, int width, int height, int tileBase, int style);
void HidePrepScreenHandCursor(void);
int IsUnitInCurrentRoster(struct Unit* unit);

extern const struct ProcInstruction gProc_PrepScreenHandCursor[];

static int LM_GetUnitDeploymentCost(struct Unit* unit)
{
	if (!UNIT_IS_VALID(unit))
		return 0;

	if (IsCharacterForceDeployed(unit->pCharacterData->number))
		return 0;

	return GetUnitDeployCost(unit);
}

static int LM_IsNotHired(struct Unit* unit)
{
	return (UNIT_CATTRIBUTES(unit) & gCaMercenary) && !(unit->state & gUsHiredFlag) && !IsCharacterForceDeployed(unit->pCharacterData->number);
}

struct PrepScreenUnitSelectProc
{
	PROC_HEADER;

	/* 29 */ u8 unk29;
	/* 2A */ u8 unk2A;
	/* 2B */ /* pad */
	/* 2C */ u16 pad2C;
	/* 2E */ u16 unk2E;
};

struct HirePromptProc
{
	PROC_HEADER;

	/* 2C */ struct Unit* pUnit;
};

static void HirePrompt_OnInit(struct HirePromptProc* proc);
static void HirePrompt_OnLoop(struct HirePromptProc* proc);
static void HirePrompt_OnEnd(struct HirePromptProc* proc);

static const struct ProcInstruction sProc_HirePrompt[] = {
	PROC_SET_NAME("Stan:LM:HirePrompt"),
	PROC_SLEEP(1),

	PROC_SET_DESTRUCTOR(HirePrompt_OnEnd),
	PROC_CALL_ROUTINE(HirePrompt_OnInit),
	PROC_LOOP_ROUTINE(HirePrompt_OnLoop),

	PROC_END
};

static void ShowPrepScreenHandCursor(void)
{
	struct Proc* proc = ProcFind(gProc_PrepScreenHandCursor);

	if (proc)
		ProcGoto(proc, 1);
}

void HirePrompt_OnInit(struct HirePromptProc* proc)
{
	gLCDIOBuffer.bgControl[0].priority = 0;

	HidePrepScreenHandCursor();

	WriteUIWindowTileMap(gBg0MapBuffer, 15, 5, 13, 8, 0, 0);
	EnableBgSyncByMask(1);

	struct FontData* savedFont = gpCurrentFont;

	struct FontData font;
	Text_InitFontExt(&font, VRAM + 0x5000, 0x0280, 3);

	gPaletteBuffer[0x10 * 3 + 0xF] = gPaletteBuffer[0x10 * 1 + 0xA];

	gpCurrentFont->drawGlyph = DrawGlyph1DTileNoClear;

	struct TextHandle text;

	// Line 1: "Hire <Name>?"

	Text_InitClear(&text, 11);
	CpuFastFill(0xFFFFFFFF, gpCurrentFont->getDrawTarget(&text), 0x40 * text.tileWidth);

	Text_SetXCursor(&text, 4);
	Text_DrawString(&text, "Hire ");

	Text_SetColorId(&text, TEXT_COLOR_BLUE);
	Text_DrawString(&text, GetStringFromIndex(proc->pUnit->pCharacterData->nameTextId));

	Text_SetColorId(&text, TEXT_COLOR_NORMAL);
	Text_DrawString(&text, "?");

	Text_Display(&text, BG_LOCATED_TILE(gBg0MapBuffer, 16, 6));

	// Line 2: "Fee: <Cost>G"

	Text_InitClear(&text, 11);
	CpuFastFill(0xFFFFFFFF, gpCurrentFont->getDrawTarget(&text), 0x40 * text.tileWidth);

	Text_SetXCursor(&text, 4);
	Text_DrawString(&text, "Fee: ");

	Text_SetXCursor(&text, 68);
	Text_DrawNumber(&text, LM_GetUnitDeploymentCost(proc->pUnit));

	const void* old = gpCurrentFont->pGlyphData;
	gpCurrentFont->pGlyphData = (const void*) (0x08590B44);

	Text_SetXCursor(&text, 76);
	Text_SetColorId(&text, TEXT_COLOR_GOLD);
	Text_DrawString(&text, "\x1E");

	gpCurrentFont->pGlyphData = old;

	Text_Display(&text, BG_LOCATED_TILE(gBg0MapBuffer, 16, 8));

	// Line 3: Yes/No

	Text_InitClear(&text, 11);
	CpuFastFill(0xFFFFFFFF, gpCurrentFont->getDrawTarget(&text), 0x40 * text.tileWidth);

	Text_SetXCursor(&text, 12);
	Text_SetColorId(&text, TEXT_COLOR_BLUE);
	Text_DrawString(&text, "A: Yes");

	Text_SetXCursor(&text, 54);
	Text_SetColorId(&text, TEXT_COLOR_BLUE);
	Text_DrawString(&text, "B: No");

	Text_Display(&text, BG_LOCATED_TILE(gBg0MapBuffer, 16, 10));

	gpCurrentFont = savedFont;
}

void HirePrompt_OnLoop(struct HirePromptProc* proc)
{
	if (gKeyState.pressedKeys & KEY_BUTTON_A)
	{
		AddPartyGoldAmount(-LM_GetUnitDeploymentCost(proc->pUnit));
		proc->pUnit->state |= gUsHiredFlag;

		PlaySfx(0xB9); // Backing out sound
		BreakProcLoop((struct Proc*)(proc));
	}

	if (gKeyState.pressedKeys & KEY_BUTTON_B)
	{
		PlaySfx(0x6B); // Backing out sound
		BreakProcLoop((struct Proc*)(proc));
	}
}

void HirePrompt_OnEnd(struct HirePromptProc* proc)
{
	BgMapFillRect(BG_LOCATED_TILE(gBg0MapBuffer, 15, 5), 12, 7, 0);
	EnableBgSyncByMask(1);

	ShowPrepScreenHandCursor();
	((void(*)(struct Proc*, int))(0x809A580+1))(proc->parent, ((struct PrepScreenUnitSelectProc*)(proc->parent))->unk2E>>1);
}

// Returns TRUE if the unit cannot be deployed yet. May block the proc
int LM_PrepScreenHireCheck(struct Unit* unit, struct PrepScreenUnitSelectProc* proc)
{
	if (proc->unk29 == proc->unk2A)
		return FALSE; // we let the game handle that one

	if (unit->state & US_BIT25)
		return FALSE; // we let the game handle that one

	if (!(unit->state & US_NOT_DEPLOYED))
		return FALSE; // already deployed, we good

	if (unit->state & gUsHiredFlag)
		return FALSE; // already hired, we good

	if (!(UNIT_CATTRIBUTES(unit) & gCaMercenary))
		return FALSE; // not a mercenary, we food

	if (GetPartyGoldAmount() >= LM_GetUnitDeploymentCost(unit))
	{
		struct HirePromptProc* newProc = (struct HirePromptProc*) ProcStartBlocking(sProc_HirePrompt, (struct Proc*) proc);

		newProc->pUnit = unit;

		PlaySfx(0x6A); // Good sound effect
		return TRUE;
	}

	PlaySfx(0x6C); // Bad sound effect
	return TRUE;
}

void LM_OnPrepScreenEnd(void)
{
	// Refund units that were hired but not deployed

	for (unsigned i = 1; i < 0x40; ++i)
	{
		struct Unit* unit = GetUnit(i);

		if (!UNIT_IS_VALID(unit))
			continue;

		if ((unit->state & gUsHiredFlag) && (unit->state & US_NOT_DEPLOYED))
			AddPartyGoldAmount(LM_GetUnitDeploymentCost(unit));
	}
}

void LM_ReplaceInitialDeployUnits(void)
{
	for (unsigned i = 1; i < 0x40; ++i)
	{
		struct Unit* unit = GetUnit(i);

		if (!UNIT_IS_VALID(unit))
			continue;

		if (!IsUnitInCurrentRoster(unit))
			continue;

		if (IsCharacterForceDeployed(unit->pCharacterData->number))
			unit->state = unit->state &~ US_NOT_DEPLOYED;
		else
			unit->state = unit->state | US_NOT_DEPLOYED;
	}
}

void LM_LoadCoinGfx(void)
{
	CopyToPaletteBuffer(gPal_LmCoinBit, 0x20 * 0x13, 0x20);
	RegisterTileGraphics(gImg_LmCoinBit, VRAM_OBJ + 0x20 * 52, 0x20);
}

void LM_PrepScreenUnitSprDraw(int x, int y, struct Unit* unit)
{
	int tim = GetGameClock() % 64;

	if (tim < 32 && LM_IsNotHired(unit))
	{
		ObjInsert(0, x + 8, y + 8, &gObj_8x8, 0x3800 + 52);
	}
}

/*

void LM_PrepScreenUnitNameDraw(struct TextHandle* text, struct Unit* unit, int color, u16* mapOut)
{
	Text_SetColorId(text, color);
	Text_DrawString(text, GetStringFromIndex(unit->pCharacterData->nameTextId));

	if (LM_IsNotHired(unit))
	{
		Text_SetXCursor(text, text->tileWidth*8-3);
		Text_SetColorId(text, TEXT_COLOR_GOLD);
		Text_DrawString(text, "`");
	}

	Text_Display(text, mapOut);
}
*/

/*

static int LMGetTotalDeploymentCost(void)
{
	int result = 0;

	for (int i = 1; i < 0x40; ++i)
	{
		struct Unit* unit = GetUnit(i);
		result += LM_GetUnitDeploymentCost(unit);
	}

	return result;
}

void LMReplace_809AAF0(struct PrepScreenUnitSelectProc* proc, int boolUnk)
{
	static struct TextHandle* const text = ((struct TextHandle*) (0x02013630)) + 2;

	if (!boolUnk)
	{
		Text_Clear(text);

		Text_SetXCursor(text, 6);
		Text_DrawString(text, "Pick");

		Text_SetXCursor(text, 56);
		Text_DrawString(text, "Cost");

		Text_Display(text, BG_LOCATED_TILE(gBg0MapBuffer, 13, 1));
	}

	BgMapFillRect(
		BG_LOCATED_TILE(gBg0MapBuffer, 17, 1), 1, 1, 0);

	DrawUiNumberOrDoubleDashes(
		BG_LOCATED_TILE(gBg0MapBuffer, 18, 1),
		(proc->unk2A == proc->unk29) ? TEXT_COLOR_GRAY : TEXT_COLOR_GREEN,
		(proc->unk2A - proc->unk29));

	BgMapFillRect(
		BG_LOCATED_TILE(gBg0MapBuffer, 23, 1), 5, 1, 0);

	DrawUiNumber(
		BG_LOCATED_TILE(gBg0MapBuffer, 27, 1),
		TEXT_COLOR_BLUE,
		LMGetTotalDeploymentCost()
	);

	DrawSpecialUiChar(
		BG_LOCATED_TILE(gBg0MapBuffer, 28, 1),
		TEXT_COLOR_GOLD, 0x1E);

	EnableBgSyncByMask(1);
}
// */
