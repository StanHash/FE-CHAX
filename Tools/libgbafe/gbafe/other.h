#ifndef GBAFE_OTHER_H
#define GBAFE_OTHER_H

#include "common.h"

enum {
	MLV_MOVE_BLUE = 0x01,
	MLV_RANGE_RED = 0x02,
	MLV_RANGE_GREEN = 0x04,
	MLV_RANGE_BLUE = 0x08,
};

#define COS_LOOKUP(value) (gSinLookup[(value)])
#define SIN_LOOKUP(value) (gCosLookup[(value)])

extern const short gSinLookup[]; //! FE8U = (0x080D751C)
extern const short gCosLookup[]; //! FE8U = (0x080D759C)

extern const ProcCode gProc_GoBackToUnitMenu[];
extern const ProcCode gProc_PlayerPhase[];
extern const ProcCode gProc_BG3HSlide[];

extern const u16 gPal_MiscUIGraphics[];

#pragma long_calls

char* GetStringFromIndex(int index); //! FE8U = 0x800A241
char* GetStringFromIndexInBuffer(int index, char* buffer); //! FE8U = 0x800A281

void SetCursorMapPosition(int x, int y); //! FE8U = 0x8015BBD

int EnsureCameraOntoPosition(Proc* parent, int x, int y); //! FE8U = 0x8015E0D

int ApplyUnitAction(struct Proc*); //! FE8U = 0x801849D

void DisplayMoveRangeGraphics(unsigned param); //! FE8U = 0x801DA99
void HideMoveRangeGraphics(void); //! FE8U = 0x801DACD

void StartBMXFADE(int locking); //! FE8U = (0x0801DDC4+1)
void StartBlockingBMXFADE(int locking, Proc* parent); //! FE8U = (0x0801DDF0+1)

int BMXFADEExists(); //! FE8U = (0x0801DE18+1)

unsigned GetPhaseAbleUnitCount(unsigned phase); //! FE8U = (0x08024CED+1)

Proc* StartBottomHelpText(Proc*, const char*); //! FE8U = 0x8035709
void EndBottomHelpText(void); //! FE8U = 0x8035749
int IsBottomHelpTextActive(void); //! FE8U = 0x8035759

void StartStatScreen(const struct Unit* unit, struct Proc* parent); //! FE8U = 0x808894D

// int div(int, int); // FE8U = (0x080D1AD4+1)
// int mod(int, int); // FE8U = (0x080D1B4C+1)

void* memcpy(void* dest, const void* src, unsigned int count); //! FE8U = (0x080D1C0C+1)
void* memset(void* dest, int ch, unsigned int count); //! FE8U = (0x080D1C6C+1)

#pragma long_calls_off

#endif // GBAFE_OTHER_H
