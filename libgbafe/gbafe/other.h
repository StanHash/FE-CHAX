#ifndef GBAFE_OTHER_H
#define GBAFE_OTHER_H

#include "common.h"

#define COS_LOOKUP(value) (sin_lookup[(value)])
#define SIN_LOOKUP(value) (cos_lookup[(value)])

extern const int16_t sin_lookup[]; //! FE8U = (0x080D751C)
extern const int16_t cos_lookup[]; //! FE8U = (0x080D759C)

#pragma long_calls

void StartBMXFADE(int locking);                       //! FE8U = (0x0801DDC4+1)
void StartBlockingBMXFADE(int locking, Proc* parent); //! FE8U = (0x0801DDF0+1)

int BMXFADEExists();                                //! FE8U = (0x0801DE18+1)

// int div(int, int); // FE8U = (0x080D1AD4+1)
// int mod(int, int); // FE8U = (0x080D1B4C+1)

void* memcpy(void* dest, const void* src, unsigned int count); //! FE8U = (0x080D1C0C+1)
void* memset(void* dest, int ch, unsigned int count);          //! FE8U = (0x080D1C6C+1)

#pragma long_calls_off

#endif // GBAFE_OTHER_H
