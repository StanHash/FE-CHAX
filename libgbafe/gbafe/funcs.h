#ifndef GBAFE_FUNCS_H
#define GBAFE_FUNCS_H

#include "common.h"

extern ActionData action_data; //! FE8U = (0x0203A958)

extern Vector2 map_size;       //! FE8U = (0x0202E4D4)

extern uint8_t** unit_map;     //! FE8U = (0x0202E4D8)
extern uint8_t** terrain_map;  //! FE8U = (0x0202E4DC)
extern uint8_t** movement_map; //! FE8U = (0x0202E4E0)
extern uint8_t** range_map;    //! FE8U = (0x0202E4E4)
extern uint8_t** fog_map;      //! FE8U = (0x0202E4E8)
extern uint8_t** hidden_map;   //! FE8U = (0x0202E4EC)
// extern uint8_t** other_movement_map; //! FE8U = (0x0202E4F0)

extern uint16_t** const tile_index_map; //! FE8U = (0x0859A9D4)

extern Trap trap_array[]; //! FE8U = (0x0203A614)

extern Unit* active_unit; //! FE8U = (0x03004E50)

extern const int16_t sin_table[]; //! FE8U = (0x080D751C)
extern const int16_t cos_table[]; //! FE8U = (0x080D759C)

#pragma long_calls

// <RNG Systems>

int NextRN();                      //! FE8U = (0x08000B88+1)
void LoadRNState(const uint16_t*); //! FE8U = (0x08000C34+1)
void SaveRNState(uint16_t*);       //! FE8U = (0x08000C4C+1)
int NextRN_100();                  //! FE8U = (0x08000C64+1)
int NextRN_N(int);                 //! FE8U = (0x08000C80+1)
int Roll1RN(int);                  //! FE8U = (0x08000CA0+1)
int Roll2RN(int);                  //! FE8U = (0x08000CB8+1)

// </RNG Systems>

// <6C/Proc System>

Proc* New6C(const ProcCode*, Proc*);             //! FE8U = (0x08002C7C+1)
Proc* New6CBlocking(const ProcCode*, Proc*);     //! FE8U = (0x08002CE0+1)
void Delete6C(Proc*);                            //! FE8U = (0x08002D6C+1)
void Exec6C(Proc*);                              //! FE8U = (0x08002E84+1)
void Break6CLoop(Proc*);                         //! FE8U = (0x08002E94+1)
Proc* Find6C(const ProcCode*);                   //! FE8U = (0x08002E9C+1)
void Goto6CLabel(Proc*, int);                    //! FE8U = (0x08002F24+1)
void Goto6CCode(Proc*, const ProcCode*);         //! FE8U = (0x08002F5C+1)
void Set6CMark(Proc*, int);                      //! FE8U = (0x08002F64+1)
void Set6CDestructor(Proc*, void(*)(Proc*));     //! FE8U = (0x08002F6C+1)
void ForAll6C(void(*)(Proc*));                   //! FE8U = (0x08002F70+1)
void ForEach6C(const ProcCode*, void(*)(Proc*)); //! FE8U = (0x08002F98+1)
void BlockEach6CMarked(int);                     //! FE8U = (0x08002FEC+1)
void UnblockEach6CMarked(int);                   //! FE8U = (0x08003014+1)
void DeleteEach6CMarked(int);                    //! FE8U = (0x08003040+1)
void DeleteEach6C(const ProcCode*);              //! FE8U = (0x08003078+1)
void BreakEach6CLoop(const ProcCode*);           //! FE8U = (0x08003094+1)
void Set6CLoop(Proc*, void(*)(Proc*));           //! FE8U = (0x08003450+1)

// </6C/Proc System>

// <Game Lock>

int LockGameLogic();    //! FE8U = (0x08015360+1)
int UnlockGameLogic();  //! FE8U = (0x08015370+1)
int GetGameLogicLock(); //! FE8U = (0x08015380+1)

// </Game Lock>

int CanUnitCrossTerrain(Unit* unit, uint8_t terrain); //! FE8U = (0x0801949C+1)

// <Maps>

void FlushTileUpdates();        //! FE8U = (0x08019A64+1)
void UpdateGameTilesGraphics(); //! FE8U = (0x08019C3C+1)
void PrepareMapChangeGfx();     //! FE8U = (0x08019CBC+1)
void RefreshEntityMaps();       //! FE8U = (0x0801A1F4+1)

// </Maps>

// <BMXFADE>

void NewBMXFADE(int locking);                       //! FE8U = (0x0801DDC4+1)
void NewBlockingBMXFADE(int locking, Proc* parent); //! FE8U = (0x0801DDF0+1)

int BMXFADEExists();                                //! FE8U = (0x0801DE18+1)

// </BMXFADE>

// <SMS>

void HideUnitSMS(Unit* unit); //! FE8U = (0x0802810C+1)
void ShowUnitSMS(Unit* unit); //! FE8U = (0x08028130+1)

// </SMS>

// <Traps>

Trap* GetTrapAt(int x, int y);                                                        //! FE8U = (0x0802E1F0+1)
Trap* GetSpecificTrapAt(int x, int y, int type);                                      //! FE8U = (0x0802E24C+1)

Trap* AddTrap(int x, int y, int type, int ext);                                       //! FE8U = (0x0802E2B8+1)
Trap* AddTrapExt(int x, int y, int type, int ext1, int ext24, int ext3, int ext5);    //! FE8U = (0x0802E2E0+1)
void RemoveTrap(Trap* trap);                                                          //! FE8U = (0x0802E2FC+1)
void AddFireTrap(int x, int y, int startCountDown, int resetCountDown);               //! FE8U = (0x0802E314+1)
void AddGasTrap(int x, int y, int direction, int startCountDown, int resetCountDown); //! FE8U = (0x0802E330+1)
void AddArrowTrap(int x, int startCountDown, int resetCountDown);                     //! FE8U = (0x0802E350+1)
void AddTrap8(int x, int y);                                                          //! FE8U = (0x0802E388+1)
void AddTrap9(int x, int y, int ext);                                                 //! FE8U = (0x0802E398+1)

void ApplyTrapMapChanges();                                                           //! FE8U = (0x0802E430+1)

void ApplyMapChangesById(int id);                                                     //! FE8U = (0x0802E58C+1)
void AddMapChange(int id);                                                            //! FE8U = (0x0802E5F8+1)
void RemoveMapChange(int id);                                                         //! FE8U = (0x0802E60C+1)

void AddGorgonEggTrap(int a, int b, int c, int d, int e);                             //! FE8U = (0x08037928+1)
Trap* AddBallista(int x, int y, int itemIndex);                                       //! FE8U = (0x08037A04+1)

// </Traps>

int GetStatIncrease(Unit*);

// <Roof>

void HideIfUnderRoof(Unit*); //! FE8U = (0x0802E660+1)
void UpdateUnitsUnderRoof(); //! FE8U = (0x0802E690+1)

// </Roof>

// <MoveUnit>

void ResetAllMoveUnitExtraData();                                             //! FE8U = (0x0807840C+1)
MoveUnitProc* NewMoveUnitForUnit_Specialized(Unit* unit, int mms, int palId); //! FE8U = (0x08078428+1)
MoveUnitProc* NewMoveUnitForUnit(Unit* unit);                                 //! FE8U = (0x08078464+1)
void EnableMoveUnitCameraFollow();                                            //! FE8U = (0x080784E4+1)
void DisableMoveUnitCameraFollow();                                           //! FE8U = (0x080784EC+1)
MoveUnitProc* NewMoveUnitForUI(Unit* unit);                                   //! FE8U = (0x080784F4+1)
MoveUnitProc* NewMoveUnit(int x, int y, int mms, int objBase, int palId);     //! FE8U = (0x08078540+1)
void SetMoveUnitDirection(MoveUnitProc* moveunit, int direction);             //! FE8U = (0x08078694+1)
void ResetMoveUnitDirection(MoveUnitProc* moveunit);                          //! FE8U = (0x080786BC+1)
void ResetMoveUnitDirection_Unique();                                         //! FE8U = (0x080786E8+1)
void SetMoveUnitMoveManual_Unique(uint8_t* moveManual);                       //! FE8U = (0x08078700+1)
int DoesMoveUnitExist();                                                      //! FE8U = (0x08078720+1)
int DoesMovingMoveUnitExist();                                                //! FE8U = (0x08078738+1)
void SetMoveUnitMoveManual(MoveUnitProc* moveunit, uint8_t* moveManual);      //! FE8U = (0x08078790+1)
void EndAllMoveUnits();                                                       //! FE8U = (0x080790A4+1)
void EndMoveUnit(MoveUnitProc* moveunit);                                     //! FE8U = (0x080790B4+1)

void SetMoveUnitDisplayPosition(MoveUnitProc* moveunit, int x, int y);        //! FE8U = (0x080797E4+1)

// </MoveUnit>

// <C Standard Library>

int div(int, int); // FE8U = (0x080D1AD4+1)
int mod(int, int); // FE8U = (0x080D1B4C+1)

void* memcpy(void* dest, const void* src, unsigned int count); //! FE8U = (0x080D1C0C+1)
void* memset(void* dest, int ch, unsigned int count);          //! FE8U = (0x080D1C6C+1)

// </C Standard Library>

uint16_t GetUnitEquippedItem(Unit*);
int GetItemPowerBonus(uint16_t);

#pragma long_calls_off

#endif // GBAFE_FUNCS_H
