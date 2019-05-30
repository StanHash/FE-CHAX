
#include "Vci.h"

struct VciHealEntry
{
	u16 itemId;
	u16 healAmt;
};

extern const struct VciHealEntry gVciItemHealAmountList[];

static int VciGetIndexVanilla(int item)
{
	return 4;
	return item & 0xFF;
}

static char* VciGetNameVanilla(int item)
{
	char* result;

	result = GetStringFromIndex(GetItemData(GetItemIndex(item))->nameTextId);
	result = String_ExpandTactName();

	return result;
}

static int VciGetDescIdVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->descTextId;
}

static int VciGetUseDescIdVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->useDescTextId;
}

static int VciGetTypeVanilla(int item)
{
	if (!item)
		return 0xFF;

	return GetItemData(GetItemIndex(item))->weaponType;
}

static int VciGetAttributesVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->attributes;
}

static int VciGetUsesVanilla(int item)
{
	if (GetItemAttributes(item) & IA_UNBREAKABLE)
		return 0xFF;
	else
		return item >> 8;
}

static int VciGetMaxUsesVanilla(int item)
{
	if (GetItemAttributes(item) & IA_UNBREAKABLE)
		return 0xFF;
	else
		return GetItemData(GetItemIndex(item))->maxUses;
}

static int VciGetMightVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->might;
}

static int VciGetHitVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->hit;
}

static int VciGetWeightVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->weight;
}

static int VciGetCritVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->crit;
}

static int VciGetCostVanilla(int item)
{
	if (GetItemAttributes(item) & IA_UNBREAKABLE)
		return GetItemCostPerUse(item);
	else
		return GetItemCostPerUse(item) * GetItemUses(item);
}

static int VciGetMinRangeVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->encodedRange >> 4;
}

static int VciGetMaxRangeVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->encodedRange & 0xF;
}

static int VciGetEncodedRangeVanilla(int item)
{
	return (GetItemMinRange(item) << 4) | (GetItemMaxRange(item) & 0xF);
}

static int VciGetRequiredExpVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->weaponRank;
}

static const u8* VciGetEffectivenessVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->pEffectiveness;
}

static const struct ItemStatBonuses* VciGetStatBonusesVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->pStatBonuses;
}

static int VciGetIconIdVanilla(int item)
{
	if (!item)
		return -1;

	return GetItemData(GetItemIndex(item))->iconId;
}

static int VciGetWeaponEffectVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->weaponEffectId;
}

static int VciGetUseEffectVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->useEffectId;
}

static int VciGetCostPerUseVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->costPerUse;
}

static int VciGetMaxCostVanilla(int item)
{
	return GetItemCostPerUse(item) * GetItemMaxUses(item);
}

static int VciGetAwardedExpVanilla(int item)
{
	return GetItemData(GetItemIndex(item))->weaponExp;
}

static int VciGetHpBonusVanilla(int item)
{
	if (item)
	{
		const struct ItemStatBonuses* statBonuses = GetItemStatBonuses(item);

		if (statBonuses)
			return statBonuses->hpBonus;
	}

	return 0;
}

static int VciGetPowBonusVanilla(int item)
{
	if (item)
	{
		const struct ItemStatBonuses* statBonuses = GetItemStatBonuses(item);

		if (statBonuses)
			return statBonuses->powBonus;
	}

	return 0;
}

static int VciGetSklBonusVanilla(int item)
{
	if (item)
	{
		const struct ItemStatBonuses* statBonuses = GetItemStatBonuses(item);

		if (statBonuses)
			return statBonuses->sklBonus;
	}

	return 0;
}

static int VciGetSpdBonusVanilla(int item)
{
	if (item)
	{
		const struct ItemStatBonuses* statBonuses = GetItemStatBonuses(item);

		if (statBonuses)
			return statBonuses->spdBonus;
	}

	return 0;
}

static int VciGetDefBonusVanilla(int item)
{
	if (item)
	{
		const struct ItemStatBonuses* statBonuses = GetItemStatBonuses(item);

		if (statBonuses)
			return statBonuses->defBonus;
	}

	return 0;
}

static int VciGetResBonusVanilla(int item)
{
	if (item)
	{
		const struct ItemStatBonuses* statBonuses = GetItemStatBonuses(item);

		if (statBonuses)
			return statBonuses->resBonus;
	}

	return 0;
}

static int VciGetLckBonusVanilla(int item)
{
	if (item)
	{
		const struct ItemStatBonuses* statBonuses = GetItemStatBonuses(item);

		if (statBonuses)
			return statBonuses->lckBonus;
	}

	return 0;
}

static int VciCloneVanilla(int item)
{
	int uses = GetItemMaxUses(item);

	if (GetItemAttributes(item) & IA_UNBREAKABLE)
		uses = 0;

	return (uses << 8) + GetItemIndex(item);
}

static int VciCanUnitUseWeaponVanilla(int item, struct Unit* unit)
{
	if (!item)
		return FALSE;

	unsigned attrs = GetItemAttributes(item);

	if (!(attrs & IA_WEAPON))
		return FALSE;

	if (attrs & IA_LOCK_ANY)
	{
		// Check for item locks

		if ((attrs & IA_LOCK_1) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_1))
			return FALSE;

		if ((attrs & IA_LOCK_4) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_4))
			return FALSE;

		if ((attrs & IA_LOCK_5) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_5))
			return FALSE;

		if ((attrs & IA_LOCK_6) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_6))
			return FALSE;

		if ((attrs & IA_LOCK_7) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_7))
			return FALSE;

		if ((attrs & IA_LOCK_2) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_2))
			return FALSE;

		// Monster lock is special
		if (attrs & IA_LOCK_3)
		{
			if (!(UNIT_CATTRIBUTES(unit) & CA_LOCK_3))
				return FALSE;

			return TRUE;
		}

		if (attrs & IA_UNUSABLE)
			if (!(IsItemUnsealedForUnit(unit, item)))
				return FALSE;
	}

	if ((unit->statusIndex == UNIT_STATUS_SILENCED) && (attrs & IA_MAGIC))
		return FALSE;

	int wRank = GetItemRequiredExp(item);
	int uRank = (unit->ranks[GetItemType(item)]);

	return (uRank >= wRank);
}

static int VciCanUnitUseStaffVanilla(int item, struct Unit* unit)
{
	if (!item)
		return FALSE;

	if (!(GetItemAttributes(item) & IA_STAFF))
		return FALSE;

	if (unit->statusIndex == UNIT_STATUS_SLEEP)
		return FALSE;

	if (unit->statusIndex == UNIT_STATUS_BERSERK)
		return FALSE;

	if (unit->statusIndex == UNIT_STATUS_SILENCED)
		return FALSE;

	int wRank = GetItemRequiredExp(item);
	int uRank = unit->ranks[GetItemType(item)];

	return (uRank >= wRank);
}

static void VciDrawMenuLineVanilla(int item, struct TextHandle* text, u16* mapOut, int isUsable)
{
	Text_SetParameters(text, 0, (isUsable ? TEXT_COLOR_NORMAL : TEXT_COLOR_GRAY));
	Text_DrawString(text, GetItemName(item));

	Text_Display(text, mapOut + 2);

	DrawUiNumberOrDoubleDashes(mapOut + 11, isUsable ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, GetItemUses(item));

	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

static void VciDrawMenuLineLongVanilla(int item, struct TextHandle* text, u16* mapOut, int isUsable)
{
	Text_SetParameters(text, 0, (isUsable ? TEXT_COLOR_NORMAL : TEXT_COLOR_GRAY));
	Text_DrawString(text, GetItemName(item));

	Text_Display(text, mapOut + 2);

	DrawUiNumberOrDoubleDashes(mapOut + 10, isUsable ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, GetItemUses(item));
	DrawUiNumberOrDoubleDashes(mapOut + 13, isUsable ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY, GetItemMaxUses(item));
	DrawSpecialUiChar(mapOut + 11, isUsable ? TEXT_COLOR_NORMAL : TEXT_COLOR_GRAY, 0x16); // draw special character?

	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

static void VciDrawMenuLineNoColorVanilla(int item, struct TextHandle* text, u16* mapOut)
{
	Text_SetXCursor(text, 0);
	Text_DrawString(text, GetItemName(item));

	Text_Display(text, mapOut + 2);

	DrawUiNumberOrDoubleDashes(mapOut + 11, Text_GetColorId(text), GetItemUses(item));

	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

static void VciDrawStatScreenLineVanilla(int item, struct TextHandle* text, u16* mapOut, int nameColor)
{
	int color;

	Text_Clear(text);

	color = nameColor;
	Text_SetColorId(text, color);

	Text_DrawString(text, GetItemName(item));

	color = (nameColor == TEXT_COLOR_GRAY) ? TEXT_COLOR_GRAY : TEXT_COLOR_NORMAL;
	DrawSpecialUiChar(mapOut + 12, color, 0x16);

	color = (nameColor != TEXT_COLOR_GRAY) ? TEXT_COLOR_BLUE : TEXT_COLOR_GRAY;
	DrawUiNumberOrDoubleDashes(mapOut + 11, color, GetItemUses(item));
	DrawUiNumberOrDoubleDashes(mapOut + 14, color, GetItemMaxUses(item));

	Text_Display(text, mapOut + 2);

	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

static int VciGetAfterUseVanilla(int item)
{
	if (GetItemAttributes(item) & IA_UNBREAKABLE)
		return item; // unbreakable items don't loose uses!

	item -= (1 << 8); // lose one use

	if (item < (1 << 8))
		return 0; // return no item if uses < 0

	return item; // return used item
}

static int VciIsEffectiveAgainstVanilla(int item, struct Unit* unit)
{
	if (unit->pClassData)
	{
		int classId = unit->pClassData->number;
		const u8* effList = GetItemEffectiveness(item);

		if (!effList)
			return FALSE;

		for (; *effList; ++effList)
			if (*effList == classId)
				// NOTE: maybe there's a better way to make this work (using an inline maybe?)
				goto check_flying_effectiveness_negation;

		return FALSE;

		check_flying_effectiveness_negation: { 
			u32 attributes;
			int i;

			if (GetItemEffectiveness(item) != (const void*)(0x088ADF2A))
				if (GetItemEffectiveness(item) != (const void*)(0x088ADEF1))
					return TRUE;

			attributes = 0;

			for (i = 0; i < UNIT_ITEM_COUNT; ++i)
				attributes = attributes | GetItemAttributes(unit->items[i]);

			if (!(attributes & IA_NEGATE_FLYING))
				return TRUE;
			else
				return FALSE;
		}
	}

	return FALSE;
}

static char* VciGetDisplayRangeStringVanilla(int item)
{
	static const short rangeTextIdLookup[10] = {
		0x522, 0x523, 0x524, 0x525, 0x526, // 0-Mag/2, 1, 1-2, 1-3, 2
		0x527, 0x528, 0x529, 0x52A, 0x52B, // 2-3, 3-10, 3-15, Total, --
	};

	unsigned range = GetItemEncodedRange(item);

	if (range == 0x10) // 1-mag/2
		return GetStringFromIndex(rangeTextIdLookup[0]);

	if (range == 0x11) // 1-1
		return GetStringFromIndex(rangeTextIdLookup[1]);

	if (range == 0x12) // 1-2
		return GetStringFromIndex(rangeTextIdLookup[2]);

	if (range == 0x13) // 1-3
		return GetStringFromIndex(rangeTextIdLookup[3]);

	if (range == 0x22) // 2-2
		return GetStringFromIndex(rangeTextIdLookup[4]);

	if (range == 0x23) // 2-3
		return GetStringFromIndex(rangeTextIdLookup[5]);

	if (range == 0x3A) // 3-10
		return GetStringFromIndex(rangeTextIdLookup[6]);

	if (range == 0x3F) // 3-15
		return GetStringFromIndex(rangeTextIdLookup[7]);

	if (range == 0xFF) // total
		return GetStringFromIndex(rangeTextIdLookup[8]);

	return GetStringFromIndex(rangeTextIdLookup[9]);
}

static char* VciGetDisplayRankStringVanilla(int item)
{
	static const short rankTextIdLookup[] = {
		0x52C, 0x52D, 0x52E, 0x52F, // --, E, D, C
		0x530, 0x531, 0x532, 0x533, // B, A, S, Prf
	};

	// reuse of the same variable for different purposes :/
	int var = GetItemRequiredExp(item);

	if ((GetItemAttributes(item) & IA_LOCK_ANY) && GetWeaponLevelFromExp(var) == WPN_LEVEL_0)
		var = 7;
	else
		var = GetWeaponLevelFromExp(var);

	return GetStringFromIndex(rankTextIdLookup[var]);
}

static int VciIsDisplayUsableVanilla(int item, struct Unit* unit)
{
	unsigned attrs = GetItemAttributes(item);

	if (attrs & IA_WEAPON)
		return CanUnitUseWeapon(unit, item);

	if (attrs & IA_STAFF)
		return CanUnitUseStaff(unit, item);

	if (GetItemUseEffect(item))
	{
		if (unit->statusIndex == UNIT_STATUS_SLEEP)
			return FALSE;

		if (unit->statusIndex == UNIT_STATUS_BERSERK)
			return FALSE;

		if (!(UNIT_CATTRIBUTES(unit) & CA_LOCKPICK) && VciIsLockpick(item))
			return FALSE;

		if (!(UNIT_CATTRIBUTES(unit) & CA_REFRESHER) && IsItemDanceRing(item))
			return FALSE;
	}

	return TRUE;
}

static int VciGetHealAmountVanilla(int item, struct Unit* unit)
{
	int idx = GetItemIndex(item);

	int result = 0;

	const struct VciHealEntry* it = gVciItemHealAmountList;

	while (it->itemId)
	{
		if (idx == it->itemId)
			result = it->healAmt;

		++it;
	}

	if (GetItemAttributes(item) & IA_STAFF)
		result += GetUnitPower(unit);

	if (result > 80)
		result = 80;

	return result;
}

static int VciIsStealableVanilla(int item)
{
	return (GetItemType(item) == ITYPE_ITEM);
}

static int VciIsHammernableVanilla(int item)
{
	if (!item)
		return FALSE;

	unsigned attrs = GetItemAttributes(item);

	if (!(attrs & (IA_WEAPON | IA_STAFF)))
		return FALSE;

	if (attrs & (IA_UNBREAKABLE | IA_HAMMERNE | IA_LOCK_3))
		return FALSE;

	if (GetItemUses(item) == GetItemMaxUses(item))
		return FALSE;

	return TRUE;
}

const struct VciInfo gVciDefaultInfo = {
	.getIndex              = VciGetIndexVanilla,
	.getName               = VciGetNameVanilla,
	.getDescId             = VciGetDescIdVanilla,
	.getUseDescId          = VciGetUseDescIdVanilla,
	.getType               = VciGetTypeVanilla,
	.getAttributes         = VciGetAttributesVanilla,
	.getUses               = VciGetUsesVanilla,
	.getMaxUses            = VciGetMaxUsesVanilla,
	.getMight              = VciGetMightVanilla,
	.getHit                = VciGetHitVanilla,
	.getWeight             = VciGetWeightVanilla,
	.getCrit               = VciGetCritVanilla,
	.getCost               = VciGetCostVanilla,
	.getMinRange           = VciGetMinRangeVanilla,
	.getMaxRange           = VciGetMaxRangeVanilla,
	.getEncodedRange       = VciGetEncodedRangeVanilla,
	.getRequiredExp        = VciGetRequiredExpVanilla,
	.getEffectiveness      = VciGetEffectivenessVanilla,
	.getStatBonuses        = VciGetStatBonusesVanilla,
	.getIconId             = VciGetIconIdVanilla,
	.getWeaponEffect       = VciGetWeaponEffectVanilla,
	.getUseEffect          = VciGetUseEffectVanilla,
	.getCostPerUse         = VciGetCostPerUseVanilla,
	.getMaxCost            = VciGetMaxCostVanilla,
	.getAwardedExp         = VciGetAwardedExpVanilla,

	.clone                 = VciCloneVanilla,
	.canUnitUseWeapon      = VciCanUnitUseWeaponVanilla,
	.canUnitUseStaff       = VciCanUnitUseStaffVanilla,
	.getAfterUse           = VciGetAfterUseVanilla,
	.isEffectiveAgainst    = VciIsEffectiveAgainstVanilla,
	.getDisplayRangeString = VciGetDisplayRangeStringVanilla,
	.getDisplayRankString  = VciGetDisplayRankStringVanilla,
	.isDisplayUsable       = VciIsDisplayUsableVanilla,
	.getHealAmount         = VciGetHealAmountVanilla,
	.isStealable           = VciIsStealableVanilla,
	.isHammernable         = VciIsHammernableVanilla,

	.drawMenuItem          = VciDrawMenuLineVanilla,
	.drawMenuItemLong      = VciDrawMenuLineLongVanilla,
	.drawMenuItemNoColor   = VciDrawMenuLineNoColorVanilla,
	.drawStatScreenItem    = VciDrawStatScreenLineVanilla,

	.getEquipHpBonus       = VciGetHpBonusVanilla,
	.getEquipPowBonus      = VciGetPowBonusVanilla,
	.getEquipSklBonus      = VciGetSklBonusVanilla,
	.getEquipSpdBonus      = VciGetSpdBonusVanilla,
	.getEquipDefBonus      = VciGetDefBonusVanilla,
	.getEquipResBonus      = VciGetResBonusVanilla,
	.getEquipLckBonus      = VciGetLckBonusVanilla,
};
