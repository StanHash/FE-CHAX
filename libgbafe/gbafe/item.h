#ifndef GBAFE_ITEM_H
#define GBAFE_ITEM_H

// bmitem.c

#include "unit.h"

typedef uint16_t Item;

typedef struct _ItemData ItemData;

struct _ItemData {
	uint16_t nameTextId;
	uint16_t descTextId;
	uint16_t useDescTextId;
	
	uint8_t  number;
	uint8_t  weaponType;
	
	uint32_t attributes;
	
	const void* pStatBonuses;
	const void* pEffectiveness;
	
	uint8_t  maxUses;
	
	uint8_t  might;
	uint8_t  hit;
	uint8_t  weight;
	uint8_t  crit;
	
	struct {
		uint8_t max : 4;
		uint8_t min : 4;
	} range;
	
	uint16_t costPerUse;
	uint8_t  weaponRank;
	uint8_t  iconId;
	uint8_t  useEffectId;
	uint8_t  weaponEffectId;
	uint8_t  weaponExp;
	
	uint8_t  _u21[3];
};

#define ITEM_SLOT_NONE  (-1)
#define ITEM_SLOT_COUNT (5)

extern const ItemData item_data[];

#pragma long_calls

char* GetItemSomeString(Item, int capitalized);   //! FE8U = (0x0801618C+1)

int  GetItemHealthBonus(Item);                    //! FE8U = (0x080163F0+1)
int  GetItemPowerBonus(Item);                     //! FE8U = (0x08016420+1)
int  GetItemSkillBonus(Item);                     //! FE8U = (0x08016450+1)
int  GetItemSpeedBonus(Item);                     //! FE8U = (0x08016480+1)
int  GetItemDefenseBonus(Item);                   //! FE8U = (0x080164B0+1)
int  GetItemResistanceBonus(Item);                //! FE8U = (0x080164E0+1)
int  GetItemLuckBonus(Item);                      //! FE8U = (0x08016510+1)

Item NewItem(uint8_t id);                         //! FE8U = (0x08016540+1)

int CanUnitWieldWeapon(const Unit*, Item);        //! FE8U = (0x08016574+1)
int CanUnitUseWeapon(const Unit*, Item);          //! FE8U = (0x08016750+1)

int CanUnitWieldStaff(const Unit*, Item);         //! FE8U = (0x080167A4+1)
int CanUnitUseStaff(const Unit*, Item);           //! FE8U = (0x08016800+1)

Item GetItemAfterUse(Item);                       //! FE8U = (0x08016AEC+1)

Item GetUnitEquippedItem(const Unit*);            //! FE8U = (0x08016B28+1)
int  GetUnitEquippedItemSlot(const Unit*);        //! FE8U = (0x08016B58+1)

int  IsItemCoveringRange(Item, int range);        //! FE8U = (0x08016B8C+1)

void EquipUnitItemSlot(Unit*, int slot);          //! FE8U = (0x08016BC0+1)

int  IsWeaponEffectiveAgainst(Item, const Unit*); //! FE8U = (0x08016BEC+1)

char* GetItemRangeString(Item);                   //! FE8U = (0x08016CC0+1)
int  GetWRankLevel(int rank);                     //! FE8U = (0x08016D5C+1)
char* GetItemWRankLevelString(Item);              //! FE8U = (0x08016D94+1)

char* GetWTypeString(int wtype);                  //! FE8U = (0x08016E20+1)

void GetWRankBarData(int wrank, int* valOut, int* maxOut); //! FE8U = (0x08016E50+1)

int IsItemDanceRing(Item);                        //! FE8U = (0x08016EC8+1)

#pragma long_calls_off

#endif // GBAFE_ITEM_H
