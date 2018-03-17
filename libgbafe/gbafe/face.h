#ifndef GBAFE_FACE_H
#define GBAFE_FACE_H

#include "proc.h"

typedef struct _PortraitData      PortraitData;
typedef struct _FaceProc          FaceProc;
typedef struct _FaceGfxDefinition FaceGfxDefinition;

struct _PortraitData {
	/* 00 */ const void*     pPortraitGraphics;
	/* 04 */ const void*     pMiniPortraitGraphics;
	/* 08 */ const uint16_t* pPortraitPalette;
	/* More */
};

struct _FaceProc {
	PROC_HEADER;

	// ????
};

struct _FaceGfxDefinition {
	const void* pTileRoot;
	uint16_t paletteIndex;
};

#pragma long_calls

PortraitData* GetPortraitStructPointer(int portraitId);                   //! FE8U = 0x8005515
void          ResetFaces(void);                                           //! FE8U = 0x8005529
void          SetupFaceGfxData(const FaceGfxDefinition[4]);               //! FE8U = 0x8005545
FaceProc*     NewFace(int faceId, int portraitId, int x, int y, int idk); //! FE8U = 0x800563D
void          DeleteFaceByPointer(FaceProc*);                             //! FE8U = 0x8005739
void          DeleteFaceByIndex(int index);                               //! FE8U = 0x8005759
int           ShouldPortraitBeSmol(int portraitId);                       //! FE8U = 0x8005C25

#pragma long_calls_off

#endif // GBAFE_FACE_H
