#ifndef GBAFE_ANIMINTERPRETER_H
#define GBAFE_ANIMINTERPRETER_H

#include "common.h"

typedef struct AnimationInterpreter AnimationInterpreter;
typedef struct AnimationInterpreter AIStruct;

struct AnimationInterpreter {
	/* 00 */ u16 state;
	/* 02 */ s16 xPosition;
	/* 04 */ s16 yPosition;
	/* 06 */ u16 delayTimer;
	/* 08 */ u16 oam2base;
	/* 0A */ u16 drawLayerPriority;
	/* 0C */ u16 state2;
	/* 0E */ u16 nextRoundId;
	/* 10 */ u16 state3;
	/* 12 */ u8 currentRoundType;
	/* 13 */ u8 frameIndex;

	/* 14 */ u8 queuedCommandCount;
	/* 15 */ u8 commandQueue[0xB];

	/* 20 */ const void* pCurrentFrame;
	/* 24 */ const void* pStartFrame;
	/* 28 */ const void* _unk28;
	/* 2C */ const void* _unk2C;
	/* 30 */ const void* pStartObjData; // aka "OAM data"

	/* 34 */ struct AnimationInterpreter* pPrev;
	/* 38 */ struct AnimationInterpreter* pNext;

	/* 40 */ const void* _unk40;
	/* 44 */ const void* _unk44;
};

extern u8 gBattleCharacterIndices[2]; //! FE8U = 0x203E190

#pragma long_calls

int GetAISSubjectId(const struct AnimationInterpreter*); //! FE8U = 0x805A16D
int IsBatteRoundTypeAMiss(u16); //! FE8U = 0x805A185
int GetBattleAnimRoundType(int index); //! FE8U = 0x8058A0D

void StartEkrNamewinAppear(int, int, int);

#pragma long_calls_off

#endif // GBAFE_ANIMINTERPRETER_H
