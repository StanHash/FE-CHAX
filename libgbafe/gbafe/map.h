#ifndef GBAFE_MAP_H
#define GBAFE_MAP_H

// Rename to bmmap.h? Since it corresponds to bmmap.c from the proto

#include "common.h"
#include "unit.h"

typedef uint8_t** MapData;

extern Vector2 gMapSize;        //! FE8U = (0x0202E4D4)

extern MapData gMapUnit;      //! FE8U = (0x0202E4D8)
extern MapData gMapTerrain;   //! FE8U = (0x0202E4DC)
extern MapData gMapMovement;  //! FE8U = (0x0202E4E0)
extern MapData gMapRange;     //! FE8U = (0x0202E4E4)
extern MapData gMapFog;       //! FE8U = (0x0202E4E8)
extern MapData gMapHidden;    //! FE8U = (0x0202E4EC)
extern MapData gMapMovement2; //! FE8U = (0x0202E4F0)

extern uint16_t** const gMapRawTiles; //! FE8U = (0x0859A9D4)

#pragma long_calls

void InitChapterMap(uint8_t chapterIndex);                                    //! FE8U = 0x80194BD
void SetupMapRowPointers(void* pool, MapData* target, int width, int height); //! FE8U = 0x80197A5

void ClearMapWith(MapData, uint8_t value);                                    //! FE8U = 0x80197E5

void LoadChapterMap(void* buffer, uint8_t chapterIndex);                      //! FE8U = 0x80198AD
void LoadChapterMapGfx(uint8_t chapterIndex);                                 //! FE8U = 0x801990D
void FlushTilesFromBuffer(void);                                              //! FE8U = 0x80199A5

void RefreshTileMaps(void);                                                   //! FE8U = 0x8019A65

uint8_t GetSomeTerrainToChangeAtSomePosition(int x, int y);                   //! FE8U = 0x8019AF5

void UpdateGameTileGfx(uint16_t* bg, int x, int y, int xTile, int yTile);     //! FE8U = 0x8019B19
void DrawTileGraphics(void);                                                  //! FE8U = 0x8019C3D

void InitMapChangeGraphics(void);                                             //! FE8U = 0x8019CBD

void UpdateUnitMapAndVision(void);                                            //! FE8U = 0x8019FA1
void UpdateTrapFogVision(void);                                               //! FE8U = 0x801A175
void UpdateTrapHiddenStates(void);                                            //! FE8U = 0x801A1A1
void RefreshEntityMaps(void);                                                 //! FE8U = 0x801A1F4

char* GetTerrainNameString(uint8_t);                                          //! FE8U = 0x801A241
int   GetTerrainHealAmount(uint8_t);                                          //! FE8U = 0x801A259
int   GetTerrainSomething(uint8_t);                                           //! FE8U = 0x801A269

void RevertMapChangesById(int id);                                            //! FE8U = 0x801A2ED

void FillMovementMapForUnit(const Unit*);                                     //! FE8U = 0x801A38D
void FillMovementMapForUnitAndMovement(const Unit*, int movement);            //! FE8U = 0x801A3CD
void FillMovementMapForUnitPosition(const Unit*);                             //! FE8U = 0x801A409
void FillMovementRangeMapSomehow(int x, int y, const uint8_t costTable[]);    //! FE8U = 0x801A43D
void FillMovementMapSomehow(int x, int y, const uint8_t costTable[]);         //! FE8U = 0x801A46D
void FillMovementMapForUnitAt(const Unit*, int x, int y, int movement);       //! FE8U = 0x801A49D
void StoreMovCostTable(const uint8_t table[]);                                //! FE8U = 0x801A4CD
void FillMovementMap(int x, int y, int movement, uint8_t unitIndex);          //! FE8U = 0x801A4ED

void CheckForHiddenStuffUnitMovement(Unit*, int xStart, int yStart);          //! FE8U = 0x801A82D

void MapAddInRange(int x, int y, int range, int value);                       //! FE8U = 0x801AABD
void StoreR3ToMapSomething(int x, int y, int range, int value);               //! FE8U = 0x801ABC1

void FillMapAttackRangeForUnit(const Unit*);                                  //! FE8U = 0x801ACBD
void FillRangeByRangeMask(const Unit*, uint32_t mask);                        //! FE8U = 0x801B461
void FillMapStaffRangeForUnit(const Unit*);                                   //! FE8U = 0x801B619
void ApplyStuffToRangeMaps(int boolStaff);                                    //! FE8U = 0x801B811
void SetSubjectMap(MapData);                                                  //! FE8U = 0x801B999
void Map_IncRange(int x, int y, int minRange, int maxRange);                  //! FE8U = 0x801B9A5

uint8_t GetCurrentMovCostTable(void);                                         //! FE8U = 0x801B9E5

#pragma long_calls_off

// better names
#define SyncTilesFromMapBuffer FlushTilesFromBuffer
#define RefreshUnitMapAndVision UpdateUnitMapAndVision
#define RefreshTrapFogVision UpdateTrapFogVision
#define RefreshTrapHiddenStates UpdateTrapHiddenStates
#define FillMovementRangeMapForPosition FillMovementRangeMapSomehow
#define FillMovementMapForPosition FillMovementMapSomehow
#define ProcessUnitMovement CheckForHiddenStuffUnitMovement
#define MapSetInRange StoreR3ToMapSomething
#define FillRangeMapByRangeMask FillRangeByRangeMask
#define FillRangeMapForDangerZone ApplyStuffToRangeMaps
#define MapIncInBoundedRange Map_IncRange

#endif // GBAFE_MAP_H
