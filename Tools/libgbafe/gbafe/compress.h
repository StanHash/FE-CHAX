#ifndef GBAFE_COMPRESS_H
#define GBAFE_COMPRESS_H

#include "common.h"

extern uint8_t gGenericBuffer[0x2000]; //! FE8U = 0x2020188

#pragma long_calls

void CopyString(char* target, const char* source);                 //! FE8U = 0x8012EC1
void CopyDirect(const void* source, void* target);                 //! FE8U = 0x8012EDD
void CopyDataWithPossibleUncomp(const void* source, void* target); //! FE8U = 0x8012F51
size_t FilterR0ForRawCopy(const void*);                            //! FE8U = 0x8012F91

#pragma long_calls_off

// better names
#define CopyNoCompData CopyDirect
#define Decompress CopyDataWithPossibleUncomp
#define GetNoCompDataSize FilterR0ForRawCopy

#endif // GBAFE_COMPRESS_H
