#ifndef FREE_SELECT_H
#define FREE_SELECT_H

#include "gbafe.h"

struct FSProc;

typedef int(*FSHandlerFunc)(struct FSProc*, int, int);

struct FSDefinition {
	FSHandlerFunc onInit;
	FSHandlerFunc onEnd;
	FSHandlerFunc onAPress;
	FSHandlerFunc onBPress;
	FSHandlerFunc onRPress;
	FSHandlerFunc onPosChange;
};

struct FSProc {
	PROC_HEADER;

	const struct FSDefinition* pDefinition;
	APHandle* pCursorAp;
};

enum FSResultCode {
	// TODO: FS_PAUSE, FS_RESUME?

	FS_END         = 0x02, // Ends selection when set
	FS_SND_BEEP    = 0x04, // Plays beep sound (sound Id 0x6A) when set
	FS_SND_BOOP    = 0x08, // Plays boop sound (sound Id 0x6B) when set
	FS_SND_GURR    = 0x10, // Plays gurr sound (sound Id 0x6C) when set
	FS_GFX_VALID   = 0x20, // Sets cursor gfx to valid
	FS_GFX_INVALID = 0x40, // Sets cursor gfx to invalid
};

struct FSProc* StartFreeSelection(const struct FSDefinition* pDefinition);

#endif // FREE_SELECT_H