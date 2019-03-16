#include "gbafe.h"

// TODO: add the libgbafe {

struct BattleQuoteEntry {
	/* +00 */ u16 charAId;
	/* +02 */ u16 charBId;
	/* +04 */ u8  chapterId;
	/* +06 */ u16 eid;

	/* +08 */ u16 textId;
	/* +0C */ const u16* pScene;
};

struct DeathQuoteEntry {
	/* +00 */ u16 charId;
	/* +02 */ u8  modeId;
	/* +03 */ u8  chapterId;
	/* +04 */ u16 eid;

	/* +06 */ u16 textId;
	/* +08 */ const u16* pScene;
};

int BattleIsTriangleAttack(void) __attribute__((long_call)); //! FE8U = 0x802CE89
int CheckEventId(u16 id) __attribute__((long_call));

// }

extern const struct BattleQuoteEntry* const gpMainBattleQuoteTable;
extern const struct DeathQuoteEntry* const gpMainDeathQuoteTable;

const struct BattleQuoteEntry* const gChapterBattleQuoteTables[0x80] = {0};
const struct DeathQuoteEntry* const gChapterDeathQuoteTables[0x80] = {0};

static int BattleQuoteCheck(const struct BattleQuoteEntry* entry, u16 chrA, u16 chrB) {
	// Check chapter id

	if (entry->chapterId != 0xFF) {
		if ((entry->chapterId == 0xFE) && !BattleIsTriangleAttack())
			return FALSE;
		else if (entry->chapterId != gChapterData.chapterIndex)
			return FALSE;
	}

	// Check eid

	if (CheckEventId(entry->eid))
		return FALSE;

	// Check characters

	if ((!entry->charAId || (entry->charAId == chrA)) && (!entry->charBId || (entry->charBId == chrB)))
		return TRUE;

	if ((!entry->charAId || (entry->charAId == chrB)) && (!entry->charBId || (entry->charBId == chrA)))
		return TRUE;

	return FALSE;
}

static int DeathQuoteCheck(const struct DeathQuoteEntry* entry, u16 chr) {
	// Check mode

	if ((entry->modeId != 0xFF) && (entry->modeId != gChapterData.mode))
		return FALSE;

	// Check chapter id

	if ((entry->chapterId != 0xFF) && (entry->chapterId != gChapterData.chapterIndex))
		return FALSE;

	// Check eid

	if (CheckEventId(entry->eid))
		return FALSE;

	// Check character

	if (entry->charId != chr)
		return FALSE;

	return TRUE;
}

const struct BattleQuoteEntry* NuggetBattleQuoteEntry(u16 chrA, u16 chrB) {
	const struct BattleQuoteEntry* it = gChapterBattleQuoteTables[gChapterData.chapterIndex];

	if (it)
		for (; it->charAId != 0xFFFF; ++it)
			if (BattleQuoteCheck(it, chrA, chrB))
				return it;

	for (it = gpMainBattleQuoteTable; it->charAId != 0xFFFF; ++it)
		if (BattleQuoteCheck(it, chrA, chrB))
			return it;

	return NULL;
}

const struct DeathQuoteEntry* NuggetDeathQuoteEntry(u16 chr) {
	const struct DeathQuoteEntry* it = gChapterDeathQuoteTables[gChapterData.chapterIndex];

	if (it)
		for (; it->charId != 0xFFFF; ++it)
			if (DeathQuoteCheck(it, chr))
				return it;

	for (it = gpMainDeathQuoteTable; it->charId != 0xFFFF; ++it)
		if (DeathQuoteCheck(it, chr))
			return it;

	return NULL;
}
