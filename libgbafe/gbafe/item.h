#ifndef GBAFE_ITEM_H
#define GBAFE_ITEM_H

// bmitem.c

#include "unit.h"

typedef uint16_t Item;

#pragma long_calls

Item GetUnitEquippedItem(const Unit*); //! FE8U = (0x08016B28+1)

int  GetItemPowerBonus(Item);          //! FE8U = (0x08016420+1)
int  GetItemSkillBonus(Item);          //! FE8U = (0x08016450+1)
int  GetItemSpeedBonus(Item);          //! FE8U = (0x08016480+1)
int  GetItemDefenseBonus(Item);        //! FE8U = (0x080164B0+1)
int  GetItemResistanceBonus(Item);     //! FE8U = (0x080164E0+1)
int  GetItemLuckBonus(Item);           //! FE8U = (0x08016510+1)

Item NewItem(uint8_t id);              //! FE8U = (0x08016540+1)

#pragma long_calls_off

#endif // GBAFE_ITEM_H
