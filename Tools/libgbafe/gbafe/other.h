#ifndef GBAFE_OTHER_H
#define GBAFE_OTHER_H

#include "common.h"

#define COS_LOOKUP(value) (gSinLookup[(value)])
#define SIN_LOOKUP(value) (gCosLookup[(value)])

extern const int16_t gSinLookup[]; //! FE8U = (0x080D751C)
extern const int16_t gCosLookup[]; //! FE8U = (0x080D759C)

extern const ProcCode gProc_GoBackToUnitMenu[];
extern const ProcCode gProc_PlayerPhase[];

#pragma long_calls

void SetCursorMapPosition(int x, int y);                   //! FE8U = 0x8015BBD

int EnsureCameraOntoPosition(Proc* parent, int x, int y);  //! FE8U = 0x8015E0D

void DisplayMoveRangeGraphics(unsigned param);             //! FE8U = 0x801DA99
void HideMoveRangeGraphics(void);                          //! FE8U = 0x801DACD

void StartBMXFADE(int locking);                            //! FE8U = (0x0801DDC4+1)
void StartBlockingBMXFADE(int locking, Proc* parent);      //! FE8U = (0x0801DDF0+1)

int BMXFADEExists();                                       //! FE8U = (0x0801DE18+1)

unsigned GetPhaseAbleUnitCount(unsigned phase);            //! FE8U = (0x08024CED+1)

Proc* NewBottomHelpText(Proc*, const char*);               //! FE8U = 0x8035709
void DeleteEach6CBB(void);                                 //! FE8U = 0x8035749
int BBExists(void);                                        //! FE8U = 0x8035759

// int div(int, int); // FE8U = (0x080D1AD4+1)
// int mod(int, int); // FE8U = (0x080D1B4C+1)

void* memcpy(void* dest, const void* src, unsigned int count); //! FE8U = (0x080D1C0C+1)
void* memset(void* dest, int ch, unsigned int count);          //! FE8U = (0x080D1C6C+1)

#pragma long_calls_off

#define StartBottomHelpText NewBottomHelpText
#define EndBottomHelpText DeleteEach6CBB
#define IsBottomHelpTextActive BBExists

#endif // GBAFE_OTHER_H
