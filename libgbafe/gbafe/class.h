#ifndef GBAFE_CLASS_H
#define GBAFE_CLASS_H

#include <stdint.h>

#include "common.h"

typedef struct _ClassData ClassData;

struct _ClassData {
	uint16_t nameTextId;
	uint16_t descTextId;
	uint8_t  number;
	uint8_t  promotion;
	uint8_t  SMSId;
	uint8_t  slowWalking;
	uint16_t defaultPortraitId;
	uint8_t  u0A;
	
	uint8_t  baseHP;
	uint8_t  basePow;
	uint8_t  baseSkl;
	uint8_t  baseSpd;
	uint8_t  baseDef;
	uint8_t  baseRes;
	uint8_t  baseLck;
	uint8_t  baseCon;
	uint8_t  baseMov;
	
	uint8_t  maxHP;
	uint8_t  maxPow;
	uint8_t  maxSkl;
	uint8_t  maxSpd;
	uint8_t  maxDef;
	uint8_t  maxRes;
	uint8_t  maxLck;
	uint8_t  maxCon;
	
	uint8_t  classRelativePower;
	
	uint8_t  growthHP;
	uint8_t  growthPow;
	uint8_t  growthSkl;
	uint8_t  growthSpd;
	uint8_t  growthDef;
	uint8_t  growthRes;
	uint8_t  growthLck;
	
	uint8_t  promotionHP;
	uint8_t  promotionPow;
	uint8_t  promotionSkl;
	uint8_t  promotionSpd;
	uint8_t  promotionDef;
	uint8_t  promotionRes;
	
	uint32_t attributes;
	
	uint8_t  ranks[8];
	
	void*    pBattleAnimDef;
	void*    pMovCostTable[3]; // standard, rain, snow
	void*    pTerrainBonusTables[3]; // def, avo, res
	
	void*    pU50;
};

#endif // GBAFE_CLASS_H
