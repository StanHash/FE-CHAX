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
typedef struct _EventUnit  EventUnit;

struct _Unit {
	CharacterData* pCharacterData;
	ClassData* pClassData;
	uint8_t level;
	uint8_t exp;
	uint8_t _u0A_saved;
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
	uint8_t _u3A;
	uint8_t _u3B;
	void* pMapSpriteHandle;
	uint16_t ai3And4;
	uint8_t ai1;
	uint8_t ai1data;
	uint8_t ai2;
	uint8_t ai2data;
	uint8_t _u46_saved;
	uint8_t _u47;
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

struct _EventUnit {
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
void UpdateRescuingData(Unit*, int x, int y); // (Drop)  //! FE8U = 0x8018371
void UpdateRescueData(Unit*, Unit*); // (Give)           //! FE8U = 0x80183C9
void HandleAllegianceChange(Unit*, UnitAllegiance);      //! FE8U = 0x8018431
void ApplyUnitMovement(Unit*);                           //! FE8U = 0x801849D

void SetupActiveUnit(Unit*);                             //! FE8U = 0x801865D
void SetActiveUnit(Unit*);                               //! FE8U = 0x80186D5
void MoveActiveUnit(int x, int y);                       //! FE8U = 0x8018741

void SetAllUnitNotBackSprite(void);                      //! FE8U = 0x801895D

void ValidateUnitItem(Unit*, int slot);                  //! FE8U = 0x8018995

int GetUnitAid(const Unit*);                             //! FE8U = 0x80189B9
int GetUnitMagBy2Range(const Unit*);                     //! FE8U = 0x8018A1D
int UnitHasMagicRank(const Unit*);                       //! FE8U = 0x8018A59
int GetUnitUseFlags(const Unit*);                        //! FE8U = 0x8018B29

int CanUnitMove(void);                                   //! FE8U = 0x8018BD9

int IsPosMagicSealed(int x, int y);                      //! FE8U = 0x8018C99
int CanUnitNotUseMagic(const Unit*);                     //! FE8U = 0x8018D09

uint16_t GetUnitLastItem(const Unit*);                   //! FE8U = 0x8018D35

const uint8_t* GetMovCostTablePtr(const Unit*);          //! FE8U = 0x8018D4D

uint8_t GetClassStandingMapSpriteId(uint8_t);            //! FE8U = 0x8018D91

void UpdatePrevDeployStates(void);                       //! FE8U = 0x8018DB1
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

ClassData GetROMClassStruct(uint8_t);                    //! FE8U = 0x8019445
CharacterData GetROMCharStruct(uint8_t);                 //! FE8U = 0x8019465

void RemoveUnitItem(Unit*, int slot);                    //! FE8U = 0x8019485

int CanUnitCrossTerrain(const Unit*, uint8_t terrain);   //! FE8U = 0x801949D

#pragma long_calls_off

// Better names that were not updated in fe8u.s yet
#define UnitDrop UpdateRescuingData
#define UnitGive UpdateRescueData
#define UnitDecreaseItemUse ValidateUnitItem
#define CanActiveUnitMove CanUnitMove
#define IsPositionMagicSealed IsPosMagicSealed
#define GetClassData GetROMClassStruct
#define GetCharacterData GetROMCharStruct

// compat with early versions
#define active_unit gActiveUnit

#endif // GBAFE_UNIT_H
