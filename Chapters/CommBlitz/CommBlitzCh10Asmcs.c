
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

/*
 * ASMC parameters:
 *   s2 = address of trap data
 */
void ASMC_LoadTraps(struct Proc* proc)
{
	// TODO: add to CLib
	extern void LoadTrapData(const void* data);

	const void* data = (const void*)(gEventSlot[2]);

	if (data)
		LoadTrapData(data);
}

/*
 * ASMC result:
 *   sC = game total turn count up until now
 */
void ASMC_GetGameTotalTurnCount(struct Proc* proc)
{
	// TODO: add to CLib
	extern int GetGameTotalTurnCount(void);

	gEventSlot[0xC] = 1 + GetGameTotalTurnCount();
}

void ASMC_FixActiveUnit(struct Proc* proc)
{
	gActiveUnit->state = gActiveUnit->state &~ (US_HIDDEN | US_HAS_MOVED | US_HAS_MOVED_AI);
}

void ASMC_FixDanGoyle(struct Proc* proc)
{
	extern const u8 gDanGoyleCharId;

	struct Unit* unit = GetUnitByCharId(gDanGoyleCharId);

	if (unit)
		unit->movBonus = 1;
}

static
int IsRescuingChar(int charId)
{
	extern const u8 gFleetCharList[];

	for (const u8* it = gFleetCharList; *it; ++it)
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

int AiTransformMoveIntoTalk(const u8* charId)
{
	AiUpdateDecision(
		AI_DECISION_TALK, 0,
		gActiveUnit->index,
		GetUnitByCharId(*charId)->index,
		0xFF);

	return TRUE;
}
