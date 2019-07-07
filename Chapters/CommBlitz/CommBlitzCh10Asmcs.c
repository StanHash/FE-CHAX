
#include "gbafe.h"

/*
 * ASMC parameters:
 *   s1 = address of unit definition (one UNIT)
 *   s2 = char id of rescuer
 *
 * ASMC result:
 *   sC = 1 if the rescued unit was sucessfully loaded, 0 otherwise
 */
void ASMC_LoadRescued(struct Proc* proc)
{
	const struct UnitDefinition* udef = ((const struct UnitDefinition*)(gEventSlot[1]));

	struct Unit* rescuer = GetUnitByCharId(gEventSlot[2]);

	if (UNIT_IS_VALID(rescuer) && !(rescuer->state & (US_UNAVAILABLE | US_RESCUING | US_RESCUED)))
	{
		struct Unit* rescued = LoadUnit(udef);

		if (rescued)
		{
			UnitRescue(rescuer, rescued);

			gEventSlot[0xC] = TRUE;
			return;
		}
	}

	gEventSlot[0xC] = FALSE;
}

static
int IsRescuingChar(int charId)
{
	extern const u8 gRescuingChars[];

	for (const u8* it = gRescuingChars; *it; ++it)
	{
		if (*it == charId)
			return TRUE;
	}

	return FALSE;
}

int ASME_CheckRescues(void* unk)
{
	for (unsigned i = FACTION_RED + 1; i < FACTION_RED + 0x40; ++i)
	{
		struct Unit* unit = GetUnit(i);

		if (!UNIT_IS_VALID(unit))
			continue;

		if (unit->state & (US_UNAVAILABLE | US_RESCUED))
			continue;

		if (IsRescuingChar(unit->pCharacterData->number) && !(unit->state & US_RESCUING))
			return TRUE;
	}

	return FALSE;
}
