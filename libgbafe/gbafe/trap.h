#ifndef GBAFE_TRAPS_H
#define GBAFE_TRAPS_H

// bmtrap.c?

#include <stdint.h>

#include "common.h"

typedef struct _EventTrapData EventTrapData;

typedef struct _Trap Trap;
typedef struct _Trap ActiveTile; // because speedruns are neat

struct _EventTrapData {
	uint8_t type;
	uint8_t data[5];
};

struct _Trap {
	uint8_t xPosition;
	uint8_t yPosition;
	
	uint8_t type;
	
	uint8_t data[5];
};

extern Trap trap_array[]; //! FE8U = (0x0203A614)

#pragma long_calls

Trap* GetTrapAt(int x, int y);                                                         //! FE8U = (0x0802E1F0+1)
Trap* GetSpecificTrapAt(int x, int y, int type);                                       //! FE8U = (0x0802E24C+1)

Trap* AddTrap(int x, int y, int type, int ext);                                        //! FE8U = (0x0802E2B8+1)
Trap* AddTrapExt(int x, int y, int type, int ext1, int ext24, int ext3, int ext5);     //! FE8U = (0x0802E2E0+1)
void  RemoveTrap(Trap* trap);                                                          //! FE8U = (0x0802E2FC+1)
void  AddFireTrap(int x, int y, int startCountDown, int resetCountDown);               //! FE8U = (0x0802E314+1)
void  AddGasTrap(int x, int y, int direction, int startCountDown, int resetCountDown); //! FE8U = (0x0802E330+1)
void  AddArrowTrap(int x, int startCountDown, int resetCountDown);                     //! FE8U = (0x0802E350+1)
void  AddTrap8(int x, int y);                                                          //! FE8U = (0x0802E388+1)
void  AddTrap9(int x, int y, int ext);                                                 //! FE8U = (0x0802E398+1)

// map changes -> map.h?
void  ApplyTrapMapChanges();                                                           //! FE8U = (0x0802E430+1)

void  ApplyMapChangesById(int id);                                                     //! FE8U = (0x0802E58C+1)
void  AddMapChange(int id);                                                            //! FE8U = (0x0802E5F8+1)
void  RemoveMapChange(int id);                                                         //! FE8U = (0x0802E60C+1)

void  HideIfUnderRoof(Unit*);                                                          //! FE8U = (0x0802E660+1)
void  UpdateUnitsUnderRoof();                                                          //! FE8U = (0x0802E690+1)

// other file?
void  AddGorgonEggTrap(int a, int b, int c, int d, int e);                             //! FE8U = (0x08037928+1)
Trap* AddBallista(int x, int y, int itemIndex);                                        //! FE8U = (0x08037A04+1)

#pragma long_calls_off

#endif // GBAFE_TRAPS_H
