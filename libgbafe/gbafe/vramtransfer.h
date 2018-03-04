#ifndef GBAFE_VRAMTRANSFER_H
#define GBAFE_VRAMTRANSFER_H

#include "common.h"

typedef struct _ObjData ObjData;
typedef struct _OAMData OAMData;

struct _OAMData {
	uint16_t oam1, oam2, oam3;
};

struct _ObjData {
	uint16_t count;
	OAMData  data[]; // NOTE: C99+ only
};

#pragma long_calls

void ClearTileRegistry(void);                                             //! FE8U = 0x8001FE1
void RegisterTileGraphics(const void* source, void* target, size_t size); //! FE8U = 0x8002015
void RegisterFillTile(uint32_t value, void* target, size_t size);         //! FE8U = 0x8002055
void SyncRegisteredTiles(void);                                           //! FE8U = 0x8002089

void InitOAMSplice(int loOAMSize);                                        //! FE8U = 0x80020FD

void SyncHiOAM(void);                                                     //! FE8U = 0x8002139
void SyncLoOAM(void);                                                     //! FE8U = 0x800217D

void WriteOAMRotScaleData(size_t index, int pa, int pb, int pc, int pd);  //! FE8U = 0x80021B1

size_t GetLoOAMSize(void);                                                //! FE8U = 0x800224D

// ARM
void PushToHiOAM(uint16_t xBase, uint16_t yBase, const ObjData* data, uint16_t tileBase); //! FE8U = 0x8002BB9
void PushToLoOAM(uint16_t xBase, uint16_t yBase, const ObjData* data, uint16_t tileBase); //! FE8U = 0x8002BCD

void ClearOAMBuffer(uint16_t* buffer, size_t size);                                       //! FE8U = 0x80D7499

#pragma long_calls_off

// decomp compat
#define CallARM_PushToSecondaryOAM PushToHiOAM
#define CallARM_PushToPrimaryOAM PushToLoOAM

#endif // GBAFE_VRAMTRANSFER_H
