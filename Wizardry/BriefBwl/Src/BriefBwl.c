
#include "gbafe.h"

enum
{
	MAX_BWL_CHAR = 0x46
};

struct NewBwlData
{
	/* 00+00bit */ unsigned losses     : 8;
	/* 00+08bit */ unsigned wins       : 10;
	/* 00+18bit */ unsigned battles    : 12;
	/* 00+30bit */ unsigned deathSkirm : 1;
	/* 00+31bit */ unsigned : 0; /* needs to stay 0 for logic to work */

	/* 04+00bit */ unsigned deathLoc   : 6;
	/* 04+06bit */ unsigned : 0; /* do what you want with those 2 bits */

	/* there's 11 full bytes free starting at +05 */

	/* 08 */ u32 pad[2];
};

extern struct NewBwlData gBWLDataStorage[];

static inline
struct NewBwlData* GetBwlData(unsigned charId)
{
	charId = charId - 1;
	return gBWLDataStorage + charId;
}

static inline
int HasBwl(unsigned charId)
{
	return (charId < MAX_BWL_CHAR) && GetCharacterData(charId)->affinity;
}

void BBwl_SetDeathStats(int charId, int killerId, int cause)
{
	if (HasBwl(charId))
	{
		struct NewBwlData* bwl = GetBwlData(charId);

		if (GetBattleMapType() == 2) // skirmish
		{
			bwl->deathSkirm = TRUE;
			bwl->deathLoc = gGMData.units[0].location;
		}
		else
		{
			bwl->deathSkirm = FALSE;
			bwl->deathLoc = gChapterData.chapterIndex;
		}
	}
}

int BBwl_GetTotalBattles(void)
{
	int result = 0;

	for (unsigned i = 1; i < MAX_BWL_CHAR; ++i)
		result += GetBwlData(i)->battles;

	return result;
}

int BBwl_GetTotalWins(void)
{
	int result = 0;

	for (unsigned i = 1; i < MAX_BWL_CHAR; ++i)
		result += GetBwlData(i)->wins;

	return result;
}

int BBwl_GetTotalLosses(void)
{
	int result = 0;

	for (unsigned i = 1; i < MAX_BWL_CHAR; ++i)
		result += GetBwlData(i)->losses;

	return result;
}

int BBwl_GetFavor(int charId)
{
	if (HasBwl(charId))
	{
		struct NewBwlData* bwl = GetBwlData(charId);
		int result = bwl->battles * 4 + bwl->wins * 0x10 - bwl->losses * 0x80;

		if (result > 4000)
			return 4000;

		if (result < 0)
			return 0;

		return result;
	}

	return 0;
}
