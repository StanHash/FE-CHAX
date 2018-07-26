#ifndef GBAFE_UNIT_H
#define GBAFE_UNIT_H

// Rename to bmunit.h? Since it corresponds to bmunit.c from the proto
// Maybe corresponds to multiple files

#include <stdint.h>

#include "common.h"
#include "character.h"
#include "class.h"

typedef struct Unit       Unit;
typedef struct BattleUnit BattleUnit;
typedef struct EventUnit  EventUnit;

struct Unit {
	/* 00 */ const CharacterData* pCharacterData;
	/* 04 */ const ClassData* pClassData;
	
	/* 08 */ uint8_t level;
	/* 09 */ uint8_t exp;
	/* 0A */ uint8_t _u0A_saved;
	
	/* 0B */ uint8_t index;
	
	/* 0C */ uint32_t state;
	
	/* 10 */ uint8_t xPos;
	/* 11 */ uint8_t yPos;

	/* 12 */ uint8_t maxHP;
	/* 13 */ uint8_t curHP;
	/* 14 */ uint8_t pow;
	/* 15 */ uint8_t skl;
	/* 16 */ uint8_t spd;
	/* 17 */ uint8_t def;
	/* 18 */ uint8_t res;
	/* 19 */ uint8_t lck;

	/* 1A */ uint8_t conBonus;
	/* 1B */ uint8_t rescueOtherUnit;
	/* 1C */ uint8_t ballistaIndex;
	/* 1D */ uint8_t movBonus;

	/* 1E */ uint16_t items[5];
	/* 28 */ uint8_t ranks[8];
	
	/* 30 */ uint8_t statusIndex    : 4;
	/* 30 */ uint8_t statusDuration : 4;
	
	/* 31 */ uint8_t torchDuration   : 4;
	/* 31 */ uint8_t barrierDuration : 4;
	
	/* 32 */ uint8_t supports[6];
	/* 38 */ uint8_t unitLeader;
	/* 39 */ uint8_t supportBits;
	/* 3A */ uint8_t _u3A;
	/* 3B */ uint8_t _u3B;

	/* 3C */ void* pMapSpriteHandle;
	
	/* 40 */ uint16_t ai3And4;
	/* 42 */ uint8_t ai1;
	/* 43 */ uint8_t ai1data;
	/* 44 */ uint8_t ai2;
	/* 45 */ uint8_t ai2data;
	/* 46 */ uint8_t _u46_saved;
	/* 47 */ uint8_t _u47;
};

struct BattleUnit {
	/* 00 */ struct Unit unit;
	
	/* 48 */ uint16_t weaponAfter;
	/* 4A */ uint16_t weaponBefore;
	/* 4C */ uint32_t weaponAttributes;
	/* 50 */ uint8_t  weaponType;
	/* 51 */ uint8_t  weaponSlotIndex;
	
	/* 52 */ uint8_t  canCounter;
	
	/* 53 */ int8_t   WTHitModifier;
	/* 54 */ int8_t   WTAtkModifier;
	
	/* 55 */ uint8_t  terrainIndex;
	/* 56 */ uint8_t  terrainDefense;
	/* 57 */ uint8_t  terrainAvoid;
	/* 58 */ uint8_t  terrainResistance;
	/* 59 */ uint8_t  _u59;

	/* 5A */ uint16_t battleAttack;
	/* 5C */ uint16_t battleDefense;
	/* 5E */ uint16_t battleAttackSpeed;
	/* 60 */ uint16_t battleHit;
	/* 62 */ uint16_t battleAvoid;
	/* 64 */ uint16_t battleEffectiveHit;
	/* 66 */ uint16_t battleCrit;
	/* 68 */ uint16_t battleDodge;
	/* 6A */ uint16_t battleEffectiveCrit;
	/* 6C */ uint16_t battleSilencerRate;

	/* 6E */ uint8_t  expGain;
	/* 6F */ uint8_t  statusOut;
	/* 70 */ uint8_t  levelPrevious;
	/* 71 */ uint8_t  expPrevious;
	
	/* 72 */ uint8_t  currentHP;
	
	/* 73 */ int8_t   changeHP;
	/* 74 */ int8_t   changePow;
	/* 75 */ int8_t   changeSkl;
	/* 76 */ int8_t   changeSpd;
	/* 77 */ int8_t   changeDef;
	/* 78 */ int8_t   changeRes;
	/* 79 */ int8_t   changeLck;
	/* 7A */ int8_t   changeCon;
	
	/* 7B */ int8_t   wexpMultiplier;
	/* 7C */ uint8_t  nonZeroDamage;
	/* 7D */ uint8_t  weaponBroke;
	
	/* 7E */ uint8_t  _u7E;
	/* 7F */ uint8_t  _u7F;
};

struct EventUnit {
	uint8_t charIndex;
	uint8_t classIndex;
	uint8_t leaderCharIndex;

	uint8_t autolevel  : 1;
	uint8_t allegiance : 2;
	uint8_t level      : 5;

	uint16_t xPosition : 6;
	uint16_t yPosition : 6;
	uint16_t extraData : 4;

	uint8_t  _u06;

	uint8_t     redaCount;
	const void* redas;

	uint8_t  items[4];
	uint8_t  ai[4];
};

#define UNIT_ATTRIBUTES(apUnit) ((apUnit)->pCharacterData->attributes | (apUnit)->pClassData->attributes)

extern Unit        gUnitArray[];  //! FE8U = 0x202BE4C
extern Unit*       gActiveUnit;   //! FE8U = 0x3004E50
extern Unit* const gUnitLookup[]; //! FE8U = 0x859A5D0

#pragma long_calls

void ClearUnits(void);                                   //! FE8U = 0x80177C5
void ClearUnit(Unit*);                                   //! FE8U = 0x80177F5
void CopyUnit(const Unit*, Unit*);                       //! FE8U = 0x801781D
Unit* GetNextFreeUnit(UnitAllegiance);                   //! FE8U = 0x8017839
Unit* GetNextFreePlayerUnit();                           //! FE8U = 0x8017871

int GetUnitFogViewRange(const Unit*);                    //! FE8U = 0x80178A9

void SetUnitNewStatus(Unit*, int status);                //! FE8U = 0x80178D9
void SetUnitStatus(Unit*, int status, int duration);     //! FE8U = 0x80178F5

int GetUnitSMSIndex(const Unit*);                        //! FE8U = 0x8017905

int UnitAddItem(Unit*, uint16_t);                        //! FE8U = 0x8017949
void ClearUnitInventory(Unit*);                          //! FE8U = 0x801796D
void RemoveUnitBlankItems(Unit*);                        //! FE8U = 0x8017985
int GetUnitItemCount(const Unit*);                       //! FE8U = 0x80179D9
int UnitHasItem(const Unit*, uint16_t);                  //! FE8U = 0x80179F9

int LoadUnits(const EventUnit[]);                        //! FE8U = 0x8017A35
int HasClassWRank(uint8_t);                              //! FE8U = 0x8017A8D
Unit* LoadUnit(const EventUnit*);                        //! FE8U = 0x8017AC5
void StoreNewUnitFromCode(Unit*, const EventUnit*);      //! FE8U = 0x8017D3D
void CharFillInventoryFromCode(Unit*, const EventUnit*); //! FE8U = 0x8017DF9
void LoadUnitStats(Unit*, const CharacterData*);         //! FE8U = 0x8017E35
void FixROMUnitStructPtr(Unit*);                         //! FE8U = 0x8017EBD
void LoadUnitSupports(Unit*);                            //! FE8U = 0x8017EF5
void AutolevelUnitWeaponRanks(Unit*);                    //! FE8U = 0x8017F21
void IncreaseUnitStatsByLevelCount(Unit*, int, int);     //! FE8U = 0x8017FC5
void RecomputeUnitStatsForLevelPenalty(Unit*, int, int); //! FE8U = 0x8018065
void ApplyBonusLevels(Unit*, int);                       //! FE8U = 0x80180CD
void AutolevelUnit(Unit*);                               //! FE8U = 0x8018121
void AutolevelRealistic(Unit*);                          //! FE8U = 0x8018161
void EnsureNoUnitStatCapOverflow(Unit*);                 //! FE8U = 0x80181C9

Unit* GetUnitByCharId(uint8_t);                          //! FE8U = 0x801829D
Unit* GetNonAllyUnitStructById(uint8_t, UnitAllegiance); //! FE8U = 0x80182D9

int CanUnitRescue(const Unit*, const Unit*);             //! FE8U = 0x801831D
void UnitRescue(Unit*, Unit*);                           //! FE8U = 0x801834D
void UnitDrop(Unit*, int x, int y); // (Drop)  //! FE8U = 0x8018371
void UnitGive(Unit*, Unit*); // (Give)           //! FE8U = 0x80183C9
void ChangeUnitAllegiance(Unit*, UnitAllegiance);      //! FE8U = 0x8018431
void ApplyUnitMovement(Unit*);                           //! FE8U = 0x801849D

void SetupActiveUnit(Unit*);                             //! FE8U = 0x801865D
void SetActiveUnit(Unit*);                               //! FE8U = 0x80186D5
void MoveActiveUnit(int x, int y);                       //! FE8U = 0x8018741

void SetAllUnitNotBackSprite(void);                      //! FE8U = 0x801895D

void UnitDecreaseItemUse(Unit*, int slot);                  //! FE8U = 0x8018995

int GetUnitAid(const Unit*);                             //! FE8U = 0x80189B9
int GetUnitMagBy2Range(const Unit*);                     //! FE8U = 0x8018A1D
int UnitHasMagicRank(const Unit*);                       //! FE8U = 0x8018A59
int GetUnitUseFlags(const Unit*);                        //! FE8U = 0x8018B29

int CanActiveUnitMove(void);                                   //! FE8U = 0x8018BD9

int IsPositionMagicSealed(int x, int y);                      //! FE8U = 0x8018C99
int CanUnitNotUseMagic(const Unit*);                     //! FE8U = 0x8018D09

uint16_t GetUnitLastItem(const Unit*);                   //! FE8U = 0x8018D35

const uint8_t* GetUnitMovCostTable(const Unit*);          //! FE8U = 0x8018D4D

uint8_t GetClassStandingMapSpriteId(uint8_t);            //! FE8U = 0x8018D91

void MapMain_UpdatePrevDeployStates(void);                       //! FE8U = 0x8018DB1
void LoadUnitPrepScreenPositions(void);                  //! FE8U = 0x8018E31
int IsUnitSlotAvailable(UnitAllegiance);                 //! FE8U = 0x8018F49
void ClearCutsceneUnits(void);                           //! FE8U = 0x80190B5

int GetUnitCurrentHP(const Unit*);                       //! FE8U = 0x8019151
int GetUnitMaxHP(const Unit*);                           //! FE8U = 0x8019191
int GetUnitPower(const Unit*);                           //! FE8U = 0x80191B1
int GetUnitSkill(const Unit*);                           //! FE8U = 0x80191D1
int GetUnitSpeed(const Unit*);                           //! FE8U = 0x8019211
int GetUnitDefense(const Unit*);                         //! FE8U = 0x8019251
int GetUnitResistance(const Unit*);                      //! FE8U = 0x8019271
int GetUnitLuck(const Unit*);                            //! FE8U = 0x8019299
int GetUnitPortraitId(const Unit*);                      //! FE8U = 0x80192B9
int GetUnitMiniPortraitId(const Unit*);                  //! FE8U = 0x80192F5
uint8_t GetUnitLeader(const Unit*);                      //! FE8U = 0x8019341

void SetUnitHP(Unit*, int);                              //! FE8U = 0x8019369
void UnitTryHeal(Unit*, int);                            //! FE8U = 0x80193A5

char* GetUnitRescuingNameString(const Unit*);            //! FE8U = 0x80193E9
char* GetUnitStatusString(const Unit*);                  //! FE8U = 0x8019415

Unit* GetUnit(uint8_t index);                            //! FE8U = 0x8019431

ClassData GetClassData(uint8_t);                    //! FE8U = 0x8019445
CharacterData GetCharacterData(uint8_t);                 //! FE8U = 0x8019465

void RemoveUnitItem(Unit*, int slot);                    //! FE8U = 0x8019485

int CanUnitCrossTerrain(const Unit*, uint8_t terrain);   //! FE8U = 0x801949D

#pragma long_calls_off

#endif // GBAFE_UNIT_H
