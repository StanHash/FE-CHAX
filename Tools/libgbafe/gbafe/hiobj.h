#ifndef GBAFE_HIOBJ_H
#define GBAFE_HIOBJ_H

#include "common.h"
#include "vramtransfer.h"
#include "proc.h"

#pragma long_calls

void HiObjClear(void); //! FE8U = 0x80053A5
void HiObjInsertSafe(int node, uint16_t xBase, uint16_t yBase, const ObjData* pData, uint16_t tileBase); //! FE8U = 0x80053E9
void HiObjInsert(int node, uint16_t xBase, uint16_t yBase, const ObjData* pData, uint16_t tileBase); //! FE8U = 0x8005429
void HiObjProcess(void); //! FE8U = 0x8005459

// TODO
// Proc* StartHiObjProc(Proc* parent, int node, uint16_t xBase, uint16_t yBase, OAMData* pData, uint16_t tileBase);
// void  MoveHiObjProc(Proc* proc, uint16_t xBase, uint16_t yBase);

#pragma long_calls_off

#endif // GBAFE_HIOBJ_H
