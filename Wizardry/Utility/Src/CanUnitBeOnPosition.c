#include "gbafe.h"

int CanUnitBeOnPosition(struct Unit* unit, int x, int y)
{
	if (x < 0 || y < 0)
		return FALSE; // position out of bounds

	if (x >= gMapSize.x || y >= gMapSize.y)
		return FALSE; // position out of bounds

	if (gMapUnit[y][x])
		return FALSE; // a unit is occupying this position

	if (gMapHidden[y][x] & 1)
		return FALSE; // a hidden unit is occupying this position

	return CanUnitCrossTerrain(unit, gMapTerrain[y][x]);
}
