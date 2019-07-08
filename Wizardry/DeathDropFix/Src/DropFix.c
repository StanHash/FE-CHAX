
#include "gbafe.h"

int CheckDeathDrop(struct Unit* unit)
{
	int x = unit->xPos;
	int y = unit->yPos;

	struct Unit* other = GetUnit(unit->rescueOtherUnit);

	if (other && ((gMapHidden[y][x] & 1) || !CanUnitCrossTerrain(other, gMapTerrain[y][x])))
	{
		UnitDrop(unit, x, y);
		UnitKill(other);

		return TRUE;
	}

	return FALSE; // a hidden unit is occupying this position
}
