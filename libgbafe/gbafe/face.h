#ifndef GBAFE_FACE_H
#define GBAFE_FACE_H

#include "proc.h"

typedef struct _FaceProc          FaceProc;
typedef struct _FaceGfxDefinition FaceGfxDefinition;

struct _FaceProc {
	PROC_FIELDS

	// ????
};

struct _FaceGfxDefinition {
	const void* pTileRoot;
	uint16_t paletteIndex;
};

#pragma long_calls

// TODO: more
void      SetupFaceGfxData(const FaceGfxDefinition[4]);
FaceProc* NewFace(int entry, int faceId, int x, int y, int idk); //! FE8U = 0x800563D
void      DeleteFaceByPointer(FaceProc*); //! FE8U = 0x8005739
void      DeleteFaceByIndex(int index); //! FE8U = 0x8005759

#pragma long_calls_off

#endif // GBAFE_FACE_H
