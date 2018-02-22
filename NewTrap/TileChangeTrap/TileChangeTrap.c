#include "gbafe.h"

extern const int SINGLE_TILE_CHANGE_TRAP_TYPE;

#define TRAP_CHANGE_X(trap) ((trap)->data[0])
#define TRAP_CHANGE_Y(trap) ((trap)->data[1])

#define GET_TRAP_CHANGE_TILE(trap) (((trap)->data[3]) | (((trap)->data[4]) << 8))
#define SET_TRAP_CHANGE_TILE(trap, tile) (trap)->data[3] = tile & 0xFF; (trap)->data[4] = (tile >> 8) & 0xFF

void ChangeSingleTile(int x, int y, int tileID, Proc* parent);
void ChangeSingleTileExt(int x, int y, int tileID, int display, Proc* parent);

static void ApplySingleTileChange(int x, int y, int tileID) {
	map_raw_tile_index[y][x] = tileID;
}

static Trap* FindTileChangeTrap(int x, int y) {
	Trap* trap = trap_array;
	
	while (trap->type) {
		if (trap->type == SINGLE_TILE_CHANGE_TRAP_TYPE)
			if ((TRAP_CHANGE_X(trap) == x) && (TRAP_CHANGE_Y(trap) == y))
				return trap;
		
		++trap;
	}
	
	return 0;
}

void ApplyTrapMapChanges() { // should replace 0802E430
	Trap* trap = trap_array;
	
	while (trap->type) {
		if (trap->type == 3)
			ApplyMapChangesById(trap->data[0]);
		else if (trap->type == 6)
			ApplyMapChangesById((trap->data[0] ? trap->yPosition : trap->xPosition)); // yee idk either
		else if (trap->type == SINGLE_TILE_CHANGE_TRAP_TYPE)
			ApplySingleTileChange(TRAP_CHANGE_X(trap), TRAP_CHANGE_Y(trap), GET_TRAP_CHANGE_TILE(trap));
		
		++trap;
	}
}

void ChangeSingleTile(int x, int y, int tileID, Proc* parent) {
	ChangeSingleTileExt(x, y, tileID, (parent ? 1 : 0), parent);
}

void ChangeSingleTileExt(int x, int y, int tileID, int display, Proc* parent) {
	Trap* trap = FindTileChangeTrap(x, y);
	int oldTile = tileID+1; // all that's important is that it's different for now
	
	if (!trap) {
		trap = AddTrap(-1, -1, SINGLE_TILE_CHANGE_TRAP_TYPE, 0);
		
		TRAP_CHANGE_X(trap) = x;
		TRAP_CHANGE_Y(trap) = y;
	} else {
		oldTile = GET_TRAP_CHANGE_TILE(trap);
	}
	
	if (tileID != oldTile) {
		if (display)
			InitMapChangeGraphics();
		
		SET_TRAP_CHANGE_TILE(trap, tileID);
		ApplySingleTileChange(TRAP_CHANGE_X(trap), TRAP_CHANGE_Y(trap), GET_TRAP_CHANGE_TILE(trap));
		
		RefreshTileMaps();
		UpdateUnitsUnderRoof();
		DrawTileGraphics();
		
		if (display) {
			if (parent)
				StartBlockingBMXFADE(1, parent);
			else
				StartBMXFADE(1);
		}
	}
}

// void ChangeTest1(Proc* ee) {
	// ChangeSingleTile(4, 1, 0,  0);
// }

// void ChangeTest2(Proc* ee) {
	// ChangeSingleTile(5, 1, 4,  ee);
// }

// void ChangeTest3(Proc* ee) {
	// ChangeSingleTile(6, 1, 8,  0);
// }

// void ChangeTest4(Proc* ee) {
	// ChangeSingleTile(7, 1, 12, ee);
// }
