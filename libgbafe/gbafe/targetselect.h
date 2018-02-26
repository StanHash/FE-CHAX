#ifndef GBAFE_TARGETSELECT_H
#define GBAFE_TARGETSELECT_H

#include "proc.h"

typedef struct _TargetEntry               TargetEntry;
typedef struct _TargetSelectionDefinition TargetSelectionDefinition;
typedef struct _TargetSelectionProc       TargetSelectionProc;

struct _TargetEntry {
	uint8_t x, y;
	uint8_t unitIndex;
	uint8_t trapIndex;

	TargetEntry* next;
	TargetEntry* prev;
};

struct _TargetSelectionDefinition {
	void(*onInit)(TargetSelectionProc*);
	void(*onEnd)(TargetSelectionProc*);
	
	void(*onInitTarget)(TargetSelectionProc*, TargetEntry*);

	void(*onSwitchIn)(TargetSelectionProc*, TargetEntry*);
	void(*onSwitchOut)(TargetSelectionProc*, TargetEntry*);

	int(*onAPress)(TargetSelectionProc*, TargetEntry*);
	int(*onBPress)(TargetSelectionProc*, TargetEntry*);
	int(*onRPress)(TargetSelectionProc*, TargetEntry*);
};

struct _TargetSelectionProc {
	Proc header;

	const TargetSelectionDefinition* pDefinition;
	TargetEntry* pCurrentEntry;
	
	uint8_t stateBits;

	int(*onAPressOverride)(TargetSelectionProc*, TargetEntry*);
};

enum _TargetSelectionEffect {
	TSE_NONE      = 0x00,

	TSE_DISABLE   = 0x01, // (for one frame, probably useful for blocking)
	TSE_END       = 0x02,
	TSE_PLAY_BEEP = 0x04,
	TSE_PLAY_BOOP = 0x08,
	TSE_CLEAR_GFX = 0x10,
	TSE_END_FACE0 = 0x20
};

extern Vector2U     gTargetPosition;  //! FE8U = 0x0203DDE8
extern TargetEntry  gTargetArray[];   //! FE8U = 0x0203DDEC
extern unsigned int gTargetArraySize; //! FE8U = 0x0203E0EC

#pragma long_calls

void InitTargets(void);                                   //! FE8U = 0x804F8A5
void AddTarget(int x, int y, uint8_t unit, uint8_t trap); //! FE8U = 0x804F8BD
void LinkTargets(void);                                   //! FE8U = 0x804F911

void TargetSelection_GetRealCursorPosition(TargetSelectionProc*, int* xTarget, int* yTarget); //! FE8U = 0x804F959
void TargetSelection_Loop(TargetSelectionProc*); //! FE8U = 0x804F96D

TargetSelectionProc* NewTargetSelection(const TargetSelectionDefinition*); //! FE8U = 0x804FA3D
TargetSelectionProc* NewTargetSelection_Specialized(const TargetSelectionDefinition*, int(*)(TargetSelectionProc*, TargetEntry*)); //! FE8U = 0x804FAA5

void EndTargetSelection(TargetSelectionProc*); //! FE8U = 0x804FAB9

// TargetSelection_HandleMoveInput //! FE8U = 0x804FAED
// TargetSelection_HandleSelectInput //! FE8U = 0x804FB65

unsigned int GetFarthestTargetIndex(void);  //! FE8U = 0x804FBFD
TargetEntry* LinkTargetsOrdered(void);      //! FE8U = 0x804FC5D
TargetEntry* GetLinkedTargetList(void);     //! FE8U = 0x804FD01
TargetEntry* GetFirstTargetPointer(void);   //! FE8U = 0x804FD11
unsigned int GetTargetListSize(void);       //! FE8U = 0x804FD29
TargetEntry* GetTarget(unsigned int index); //! FE8U = 0x804FD35

#pragma long_calls_off

#define ClearTargets InitTargets

#define StartTargetSelection NewTargetSelection
#define StartTargetSelectionExt NewTargetSelection_Specialized

#endif // GBAFE_TARGETSELECT_H
