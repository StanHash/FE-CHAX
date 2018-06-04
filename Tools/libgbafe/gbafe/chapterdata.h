#ifndef GBAFE_CHAPTERDATA_H
#define GBAFE_CHAPTERDATA_H

#include <stdint.h>

#include "common.h"

typedef struct ChapterData ChapterData;

struct ChapterData {
	/* 00 */ unsigned _u00;
	/* 04 */ unsigned _u04;
	/* 08 */ unsigned goldAmount;
	/* 0C */ uint8_t  saveSlotIndex;
	/* 0D */ uint8_t  visionRange;
	/* 0E */ uint8_t  chapterIndex;
	/* 0F */ uint8_t  currentPhase;
	/* 10 */ uint16_t turnNumber;
	/* 12 */ uint8_t  xCursorSaved;
	/* 13 */ uint8_t  yCursorSaved;
	/* 14 */ uint8_t  chapterStateBits;
	/* 15 */ uint8_t  weather;
	/* 16 */ uint16_t supportGainTotal;
	/* 18 */ uint8_t  _u18;
	/* 19 */ uint8_t  _u19;
	/* 1A */ uint8_t  _u1A;
	/* 1B */ uint8_t  mode;
};

extern ChapterData gChapterData; //! FE8U = (0x202BCF0)

#pragma long_calls
#pragma long_calls_off

#endif // GBAFE_CHAPTERDATA_H
