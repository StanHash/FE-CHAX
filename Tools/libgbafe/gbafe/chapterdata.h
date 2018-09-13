#ifndef GBAFE_CHAPTERDATA_H
#define GBAFE_CHAPTERDATA_H

#include "common.h"

typedef struct ChapterState ChapterState;

struct ChapterState {
	/* 00 */ u32 _u00;
	/* 04 */ u32 _u04;
	/* 08 */ u32 goldAmount;
	/* 0C */ u8 saveSlotIndex;
	/* 0D */ u8 visionRange;
	/* 0E */ u8 chapterIndex;
	/* 0F */ u8 currentPhase;
	/* 10 */ u16 turnNumber;
	/* 12 */ u8 xCursorSaved;
	/* 13 */ u8 yCursorSaved;
	/* 14 */ u8 chapterStateBits;
	/* 15 */ u8 weather;
	/* 16 */ u16 supportGainTotal;
	/* 18 */ u8 playthroughId;
	/* 19 */ u8 _u19;
	/* 1A */ u8 lastUnitListSortType;
	/* 1B */ u8 mode;
	/* 1C */ u8 unk1C[4]; // Weapon type lookup by weapon specifying which character ids may bypass the unusable bit????? (see FE8U:80174AC)

	/* 20 */ char playerName[0x40 - 0x20]; // unused outside of link arena (was tactician name in FE7); Size unknown

	// option byte 1 (of 3)
	u32 unk40_1:1;
	u32 terrainWindowOption:1;
	u32 unitWindowOption:2;
	u32 autocursorOption:1;
	u32 textSpeedOption:2;
	u32 gameSpeedOption:1;

	u32 unk41_1:1;
	u32 muteSfxOption:1;
	u32 unk41_3:5;
	u32 subtitleHelpOption:1;

	u32 unk42_1:1;
	u32 unk42_2:1;
	u32 unk42_3:1;
	u32 unk42_4:1;
	u32 unk42_5:1;
	u32 unk42_6:1;
	u32 unk42_7:1;
	u32 unk42_8:1;

	u32 unk43_1:8;

	u8 unk44[0x48 - 0x44];

	u16 unk48;

	u16 unk4A_1 : 1;
	u16 unk4A_2 : 3;
};

extern struct ChapterState gChapterData; //! FE8U = (0x202BCF0)

#pragma long_calls
#pragma long_calls_off

#endif // GBAFE_CHAPTERDATA_H
