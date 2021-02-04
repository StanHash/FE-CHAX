
#include "gbafe.h"

//! Should Replace fe8u:0802AF90
int BattleGetFollowUpOrder(struct BattleUnit** attacker, struct BattleUnit** defender)
{
    if (gBattleActor.battleSpeed > 250)
        return FALSE;

    if (gBattleActor.battleSpeed >= gBattleTarget.battleSpeed*2)
    {
        *attacker = &gBattleActor;
        *defender = &gBattleTarget;

        if (GetItemWeaponEffect((*attacker)->weaponBefore) == 3)
            return FALSE;

        return TRUE;
    }

    if (gBattleTarget.battleSpeed >= gBattleActor.battleSpeed*2)
    {
        *attacker = &gBattleTarget;
        *defender = &gBattleActor;

        if (GetItemWeaponEffect((*attacker)->weaponBefore) == 3)
            return FALSE;

        return TRUE;
    }

    return FALSE;
}