
#include "Vci.h"

extern const u8 gVciNightmareId;
extern const u8 gVciLockpickId;

const struct VciInfo* const gVciInfoTable[0x100] = {};

static inline
const struct VciInfo* VciGetInfo(int item)
{
	return gVciInfoTable[item & 0xFF];
}

int VciGetIndex(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getIndex)
		return VciGetInfo(item)->getIndex(item);

	return gVciDefaultInfo.getIndex(item);
}

char* VciGetName(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getName)
		return VciGetInfo(item)->getName(item);

	return gVciDefaultInfo.getName(item);
}

int VciGetDescId(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getDescId)
		return VciGetInfo(item)->getDescId(item);

	return gVciDefaultInfo.getDescId(item);
}

int VciGetUseDescId(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getUseDescId)
		return VciGetInfo(item)->getUseDescId(item);

	return gVciDefaultInfo.getUseDescId(item);
}

int VciGetType(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getType)
		return VciGetInfo(item)->getType(item);

	return gVciDefaultInfo.getType(item);
}

int VciGetAttributes(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getAttributes)
		return VciGetInfo(item)->getAttributes(item);

	return gVciDefaultInfo.getAttributes(item);
}

int VciGetUses(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getUses)
		return VciGetInfo(item)->getUses(item);

	return gVciDefaultInfo.getUses(item);
}

int VciGetMaxUses(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getMaxUses)
		return VciGetInfo(item)->getMaxUses(item);

	return gVciDefaultInfo.getMaxUses(item);
}

int VciGetMight(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getMight)
		return VciGetInfo(item)->getMight(item);

	return gVciDefaultInfo.getMight(item);
}

int VciGetHit(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getHit)
		return VciGetInfo(item)->getHit(item);

	return gVciDefaultInfo.getHit(item);
}

int VciGetWeight(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getWeight)
		return VciGetInfo(item)->getWeight(item);

	return gVciDefaultInfo.getWeight(item);
}

int VciGetCrit(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getCrit)
		return VciGetInfo(item)->getCrit(item);

	return gVciDefaultInfo.getCrit(item);
}

int VciGetCost(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getCost)
		return VciGetInfo(item)->getCost(item);

	return gVciDefaultInfo.getCost(item);
}

int VciGetMinRange(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getMinRange)
		return VciGetInfo(item)->getMinRange(item);

	return gVciDefaultInfo.getMinRange(item);
}

int VciGetMaxRange(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getMaxRange)
		return VciGetInfo(item)->getMaxRange(item);

	return gVciDefaultInfo.getMaxRange(item);
}

int VciGetEncodedRange(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEncodedRange)
		return VciGetInfo(item)->getEncodedRange(item);

	return gVciDefaultInfo.getEncodedRange(item);
}

int VciGetRequiredExp(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getRequiredExp)
		return VciGetInfo(item)->getRequiredExp(item);

	return gVciDefaultInfo.getRequiredExp(item);
}

const u8* VciGetEffectiveness(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEffectiveness)
		return VciGetInfo(item)->getEffectiveness(item);

	return gVciDefaultInfo.getEffectiveness(item);
}

const struct ItemStatBonuses* VciGetStatBonuses(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEquipBonuses)
		return VciGetInfo(item)->getEquipBonuses(item);

	return gVciDefaultInfo.getEquipBonuses(item);
}

int VciGetIconId(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getIconId)
		return VciGetInfo(item)->getIconId(item);

	return gVciDefaultInfo.getIconId(item);
}

int VciGetWeaponEffect(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getWeaponEffect)
		return VciGetInfo(item)->getWeaponEffect(item);

	return gVciDefaultInfo.getWeaponEffect(item);
}

int VciGetUseEffect(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getUseEffect)
		return VciGetInfo(item)->getUseEffect(item);

	return gVciDefaultInfo.getUseEffect(item);
}

int VciGetCostPerUse(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getCostPerUse)
		return VciGetInfo(item)->getCostPerUse(item);

	return gVciDefaultInfo.getCostPerUse(item);
}

int VciGetMaxCost(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getMaxCost)
		return VciGetInfo(item)->getMaxCost(item);

	return gVciDefaultInfo.getMaxCost(item);
}

int VciGetAwardedExp(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getAwardedExp)
		return VciGetInfo(item)->getAwardedExp(item);

	return gVciDefaultInfo.getAwardedExp(item);
}

int VciGetEquipHpBonus(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEquipHpBonus)
		return VciGetInfo(item)->getEquipHpBonus(item);

	return gVciDefaultInfo.getEquipHpBonus(item);
}

int VciGetEquipPowBonus(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEquipPowBonus)
		return VciGetInfo(item)->getEquipPowBonus(item);

	return gVciDefaultInfo.getEquipPowBonus(item);
}

int VciGetEquipSklBonus(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEquipSklBonus)
		return VciGetInfo(item)->getEquipSklBonus(item);

	return gVciDefaultInfo.getEquipSklBonus(item);
}

int VciGetEquipSpdBonus(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEquipSpdBonus)
		return VciGetInfo(item)->getEquipSpdBonus(item);

	return gVciDefaultInfo.getEquipSpdBonus(item);
}

int VciGetEquipDefBonus(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEquipDefBonus)
		return VciGetInfo(item)->getEquipDefBonus(item);

	return gVciDefaultInfo.getEquipDefBonus(item);
}

int VciGetEquipResBonus(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEquipResBonus)
		return VciGetInfo(item)->getEquipResBonus(item);

	return gVciDefaultInfo.getEquipResBonus(item);
}

int VciGetEquipLckBonus(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getEquipLckBonus)
		return VciGetInfo(item)->getEquipLckBonus(item);

	return gVciDefaultInfo.getEquipLckBonus(item);
}

int VciMakeNewItem(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->clone)
		return VciGetInfo(item)->clone(item);

	return gVciDefaultInfo.clone(item);
}

s8 VciCanUnitUseWeapon(struct Unit* unit, int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->canUnitUseWeapon)
		return VciGetInfo(item)->canUnitUseWeapon(item, unit);

	return gVciDefaultInfo.canUnitUseWeapon(item, unit);
}

s8 VciCanUnitUseStaff(struct Unit* unit, int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->canUnitUseStaff)
		return VciGetInfo(item)->canUnitUseStaff(item, unit);

	return gVciDefaultInfo.canUnitUseStaff(item, unit);
}

void VciDrawMenuLine(struct TextHandle* text, int item, s8 isUsable, u16* mapOut)
{
	if (VciGetInfo(item) && VciGetInfo(item)->drawMenuItem)
		return VciGetInfo(item)->drawMenuItem(item, text, mapOut, isUsable);

	return gVciDefaultInfo.drawMenuItem(item, text, mapOut, isUsable);
}

void VciDrawMenuLineLong(struct TextHandle* text, int item, s8 isUsable, u16* mapOut)
{
	if (VciGetInfo(item) && VciGetInfo(item)->drawMenuItemLong)
		return VciGetInfo(item)->drawMenuItemLong(item, text, mapOut, isUsable);

	return gVciDefaultInfo.drawMenuItemLong(item, text, mapOut, isUsable);
}

void VciDrawMenuLineNoColor(int item, struct TextHandle* text, u16* mapOut)
{
	if (VciGetInfo(item) && VciGetInfo(item)->drawMenuItemNoColor)
		return VciGetInfo(item)->drawMenuItemNoColor(item, text, mapOut);

	return gVciDefaultInfo.drawMenuItemNoColor(item, text, mapOut);
}

void VciDrawStatScreenLine(struct TextHandle* text, int item, int nameColor, u16* mapOut)
{
	if (VciGetInfo(item) && VciGetInfo(item)->drawStatScreenItem)
		return VciGetInfo(item)->drawStatScreenItem(item, text, mapOut, nameColor);

	return gVciDefaultInfo.drawStatScreenItem(item, text, mapOut, nameColor);
}

u16 VciGetItemAfterUse(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getAfterUse)
		return VciGetInfo(item)->getAfterUse(item);

	return gVciDefaultInfo.getAfterUse(item);
}

s8 VciIsEffectiveAgainst(int item, struct Unit* unit)
{
	if (VciGetInfo(item) && VciGetInfo(item)->isEffectiveAgainst)
		return VciGetInfo(item)->isEffectiveAgainst(item, unit);

	return gVciDefaultInfo.isEffectiveAgainst(item, unit);
}

char* VciGetDisplayRangeString(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getDisplayRangeString)
		return VciGetInfo(item)->getDisplayRangeString(item);

	return gVciDefaultInfo.getDisplayRangeString(item);
}

char* VciGetDisplayRankString(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getDisplayRankString)
		return VciGetInfo(item)->getDisplayRankString(item);

	return gVciDefaultInfo.getDisplayRankString(item);
}

s8 VciIsDisplayUsable(struct Unit* unit, int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->isDisplayUsable)
		return VciGetInfo(item)->isDisplayUsable(item, unit);

	return gVciDefaultInfo.isDisplayUsable(item, unit);
}

int VciGetHealAmount(struct Unit* unit, int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->getHealAmount)
		return VciGetInfo(item)->getHealAmount(item, unit);

	return gVciDefaultInfo.getHealAmount(item, unit);
}

s8 VciIsStealable(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->isStealable)
		return VciGetInfo(item)->isStealable(item);

	return gVciDefaultInfo.isStealable(item);
}

s8 VciIsHammernable(int item)
{
	if (VciGetInfo(item) && VciGetInfo(item)->isHammernable)
		return VciGetInfo(item)->isHammernable(item);

	return gVciDefaultInfo.isHammernable(item);
}

int VciIsNightmare(int item)
{
	return VciGetIndex(item) == gVciNightmareId;
}

int VciIsLockpick(int item)
{
	return VciGetIndex(item) == gVciLockpickId;
}
