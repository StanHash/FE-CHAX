#ifndef GBAFE_RANDOM_H
#define GBAFE_RANDOM_H

#include "common.h"

extern u16 gRNState[];
extern u16 gOtherRN;

#pragma long_calls

int NextRN(void); //! FE8U = (0x08000B88+1)
void LoadRNState(const u16[3]); //! FE8U = (0x08000C34+1)
void SaveRNState(u16[3]); //! FE8U = (0x08000C4C+1)
int NextRN_100(void); //! FE8U = (0x08000C64+1)
int NextRN_N(int); //! FE8U = (0x08000C80+1)
int Roll1RN(int); //! FE8U = (0x08000CA0+1)
int Roll2RN(int); //! FE8U = (0x08000CB8+1)

#pragma long_calls_off

#endif // GBAFE_FUNCTIONS_RANDOM_H
