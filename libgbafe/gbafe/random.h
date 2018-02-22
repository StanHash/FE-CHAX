#ifndef GBAFE_RANDOM_H
#define GBAFE_RANDOM_H

#include "common.h"

#pragma long_calls

int  NextRN();                     //! FE8U = (0x08000B88+1)
void LoadRNState(const uint16_t*); //! FE8U = (0x08000C34+1)
void SaveRNState(uint16_t*);       //! FE8U = (0x08000C4C+1)
int  NextRN_100();                 //! FE8U = (0x08000C64+1)
int  NextRN_N(int);                //! FE8U = (0x08000C80+1)
int  Roll1RN(int);                 //! FE8U = (0x08000CA0+1)
int  Roll2RN(int);                 //! FE8U = (0x08000CB8+1)

#pragma long_calls_off

#endif // GBAFE_FUNCTIONS_RANDOM_H
