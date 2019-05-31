#ifndef VCI_INCLUDED
#define VCI_INCLUDED

#include "gbafe.h"

struct VciInfo
{
	// basic getters
	int (*getIndex) (int item);
	char* (*getName) (int item);
	int (*getDescId) (int item);
	int (*getUseDescId) (int item);
	int (*getType) (int item);
	int (*getAttributes) (int item);
	int (*getUses) (int item);
	int (*getMaxUses) (int item);
	int (*getMight) (int item);
	int (*getHit) (int item);
	int (*getWeight) (int item);
	int (*getCrit) (int item);
	int (*getCost) (int item);
	int (*getMinRange) (int item);
	int (*getMaxRange) (int item);
	int (*getEncodedRange) (int item);
	int (*getRequiredExp) (int item);
	const u8* (*getEffectiveness) (int item);
	const struct ItemStatBonuses* (*getEquipBonuses) (int item);
	int (*getIconId) (int item);
	int (*getWeaponEffect) (int item);
	int (*getUseEffect) (int item);
	int (*getCostPerUse) (int item);
	int (*getMaxCost) (int item);
	int (*getAwardedExp) (int item);

	// extended getters
	int (*clone) (int item);
	int (*canUnitUseWeapon) (int item, struct Unit* unit);
	int (*canUnitUseStaff) (int item, struct Unit* unit);
	int (*getAfterUse) (int item);
	int (*isEffectiveAgainst) (int item, struct Unit* unit);
	char* (*getDisplayRangeString) (int item);
	char* (*getDisplayRankString) (int item);
	int (*isDisplayUsable) (int item, struct Unit* unit);
	int (*getHealAmount) (int item, struct Unit* unit);
	int (*isStealable) (int item);
	int (*isHammernable) (int item);

	// display functions
	void (*drawMenuItem) (int item, struct TextHandle* text, u16* mapOut, int isUsable);
	void (*drawMenuItemLong) (int item, struct TextHandle* text, u16* mapOut, int isUsable);
	void (*drawMenuItemNoColor) (int item, struct TextHandle* text, u16* mapOut);
	void (*drawStatScreenItem) (int item, struct TextHandle* text, u16* mapOut, int nameColor);

	// stat bonuses getters
	int (*getEquipHpBonus) (int item);
	int (*getEquipPowBonus) (int item);
	int (*getEquipSklBonus) (int item);
	int (*getEquipSpdBonus) (int item);
	int (*getEquipDefBonus) (int item);
	int (*getEquipResBonus) (int item);
	int (*getEquipLckBonus) (int item);
};

// functions

int VciIsLockpick(int item);
int VciIsNightmare(int item);

// objects

extern const struct VciInfo* const gVciInfoTable[0x100];
extern const struct VciInfo gVciDefaultInfo;

#endif // VCI_INCLUDED
