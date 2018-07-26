#ifndef GBAFE_CHARACTER_H
#define GBAFE_CHARACTER_H

#include "common.h"

typedef struct CharacterData CharacterData;

struct CharacterData {
	u16 nameTextId;
	u16 descTextId;
	u8 number;
	u8 defaultClass;
	u16 portraitId;
	u8 miniPortrait;
	u8 affinity;
	u8 u0A;
	
	u8 baseLevel;
	u8 baseHP;
	u8 basePow;
	u8 baseSkl;
	u8 baseSpd;
	u8 baseDef;
	u8 baseRes;
	u8 baseLck;
	u8 baseCon;
	
	u8 ranks[8];
	
	u8 growthHP;
	u8 growthPow;
	u8 growthSkl;
	u8 growthSpd;
	u8 growthDef;
	u8 growthRes;
	u8 growthLck;
	
	u8 u23;
	u8 u24;
	u8 u25;
	u8 u26;
	u8 u27;
	
	u32 attributes;
	
	void* pSupportData;
	void* pU30;
};

extern const struct CharacterData gCharacterData[];

#endif // GBAFE_CHARACTER_H
