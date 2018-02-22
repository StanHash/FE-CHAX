#ifndef GBAFE_GAME_H
#define GBAFE_GAME_H

// Rename to bm.h? Since it corresponds to bm.c from the proto

#pragma long_calls

int LockGameLogic();    //! FE8U = (0x08015360+1)
int UnlockGameLogic();  //! FE8U = (0x08015370+1)
int GetGameLogicLock(); //! FE8U = (0x08015380+1)

#pragma long_calls_off

#endif // GBAFE_GAME_H
