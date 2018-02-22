#ifndef GBAFE_ANIMHANDLE_H
#define GBAFE_ANIMHANDLE_H

// this may or may not correspond to the ap.c file in the proto
// (the order matches, but may be dialog interpreter too, which is just before)

#include <stdint.h>

#include "common.h"

// I call those "TCS" in my doc -Stan

typedef struct _AnimHandle AnimHandle;
typedef struct _AnimHandle TCStruct;

struct _AnimHandle {
	void* pDefinition;      // 00 | word  | Pointer to ROMTCS
	void* pFrameData;       // 04 | word  | Pointer to Frame Data Ref (from ROMTCS)
	void* pAnimDataStart;   // 08 | word  | Pointer to Current Anim Data (Start, where you go back on loop)
	void* pAnimDataCurrent; // 0C | word  | Pointer to Current Anim Data (Cursor)
	void* pCurrentFrameOAM; // 10 | word  | Pointer to Current Frame/OAM Data
	void* pCurrentRotScale; // 14 | word  | Pointer to Current Rot/Scale Data
	uint16_t cycleClock;    // 18 | short | Cycle Timer
	uint16_t cycleTimeStep; // 1A | short | Cycle Time Step (0x100 is one frame)
	uint16_t cycleOffset;   // 1C | short | Sub frame time offset or something
	uint16_t objectDepth;   // 1E | short | OAM Index?
	uint8_t gfxNeedsUpdate; // 20 | byte  | bool defining whether gfx needs update
	uint8_t rotScaleIndex;  // 21 | byte  | Rotation/Scale OAM Index
	uint16_t OAMBase;       // 22 | short | OAM Extra Data (Tile Index Root & OAM2 Stuff)
	void* pGraphics;        // 24 | word  | Gfx Pointer
};

#pragma long_calls
#pragma long_calls_off

#endif // GBAFE_ANIMHANDLE_H
