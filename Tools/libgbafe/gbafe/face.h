#ifndef GBAFE_FACE_H
#define GBAFE_FACE_H

#include "proc.h"
#include "hiobj.h"

typedef struct PortraitData      PortraitData;
typedef struct FaceProc          FaceProc;
typedef struct FaceGfxDefinition FaceGfxDefinition;

struct PortraitData {
	/* 00 */ const void*     pPortraitGraphics;
	/* 04 */ const void*     pMiniPortraitGraphics;
	/* 08 */ const uint16_t* pPortraitPalette;
	/* More */
};

struct FaceProc {
	/* 00 */ PROC_HEADER;

	/* 2C */ const PortraitData* pPortraitData;
	/* 30 */ uint32_t displayBits;
	/* 34 */ int16_t xPosition;
	/* 36 */ int16_t yPosition;
	/* 38 */ const ObjData* pObjData;
	/* 3C */ uint16_t tileData;
	/* 3E */ uint16_t portraitIndex;
	/* 40 */ uint8_t faceSlotIndex;
	/* 41 */ uint8_t objectDepth;

	/* 44 */ Proc* _pu44Proc;
	/* 48 */ Proc* pEyeWinkProc;
};

struct FaceGfxDefinition {
	const void* pTileRoot;
	uint16_t paletteIndex;
};

#pragma long_calls

const PortraitData* GetPortraitData(int portraitId);                   //! FE8U = 0x8005515
void                InitFaces(void);                                           //! FE8U = 0x8005529
void                SetFaceGfxConfig(const FaceGfxDefinition[4]);               //! FE8U = 0x8005545
FaceProc*           NewFace(int faceId, int portraitId, int x, int y, int idk); //! FE8U = 0x800563D
void                EndFace(FaceProc*);                             //! FE8U = 0x8005739
void                EndFaceById(int index);                               //! FE8U = 0x8005759
int                 ShouldPortraitBeSmol(int portraitId);                       //! FE8U = 0x8005C25

#pragma long_calls_off

#endif // GBAFE_FACE_H
