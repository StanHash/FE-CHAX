#ifndef VES_H
#define VES_H

#include "gbafe.h"

#pragma long_calls

void  MakeUIWindowTileMap_BG0BG1(int x, int y, int width, int height, int style);
char* String_GetFromIndex(short index);

#pragma long_calls_off

#define VES_PREFIX "Stan:ValentianExpShare"

struct VESProc {
	PROC_HEADER;
};

struct VESUnitDisplayProc {
	PROC_HEADER;

	unsigned char lineId;

	const Unit* pUnit;
	struct VESProc* pVESMain;

	TextHandle text;
};

struct VESUnitDisplayProc* VESStartUnitDisplay(struct VESProc* proc, unsigned lineId, const Unit* unit);

#endif // VES_H
