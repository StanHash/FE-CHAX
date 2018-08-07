#ifndef GBAFE_GAME_H
#define GBAFE_GAME_H

// Rename to bm.h? Since it corresponds to bm.c from the proto

#pragma long_calls

int GetGameClock(void); //! FE8U = 0x8000D29

int LockGameLogic(void); //! FE8U = (0x08015360+1)
int UnlockGameLogic(void); //! FE8U = (0x08015370+1)
int GetGameLogicLock(void); //! FE8U = (0x08015380+1)

// TODO: move those elsewhere
void BlockGameGraphicsLogic(void); //! FE8U = 0x8030185
void UnblockGameGraphicsLogic(void); //! FE8U = 0x80301B9

void ClearBG0BG1(void); //! FE8U = 0x804E885

#pragma long_calls_off

#endif // GBAFE_GAME_H
