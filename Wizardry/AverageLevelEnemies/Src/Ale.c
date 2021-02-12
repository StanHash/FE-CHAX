
#include "gbafe.h"

int AleGetBlueAverageLevel(void)
{
    int sum = 0;
    int amt = 0;

    for (int i = 1; i < 0x40; ++i)
    {
        struct Unit* const unit = GetUnit(i);

        if (unit == NULL)
            continue;

        if (unit->pCharacterData == NULL)
            continue;

        if (unit->state & (US_DEAD | US_BIT16))
            continue;

        if (unit->level == 0)
            continue;

        sum += unit->level;

        if (UNIT_ATTRIBUTES(unit) & CA_PROMOTED)
            sum += 20;

        amt += 1;
    }

    return Div(sum, amt);
}

void AleApplyLoadUnitLevel(struct Unit* unit, struct UnitDefinition* def)
{
    int level = AleGetBlueAverageLevel();

    if (UNIT_ATTRIBUTES(unit) & CA_PROMOTED)
        level = level - 20;

    if (level < 0)
        level = 0;

    if (level > 20)
        level = 20;

    unit->level = level;
}
