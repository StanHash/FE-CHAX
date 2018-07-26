#ifndef GBAFE_CLASS_H
#define GBAFE_CLASS_H

#include "common.h"

typedef struct ClassData ClassData;

struct ClassData {
	u16 nameTextId;
	u16 descTextId;
	u8 number;
	u8 promotion;
	u8 SMSId;
	u8 slowWalking;
	u16 defaultPortraitId;
	u8 _u0A;
	
	u8 baseHP;
	u8 basePow;
	u8 baseSkl;
	u8 baseSpd;
	u8 baseDef;
	u8 baseRes;
	u8 baseCon;
	u8 baseMov;
	
	u8 maxHP;
	u8 maxPow;
	u8 maxSkl;
	u8 maxSpd;
	u8 maxDef;
	u8 maxRes;
	u8 maxCon;
	
	u8 classRelativePower;
	
	u8 growthHP;
	u8 growthPow;
	u8 growthSkl;
	u8 growthSpd;
	u8 growthDef;
	u8 growthRes;
	u8 growthLck;
	
	u8 promotionHP;
	u8 promotionPow;
	u8 promotionSkl;
	u8 promotionSpd;
	u8 promotionDef;
	u8 promotionRes;
	
	u32 attributes;
	
	u8 ranks[8];
	
	const void* pBattleAnimDef;
	const u8* pMovCostTable[3]; // standard, rain, snow
	const u8* pTerrainBonusTables[3]; // def, avo, res
	
	const void* _pU50;
};

extern const struct ClassData gClassData[];

#endif // GBAFE_CLASS_H
