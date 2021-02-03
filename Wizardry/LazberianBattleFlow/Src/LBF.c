
#include "gbafe.h"

void ClearBattleHits(void);
void BattleGetBattleUnitOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender);

static int CheckNonMiss(struct BattleHit const* begin, struct BattleHit const* end);
static int GetPlus(struct BattleUnit const* attacker, struct BattleUnit const* defender);
static struct BattleHit const* IncBattleHitIt(struct BattleHit const* it);

extern u32 const gPlusItemAttribute;
extern u8 const gLbfBattleHitLen;

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

static int CheckNonMiss(struct BattleHit const* begin, struct BattleHit const* end)
{
    for (struct BattleHit const* it = begin; it != end; it = IncBattleHitIt(it))
    {
        if (it->attributes & BATTLE_HIT_ATTR_MISS)
            continue;

        return TRUE;
    }

    return FALSE;
}

static int GetPlus(struct BattleUnit const* attacker, struct BattleUnit const* defender)
{
    return (attacker->weaponAttributes & gPlusItemAttribute) && (attacker->battleSpeed > defender->battleSpeed) ? 2 : 1;
}

static struct BattleHit const* IncBattleHitIt(struct BattleHit const* it)
{
    return it + (gLbfBattleHitLen/4);
}
