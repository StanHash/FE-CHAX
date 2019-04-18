
#include "gbafe.h"

extern const unsigned gLwIconSheet;
extern const unsigned gPlusItemAttribute;

static int LW_IsLazberianWeapon(int item)
{
	static const u8 lookup[] = { TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE };

	int wt = GetItemType(item);
	return wt < 8 ? lookup[wt] : FALSE;
}

static int LW_GetItemUsesPercentage(int item)
{
	return Div(100 * GetItemUses(item), GetItemMaxUses(item));
}

static int LW_GetItemDisplayDurabilityIcon(int item)
{
	int perc = LW_GetItemUsesPercentage(item);

	if (perc > 61)
		return 0 | (gLwIconSheet << 8);

	if (perc > 41)
		return 1 | (gLwIconSheet << 8);

	if (perc > 21)
		return 2 | (gLwIconSheet << 8);

	if (perc > 1)
		return 3 | (gLwIconSheet << 8);

	return 4 | (gLwIconSheet << 8);
}

static int LW_GetItemBreakChance(int item)
{
	int perc = LW_GetItemUsesPercentage(item);

	if (perc > 61)
		return 0;

	if (perc > 41)
		return 1;

	if (perc > 21)
		return 2;

	if (perc > 1)
		return 4;

	return 100;
}

char* LW_GetItemPlusTimesString(int item, char buf[0x20])
{
	int i = 0;
	unsigned attributes = GetItemAttributes(item);

	if (attributes & IA_BRAVE)
	{
		buf[i++] = ' ';
		buf[i++] = 'x';
		buf[i++] = '2';
	}

	if (attributes & gPlusItemAttribute)
	{
		buf[i++] = ' ';
		buf[i++] = '+';
	}

	buf[i] = 0;

	return buf;
}

void LW_DrawDurabilityRaw(int item, int color, u16* mapOut)
{
	int uses = GetItemUses(item);

	if (uses != 0xFF && LW_IsLazberianWeapon(item))
	{
		DrawIcon(mapOut, LW_GetItemDisplayDurabilityIcon(item), 0x4000);
	}
	else
	{
		DrawUiNumberOrDoubleDashes(mapOut + 1, color, uses);
	}
}

//! Replaces 
void DrawItemMenuLine(struct TextHandle* text, int item, s8 isUsable, u16* mapOut)
{
	Text_SetParameters(text, 0, (isUsable ? TEXT_COLOR_NORMAL : TEXT_COLOR_GRAY));
	Text_DrawString(text, GetItemName(item));

	Text_Display(text, mapOut + 2);

	if (LW_IsLazberianWeapon(item))
	{
		char buf[0x20];
		LW_GetItemPlusTimesString(item, buf);

		if (buf[0])
		{
			Text_SetColorId(text, isUsable ? TEXT_COLOR_GOLD : TEXT_COLOR_GRAY);
			Text_DrawString(text, buf);
		}

		if (GetItemUses(item) == 0xFF)
			DrawUiNumberOrDoubleDashes(mapOut + 11, isUsable ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, 0xFF);
		else
			DrawIcon(mapOut + 10, LW_GetItemDisplayDurabilityIcon(item), 0x4000);
	}
	else
	{
		DrawUiNumberOrDoubleDashes(mapOut + 11, isUsable ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, GetItemUses(item));
	}

	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

//! Replaces 
void DrawItemMenuLineLong(struct TextHandle* text, int item, s8 isUsable, u16* mapOut)
{
	Text_SetParameters(text, 0, (isUsable ? TEXT_COLOR_NORMAL : TEXT_COLOR_GRAY));
	Text_DrawString(text, GetItemName(item));

	Text_Display(text, mapOut + 2);

	if (LW_IsLazberianWeapon(item))
	{
		char buf[0x20];
		LW_GetItemPlusTimesString(item, buf);

		if (buf[0])
		{
			Text_SetColorId(text, isUsable ? TEXT_COLOR_GOLD : TEXT_COLOR_GRAY);
			Text_DrawString(text, buf);
		}

		if (GetItemUses(item) == 0xFF)
			DrawUiNumberOrDoubleDashes(mapOut + 13, isUsable ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, 0xFF);
		else
			DrawIcon(mapOut + 12, LW_GetItemDisplayDurabilityIcon(item), 0x4000);
	}
	else
	{
		DrawUiNumberOrDoubleDashes(mapOut + 10, isUsable ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, GetItemUses(item));
		DrawUiNumberOrDoubleDashes(mapOut + 13, isUsable ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, GetItemMaxUses(item));
		DrawSpecialUiChar(mapOut + 11, isUsable ? TEXT_COLOR_NORMAL : TEXT_COLOR_GRAY, 0x16);
	}

	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

//! Replaces 
void DrawItemMenuLineNoColor(struct TextHandle* text, int item, u16* mapOut)
{
	Text_SetXCursor(text, 0);
	Text_DrawString(text, GetItemName(item));

	Text_Display(text, mapOut + 2);

	if (LW_IsLazberianWeapon(item))
	{
		char buf[0x20];
		LW_GetItemPlusTimesString(item, buf);

		if (buf[0])
		{
			Text_DrawString(text, buf);
		}

		if (GetItemUses(item) == 0xFF)
			DrawUiNumberOrDoubleDashes(mapOut + 11, Text_GetColorId(text), 0xFF);
		else
			DrawIcon(mapOut + 10, LW_GetItemDisplayDurabilityIcon(item), 0x4000);
	}
	else
	{
		DrawUiNumberOrDoubleDashes(mapOut + 11, Text_GetColorId(text), GetItemUses(item));
	}

	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

//! Replaces 
void DrawItemStatScreenLine(struct TextHandle* text, int item, int nameColor, u16* mapOut)
{
	int color;

	Text_Clear(text);

	color = nameColor;
	Text_SetColorId(text, color);

	Text_DrawString(text, GetItemName(item));

	if (LW_IsLazberianWeapon(item))
	{
		char buf[0x20];
		LW_GetItemPlusTimesString(item, buf);

		if (buf[0])
		{
			Text_SetColorId(text, (nameColor == TEXT_COLOR_GRAY) ? TEXT_COLOR_GRAY : TEXT_COLOR_GOLD);
			Text_DrawString(text, buf);
		}

		if (GetItemUses(item) == 0xFF)
			DrawUiNumberOrDoubleDashes(mapOut + 14, (nameColor != TEXT_COLOR_GRAY) ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, 0xFF);
		else
			DrawIcon(mapOut + 13, LW_GetItemDisplayDurabilityIcon(item), 0x4000);
	}
	else
	{
		color = (nameColor == TEXT_COLOR_GRAY) ? TEXT_COLOR_GRAY : TEXT_COLOR_NORMAL;
		DrawSpecialUiChar(mapOut + 12, color, 0x16);

		color = (nameColor != TEXT_COLOR_GRAY) ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY;
		DrawUiNumberOrDoubleDashes(mapOut + 11, color, GetItemUses(item));
		DrawUiNumberOrDoubleDashes(mapOut + 14, color, GetItemMaxUses(item));
	}

	Text_Display(text, mapOut + 2);

	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

//! Replaces 
u16 GetItemAfterUse(int item)
{
	if (GetItemAttributes(item) & IA_UNBREAKABLE)
		return item; // unbreakable items don't loose uses!

	item -= (1 << 8); // lose one use

	if (LW_IsLazberianWeapon(item))
	{
		if (LW_GetItemBreakChance(item) > NextRN_100())
			return 0;

		return item;
	}
	else
	{
		if (item < (1 << 8))
			return 0; // return no item if uses < 0

		return item; // return used item
	}
}
