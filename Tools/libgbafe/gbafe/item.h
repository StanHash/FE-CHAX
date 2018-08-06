#ifndef GBAFE_ITEM_H
#define GBAFE_ITEM_H

// bmitem.c

#include "unit.h"

typedef u16 Item;

typedef struct ItemData ItemData;

struct ItemData {
	u16 nameTextId;
	u16 descTextId;
	u16 useDescTextId;
	
	u8 number;
	u8 weaponType;
	
	u32 attributes;
	
	const u8* pStatBonuses;
	const u8* pEffectiveness;
	
	u8 maxUses;
	
	u8 might;
	u8 hit;
	u8 weight;
	u8 crit;
	
	u8 maxRange : 4;
	u8 minRange : 4;
	
	u16 costPerUse;
	u8 weaponRank;
	u8 iconId;
	u8 useEffectId;
	u8 weaponEffectId;
	u8 weaponExp;
	
	u8 _u21[3];
};

#define ITEM_SLOT_NONE (-1)
#define ITEM_SLOT_COUNT (5)

extern const ItemData gItemData[];

#pragma long_calls

char* GetItemSomeString(Item, int capitalized); //! FE8U = (0x0801618C+1)

int GetItemHealthBonus(Item); //! FE8U = (0x080163F0+1)
int GetItemPowerBonus(Item); //! FE8U = (0x08016420+1)
int GetItemSkillBonus(Item); //! FE8U = (0x08016450+1)
int GetItemSpeedBonus(Item); //! FE8U = (0x08016480+1)
int GetItemDefenseBonus(Item); //! FE8U = (0x080164B0+1)
int GetItemResistanceBonus(Item); //! FE8U = (0x080164E0+1)
int GetItemLuckBonus(Item); //! FE8U = (0x08016510+1)

Item NewItem(u8 id); //! FE8U = (0x08016540+1)

int CanUnitWieldWeapon(const struct Unit*, Item); //! FE8U = (0x08016574+1)
int CanUnitUseWeapon(const struct Unit*, Item); //! FE8U = (0x08016750+1)

int CanUnitWieldStaff(const struct Unit*, Item); //! FE8U = (0x080167A4+1)
int CanUnitUseStaff(const struct Unit*, Item); //! FE8U = (0x08016800+1)

Item GetItemAfterUse(Item); //! FE8U = (0x08016AEC+1)

Item GetUnitEquippedItem(const struct Unit*); //! FE8U = (0x08016B28+1)
int GetUnitEquippedItemSlot(const struct Unit*); //! FE8U = (0x08016B58+1)

int IsItemCoveringRange(Item, int range); //! FE8U = (0x08016B8C+1)

void EquipUnitItemSlot(struct Unit*, int slot); //! FE8U = (0x08016BC0+1)

int IsWeaponEffectiveAgainst(Item, const struct Unit*); //! FE8U = (0x08016BEC+1)

char* GetItemRangeString(Item); //! FE8U = (0x08016CC0+1)
int GetWRankLevel(int rank); //! FE8U = (0x08016D5C+1)
char* GetItemWRankLevelString(Item); //! FE8U = (0x08016D94+1)

char* GetWTypeString(int wtype); //! FE8U = (0x08016E20+1)

void GetWRankBarData(int wrank, int* valOut, int* maxOut); //! FE8U = (0x08016E50+1)

int IsItemDanceRing(Item); //! FE8U = (0x08016EC8+1)
int IsItemUsable(const struct Unit*, Item); //! FE8U = 0x8016EE5
int CanUnitUse(const struct Unit*, Item); //! FE8U = 0x8016F7D
int GetItemHealAmount(const struct Unit*, Item); //! FE8U = 0x8016FB9
int GetUnitItemSlot(const struct Unit*, Item); //! FE8U = 0x801702D

int IsItemStealable(Item); //! FE8U = 0x8017055
int IsItemHammernable(Item); //! FE8U = 0x8017081

u32 GetWeaponRangeMask(Item); //! FE8U = 0x80170D5
u32 GetUnitRangeMask(const struct Unit*, int slot); //! FE8U = 0x80171E9
u32 GetUnitStaffRangeMask(const struct Unit*); //! FE8U = 0x80172F9

int GetConvoyTotalGoldValue(); //! FE8U = 0x801737D

u8 GetItemIndex(Item); //! FE8U = 0x80174ED
char* GetItemNameString(Item); //! FE8U = 0x80174F5
int GetItemDescTextIndex(Item); //! FE8U = 0x8017519
int GetItemUseDescTextIndex(Item); //! FE8U = 0x8017531
int GetItemWType(Item); //! FE8U = 0x8017549
u32 GetItemAttributes(Item); //! FE8U = 0x801756D
int GetItemUses(Item); //! FE8U = 0x8017585
int GetItemMaxUses(Item); //! FE8U = 0x80175B1
int GetItemMight(Item); //! FE8U = 0x80175DD
int GetItemHit(Item); //! FE8U = 0x80175F5
int GetItemWeight(Item); //! FE8U = 0x801760D
int GetItemCrit(Item); //! FE8U = 0x8017625
int GetItemCost(Item); //! FE8U = 0x801763D
int GetItemMinRange(Item); //! FE8U = 0x801766D
int GetItemMaxRange(Item); //! FE8U = 0x8017685
int GetItemRange(Item); //! FE8U = 0x80176A1
int GetItemWRank(Item); //! FE8U = 0x80176B9
u8* GetItemEffectivenessPtr(Item); //! FE8U = 0x80176D1
u8* GetItemStatBonusesPointer(Item); //! FE8U = 0x80176E9
int GetItemIconId(Item); //! FE8U = 0x8017701
int GetItemWeaponEffect(Item); //! FE8U = 0x8017725
int GetItemUseEffect(Item); //! FE8U = 0x801773D
int GetItemCostPerUse(Item); //! FE8U = 0x8017755
int GetItemMaxCost(Item); //! FE8U = 0x801776D
int GetItemWExp(Item); //! FE8U = 0x8017799

const ItemData* GetItemData(u8); //! FE8U = 0x80177B1

#pragma long_calls_off

#define GetItemData GetItemData

#endif // GBAFE_ITEM_H
