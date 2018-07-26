#ifndef GBAFE_ANIMHANDLE_H
#define GBAFE_ANIMHANDLE_H

// ap.c

#include "common.h"
#include "proc.h"

// I used to call those "TCS" in my doc -Stan

typedef struct APHandle APHandle;
typedef struct APHandle AnimHandle;
typedef struct APHandle TCStruct;

struct APHandle {
	const void* pDefinition; // Pointer to Definition "ROMTCS"
	const void* pFrameData; // Pointer to Frame Data Ref (from ROMTCS)
	const void* pAnimDataStart; // Pointer to Current Anim Data (Start, where you go back on loop)
	const void* pAnimDataCurrent; // Pointer to Current Anim Data (Cursor)
	const void* pCurrentFrameOAM; // Pointer to Current Frame/OAM Data
	const void* pCurrentRotScale; // Pointer to Current Rot/Scale Data
	u16 cycleClock; // Cycle Timer
	u16 cycleTimeStep;	 // Cycle Time Step (0x100 is one frame)
	u16 cycleOffset; // Sub frame time offset or something
	u16 objectDepth; // OAM Index?
	u8 gfxNeedsUpdate; // bool defining whether gfx needs update
	u8 rotScaleIndex; // Rotation/Scale OAM Index
	u16 OAMBase; // OAM Extra Data (Tile Index Root & OAM2 Stuff)
	const void *pGraphics; // Gfx Pointer
};

extern struct APHandle gAPArray[];

#pragma long_calls

struct APHandle* AP_Create(const void *definition, int depth); //! FE8U = (0x0800927C+1)
void AP_Delete(struct APHandle* ap); //! FE8U = (0x080092A4+1)
void AP_Update(struct APHandle* ap, u16 x, u16 y); //! FE8U = (0x080092BC+1)
void AP_SwitchAnimation(struct APHandle* ap, int index); //! FE8U = (0x08009518+1)
void AP_SetDefinition(struct APHandle* ap, const void* definition); //! FE8U = (0x08009548+1)

struct Proc* APProc_Create(const void* definition, u16 x, u16 y, u16 tileBase, int animIndex, int depth); //! FE8U = (0x08009718+1)
void APProc_SetParameters(struct Proc* apProc, u16 x, u16 y, u16 tileBase); //! FE8U = (0x08009798+1)
void APProc_Delete(struct Proc*); //! FE8U = (0x080097B4+1)
void APProc_DeleteAll(void); //! FE8U = (0x080097C0+1)
int APProc_Exists(void); //! FE8U = (0x080097D1)

#pragma long_calls_off

#endif // GBAFE_ANIMHANDLE_H
