#ifndef GBAFE_UNIT_H
#define GBAFE_UNIT_H

// Rename to bmunit.h? Since it corresponds to bmunit.c from the proto
// Maybe corresponds to multiple files

#include <stdint.h>

#include "common.h"
#include "character.h"
#include "class.h"

typedef struct Unit Unit;
typedef struct BattleUnit BattleUnit;
typedef struct EventUnit EventUnit;

struct SMSHandle;

struct Unit {
	/* 00 */ const CharacterData* pCharacterData;
	/* 04 */ const ClassData* pClassData;
	
	/* 08 */ u8 level;
	/* 09 */ u8 exp;
	/* 0A */ u8 _u0A_saved;
	
	/* 0B */ u8 index;
	
	/* 0C */ u32 state;
	
	/* 10 */ u8 xPos;
	/* 11 */ u8 yPos;

	/* 12 */ u8 maxHP;
	/* 13 */ u8 curHP;
	/* 14 */ u8 pow;
	/* 15 */ u8 skl;
	/* 16 */ u8 spd;
	/* 17 */ u8 def;
	/* 18 */ u8 res;
	/* 19 */ u8 lck;

	/* 1A */ u8 conBonus;
	/* 1B */ u8 rescueOtherUnit;
	/* 1C */ u8 ballistaIndex;
	/* 1D */ u8 movBonus;

	/* 1E */ u16 items[5];
	/* 28 */ u8 ranks[8];
	
	/* 30 */ u8 statusIndex : 4;
	/* 30 */ u8 statusDuration : 4;
	
	/* 31 */ u8 torchDuration : 4;
	/* 31 */ u8 barrierDuration : 4;
	
	/* 32 */ u8 supports[6];
	/* 38 */ u8 unitLeader;
	/* 39 */ u8 supportBits;
	/* 3A */ u8 _u3A;
	/* 3B */ u8 _u3B;

	/* 3C */ struct SMSHandle* pMapSpriteHandle;

	/* 40 */ u16 ai3And4;
	/* 42 */ u8 ai1;
	/* 43 */ u8 ai1data;
	/* 44 */ u8 ai2;
	/* 45 */ u8 ai2data;
	/* 46 */ u8 _u46_saved;
	/* 47 */ u8 _u47;
};

struct BattleUnit {
	/* 00 */ struct Unit unit;
	
	/* 48 */ u16 weaponAfter;
	/* 4A */ u16 weaponBefore;
	/* 4C */ u32 weaponAttributes;
	/* 50 */ u8 weaponType;
	/* 51 */ u8 weaponSlotIndex;
	
	/* 52 */ u8 canCounter;
	
	/* 53 */ s8 WTHitModifier;
	/* 54 */ s8 WTAtkModifier;
	
	/* 55 */ u8 terrainIndex;
	/* 56 */ u8 terrainDefense;
	/* 57 */ u8 terrainAvoid;
	/* 58 */ u8 terrainResistance;
	/* 59 */ u8 _u59;

	/* 5A */ u16 battleAttack;
	/* 5C */ u16 battleDefense;
	/* 5E */ u16 battleAttackSpeed;
	/* 60 */ u16 battleHit;
	/* 62 */ u16 battleAvoid;
	/* 64 */ u16 battleEffectiveHit;
	/* 66 */ u16 battleCrit;
	/* 68 */ u16 battleDodge;
	/* 6A */ u16 battleEffectiveCrit;
	/* 6C */ u16 battleSilencerRate;

	/* 6E */ u8 expGain;
	/* 6F */ u8 statusOut;
	/* 70 */ u8 levelPrevious;
	/* 71 */ u8 expPrevious;
	
	/* 72 */ u8 currentHP;
	
	/* 73 */ s8 changeHP;
	/* 74 */ s8 changePow;
	/* 75 */ s8 changeSkl;
	/* 76 */ s8 changeSpd;
	/* 77 */ s8 changeDef;
	/* 78 */ s8 changeRes;
	/* 79 */ s8 changeLck;
	/* 7A */ s8 changeCon;
	
	/* 7B */ s8 wexpMultiplier;
	/* 7C */ u8 nonZeroDamage;
	/* 7D */ u8 weaponBroke;
	
	/* 7E */ u8 _u7E;
	/* 7F */ u8 _u7F;
};

struct EventUnit {
	u8 charIndex;
	u8 classIndex;
	u8 leaderCharIndex;

	u8 autolevel : 1;
	u8 allegiance : 2;
	u8 level : 5;

	u16 xPosition : 6;
	u16 yPosition : 6;
	u16 extraData : 4;

	u8 _u06;

	u8 redaCount;
	const void* redas;

	u8 items[4];
	u8 ai[4];
};

#define UNIT_ATTRIBUTES(apUnit) ((apUnit)->pCharacterData->attributes | (apUnit)->pClassData->attributes)

extern struct Unit gUnitArray[]; //! FE8U = 0x202BE4C
extern struct Unit* gActiveUnit; //! FE8U = 0x3004E50
extern struct Unit* const gUnitLookup[]; //! FE8U = 0x859A5D0

extern u8 gActiveUnitId; //! FE8U = 0x202BE44
extern struct Vector2 gActiveUnitPosition; //! FE8U = 0x202BE48

#pragma long_calls

void ClearUnits(void); //! FE8U = 0x80177C5
void ClearUnit(struct Unit*); //! FE8U = 0x80177F5
void CopyUnit(const struct Unit*, struct Unit*); //! FE8U = 0x801781D
struct Unit* GetNextFreeUnit(UnitAllegiance); //! FE8U = 0x8017839
struct Unit* GetNextFreePlayerUnit(); //! FE8U = 0x8017871

int GetUnitFogViewRange(const struct Unit*); //! FE8U = 0x80178A9

void SetUnitNewStatus(struct Unit*, int status); //! FE8U = 0x80178D9
void SetUnitStatus(struct Unit*, int status, int duration); //! FE8U = 0x80178F5

int GetUnitSMSIndex(const struct Unit*); //! FE8U = 0x8017905

int UnitAddItem(struct Unit*, u16); //! FE8U = 0x8017949
void ClearUnitInventory(struct Unit*); //! FE8U = 0x801796D
void RemoveUnitBlankItems(struct Unit*); //! FE8U = 0x8017985
int GetUnitItemCount(const struct Unit*); //! FE8U = 0x80179D9
int UnitHasItem(const struct Unit*, u16); //! FE8U = 0x80179F9

int LoadUnits(const struct EventUnit[]); //! FE8U = 0x8017A35
int HasClassWRank(u8); //! FE8U = 0x8017A8D
struct Unit* LoadUnit(const struct EventUnit*); //! FE8U = 0x8017AC5
void StoreNewUnitFromCode(struct Unit*, const struct EventUnit*); //! FE8U = 0x8017D3D
void CharFillInventoryFromCode(struct Unit*, const struct EventUnit*); //! FE8U = 0x8017DF9
void LoadUnitStats(struct Unit*, const struct CharacterData*); //! FE8U = 0x8017E35
void FixROMUnitStructPtr(struct Unit*); //! FE8U = 0x8017EBD
void LoadUnitSupports(struct Unit*); //! FE8U = 0x8017EF5
void AutolevelUnitWeaponRanks(struct Unit*); //! FE8U = 0x8017F21
void IncreaseUnitStatsByLevelCount(struct Unit*, int, int); //! FE8U = 0x8017FC5
void RecomputeUnitStatsForLevelPenalty(struct Unit*, int, int); //! FE8U = 0x8018065
void ApplyBonusLevels(struct Unit*, int); //! FE8U = 0x80180CD
void AutolevelUnit(struct Unit*); //! FE8U = 0x8018121
void AutolevelRealistic(struct Unit*); //! FE8U = 0x8018161
void EnsureNoUnitStatCapOverflow(struct Unit*); //! FE8U = 0x80181C9

struct Unit* GetUnitByCharId(u8); //! FE8U = 0x801829D
struct Unit* GetNonAllyUnitStructById(u8, enum UnitAllegiance); //! FE8U = 0x80182D9

int CanUnitRescue(const struct Unit*, const struct Unit*); //! FE8U = 0x801831D
void UnitRescue(struct Unit*, struct Unit*); //! FE8U = 0x801834D
void UnitDrop(struct Unit*, int x, int y); //! FE8U = 0x8018371
void UnitGive(struct Unit*, struct Unit*); //! FE8U = 0x80183C9
void ChangeUnitAllegiance(struct Unit*, enum UnitAllegiance); //! FE8U = 0x8018431
void ApplyUnitMovement(struct Unit*); //! FE8U = 0x801849D

void SetupActiveUnit(struct Unit*); //! FE8U = 0x801865D
void SetActiveUnit(struct Unit*); //! FE8U = 0x80186D5
void MoveActiveUnit(int x, int y); //! FE8U = 0x8018741

void SetAllUnitNotBackSprite(void); //! FE8U = 0x801895D

void UnitDecreaseItemUse(struct Unit*, int slot); //! FE8U = 0x8018995

int GetUnitAid(const struct Unit*); //! FE8U = 0x80189B9
int GetUnitMagBy2Range(const struct Unit*); //! FE8U = 0x8018A1D
int UnitHasMagicRank(const struct Unit*); //! FE8U = 0x8018A59
int GetUnitUseFlags(const struct Unit*); //! FE8U = 0x8018B29

int CanActiveUnitMove(void); //! FE8U = 0x8018BD9

int IsPositionMagicSealed(int x, int y); //! FE8U = 0x8018C99
int CanUnitNotUseMagic(const struct Unit*); //! FE8U = 0x8018D09

u16 GetUnitLastItem(const struct Unit*); //! FE8U = 0x8018D35

const u8* GetUnitMovCostTable(const struct Unit*); //! FE8U = 0x8018D4D

u8 GetClassStandingMapSpriteId(u8); //! FE8U = 0x8018D91

void MapMain_UpdatePrevDeployStates(void); //! FE8U = 0x8018DB1
void LoadUnitPrepScreenPositions(void); //! FE8U = 0x8018E31
int IsUnitSlotAvailable(enum UnitAllegiance); //! FE8U = 0x8018F49
void ClearCutsceneUnits(void); //! FE8U = 0x80190B5

int GetUnitCurrentHP(const struct Unit*); //! FE8U = 0x8019151
int GetUnitMaxHP(const struct Unit*); //! FE8U = 0x8019191
int GetUnitPower(const struct Unit*); //! FE8U = 0x80191B1
int GetUnitSkill(const struct Unit*); //! FE8U = 0x80191D1
int GetUnitSpeed(const struct Unit*); //! FE8U = 0x8019211
int GetUnitDefense(const struct Unit*); //! FE8U = 0x8019251
int GetUnitResistance(const struct Unit*); //! FE8U = 0x8019271
int GetUnitLuck(const struct Unit*); //! FE8U = 0x8019299
int GetUnitPortraitId(const struct Unit*); //! FE8U = 0x80192B9
int GetUnitMiniPortraitId(const struct Unit*); //! FE8U = 0x80192F5
u8 GetUnitLeader(const struct Unit*); //! FE8U = 0x8019341

void SetUnitHP(struct Unit*, int); //! FE8U = 0x8019369
void UnitTryHeal(struct Unit*, int); //! FE8U = 0x80193A5

char* GetUnitRescuingNameString(const struct Unit*); //! FE8U = 0x80193E9
char* GetUnitStatusString(const struct Unit*); //! FE8U = 0x8019415

struct Unit* GetUnit(u8 index); //! FE8U = 0x8019431

const struct ClassData* GetClassData(u8); //! FE8U = 0x8019445
const struct CharacterData* GetCharacterData(u8); //! FE8U = 0x8019465

void RemoveUnitItem(struct Unit*, int slot); //! FE8U = 0x8019485

int CanUnitCrossTerrain(const struct Unit*, u8 terrain); //! FE8U = 0x801949D

#pragma long_calls_off

#endif // GBAFE_UNIT_H
