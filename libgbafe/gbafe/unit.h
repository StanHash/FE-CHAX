#ifndef GBAFE_UNIT_H
#define GBAFE_UNIT_H

// Rename to bmunit.h? Since it corresponds to bmunit.c from the proto
// Maybe corresponds to multiple files

#include <stdint.h>

#include "common.h"
#include "character.h"
#include "class.h"

typedef struct _Unit       Unit;
typedef struct _BattleUnit BattleUnit;

struct _Unit {
	CharacterData* pCharacterData;
	ClassData* pClassData;
	uint8_t level;
	uint8_t exp;
	uint8_t u0A_saved;
	uint8_t index;
	uint32_t state;
	uint8_t xPos;
	uint8_t yPos;
	uint8_t maxHP;
	uint8_t curHP;
	uint8_t pow;
	uint8_t skl;
	uint8_t spd;
	uint8_t def;
	uint8_t res;
	uint8_t lck;
	uint8_t conBonus;
	uint8_t rescueOtherUnit;
	uint8_t ballistaIndex;
	uint8_t movBonus;
	uint16_t items[5];
	uint8_t ranks[8];
	
	struct {
		uint8_t index    : 4;
		uint8_t duration : 4;
	} status;
	
	uint8_t torchDuration   : 4;
	uint8_t barrierDuration : 4;
	
	uint8_t supports[6];
	uint8_t unitLeader;
	uint8_t supportBits;
	uint8_t u3A;
	uint8_t u3B;
	void* pMapSpriteHandle;
	uint16_t ai3And4;
	uint8_t ai1;
	uint8_t ai1data;
	uint8_t ai2;
	uint8_t ai2data;
	uint8_t u46_saved;
	uint8_t u47;
};

struct _BattleUnit {
	Unit unit;
	
	uint16_t weaponAfter;
	uint16_t weaponBefore;
	uint32_t weaponAttributes;
	uint8_t  weaponType;
	uint8_t  weaponSlotIndex;
	
	uint8_t  canCounter;
	
	int8_t   WTHitModifier;
	int8_t   WTAtkModifier;
	
	uint8_t  terrainIndex;
	uint8_t  terrainDefense;
	uint8_t  terrainAvoid;
	uint8_t  terrainResistance;
	uint8_t  u59;
	
	struct {
		uint16_t attack;
		uint16_t defense;
		uint16_t attackSpeed;
		uint16_t hit;
		uint16_t avoid;
		uint16_t battleHit;
		uint16_t crit;
		uint16_t dodge;
		uint16_t battleCrit;
		uint16_t silencerRate;
	} battleStats;
	
	uint8_t  expGain;
	uint8_t  statusOut;
	uint8_t  levelPrevious;
	uint8_t  expPrevious;
	
	uint8_t  currentHP;
	
	int8_t   changeHP;
	int8_t   changePow;
	int8_t   changeSkl;
	int8_t   changeSpd;
	int8_t   changeDef;
	int8_t   changeRes;
	int8_t   changeLck;
	int8_t   changeCon;
	
	int8_t   wexpMultiplier;
	uint8_t  nonZeroDamage;
	uint8_t  weaponBroke;
	
	uint8_t  u7E;
	uint8_t  u7F;
};

#define UNIT_ATTRIBUTES(apUnit) ((apUnit)->pCharacterData->attributes | (apUnit)->pClassData->attributes)

extern Unit* active_unit; //! FE8U = (0x03004E50)

#pragma long_calls
#pragma long_calls_off

#endif // GBAFE_UNIT_H
