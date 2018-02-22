#ifndef GBAFE_MOVEUNIT_H
#define GBAFE_MOVEUNIT_H

#include <stdint.h>

#include "6C.h"
#include "unit.h"
#include "TCS.h"

typedef struct _MoveUnitProc MoveUnitProc;
typedef struct _MoveUnitProc MoveUnit6C;

typedef struct _MoveUnitExtraData MoveUnitExtraData;

struct _MoveUnitExtraData {
	uint8_t index;
	
	uint8_t  objPaletteIndex;
	uint16_t objTileIndex;
	
	uint8_t commandCount;
	uint8_t commands[63];
	
	void* _;
	MoveUnitProc* moveunit;
};

struct _MoveUnitProc {
	Proc header;
	
	// should be padded to 0x2C
	
	Unit* pUnit;
	AnimHandle* pTCS;
	MoveUnitExtraData* pExtraData;
	void* pVRAM;
	
	uint8_t spriteFrameIndex;
	uint8_t _3D;
	uint8_t cameraFollow;
	uint8_t state;
	uint8_t _40;                 // 40 | byte  | Related to Movement Speed? (0 initially) (80797D4 sets this to 1) (80797DC sets this to 0)
	uint8_t classIndex;          // 41 | byte  | Class Index / Map Sprite Entry (The fact that it is used as class index too annoys me)
	uint8_t currentAnimIndex;    // 42 | byte  | Current Direction byte? (0xB initially) (Stored in routine at 08078694)
	uint8_t _43;                 // 43 | byte  | Something? (Initialized at 0xFE/-2 if another MOVEUNIT instance exists, 0 otherwise. Incremented in routine at 8078D6C)
	uint8_t _44;                 // 44 | byte  | Related to Movement Speed? (0 initially) (8079B10 sets this to 1)
	// uint8_t _45;              // should be padded to 0x46
	uint16_t objectPriorityBits; // 46 | short | OAM priority mask (0x800 initially)
	uint16_t _48;                // 48 | short | When Moving, Incremented by Movement Speed, added to positions and then finally cleared. State 3 (3F = 3) Decrements it and sets State to 2 when reaches 0
	uint16_t _4A;                // 4A | short | If bit 7 is set, it will not play any sound/create any 6C_89A2938 while moving the unit
	
	int16_t xPosition;           // 4C | short | x*256
	int16_t yPosition;           // 4E | short | y*256
	int16_t xOffset;             // 50 | short | added to x? (0 initially)
	int16_t yOffset;             // 52 | short | added to y? (0 initially)
	
	Proc* pBlendTimerProc;       // 54 | word  | blend timer thing 6C pointer
};

#endif // GBAFE_MOVEUNIT_H
