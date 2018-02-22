#include "utility.h"

static int sign(int value) { return (value>>31) - (-value>>31); }

int MoveMoveUnitTowards(MoveUnitState* moveunit, int x, int y, int speed) {
	x = x << 8;
	y = y << 8;
	
	int xSign = sign(x - moveunit->xPosition);
	int ySign = sign(y - moveunit->yPosition);
	
	moveunit->xPosition += xSign << speed;
	moveunit->yPosition += ySign << speed;
	
	return xSign | ySign;
}
