
#include "gbafe.h"

void ClearBattleHits(void);
void BattleGetBattleUnitOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender);

static int CheckNonMiss(const struct BattleHit* begin, const struct BattleHit* end);
static int GetPlus(const struct BattleUnit* attacker, const struct BattleUnit* defender);

extern const unsigned gPlusItemAttribute;

//! Should replace function at FE8U:0802AED0
void BattleUnwind(void)
{
	ClearBattleHits();

	struct BattleUnit* attacker;
	struct BattleUnit* defender;

	BattleGetBattleUnitOrder(&attacker, &defender);

	gBattleHitIterator->info |= BATTLE_HIT_INFO_BEGIN;

	int attackerPlus = GetPlus(attacker, defender);
	int defenderPlus = GetPlus(defender, attacker);

	while (attackerPlus > 0)
	{
		struct BattleHit* it = gBattleHitIterator;

		if (BattleGenerateRoundHits(attacker, defender))
			break;

		if (defenderPlus > 0)
		{
			if (!CheckNonMiss(it, gBattleHitIterator))
			{
				gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_RETALIATE;

				if (BattleGenerateRoundHits(defender, attacker))
					break;
			}

			defenderPlus--;
		}

		attackerPlus--;
	}

	gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
}

static int CheckNonMiss(const struct BattleHit* begin, const struct BattleHit* end)
{
	for (const struct BattleHit* it = begin; it != end; ++it)
	{
		if (it->attributes & BATTLE_HIT_ATTR_MISS)
			continue;

		return TRUE;
	}

	return FALSE;
}

static int GetPlus(const struct BattleUnit* attacker, const struct BattleUnit* defender)
{
	return (attacker->weaponAttributes & gPlusItemAttribute) && (attacker->battleSpeed > defender->battleSpeed) ? 2 : 1;
}
