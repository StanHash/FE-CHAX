#include "utility.h"

static int sign(int value) { return (value>>31) - (-value>>31); }
static int abs(int value) { return value < 0 ? -value : value; }
static int sqr(int value) { return value * value; }

int MoveMoveUnitTowards(MoveUnitState* moveunit, int x, int y, int speed) {
	x = x << 8;
	y = y << 8;

	int xSign = sign(x - moveunit->xPosition);
	int ySign = sign(y - moveunit->yPosition);

	moveunit->xPosition += xSign << speed;
	moveunit->yPosition += ySign << speed;

	return xSign | ySign;
}

// needs testing
int MoveUnitUnitTowards2(MoveUnitProc* moveunit, int x, int y, int speed) {
	int xDistance = (0x100 * x) - moveunit->xPosition;
	int yDistance = (0x100 * y) - moveunit->yPosition;

	int distance = Sqrt(sqr(abs(xDistance)) + sqr(abs(yDistance)));

	int factor = Div(speed, distance);

	int xStep = (xDistance * factor) / 0x10;
	int yStep = (yDistance * factor) / 0x10;

	if (abs(xDistance) < abs(xStep) || abs(yDistance) < abs(yStep)) {
		moveunit->xPosition = x * 0x100;
		moveunit->yPosition = y * 0x100;

		return FALSE; // movement ended (destination reached)
	} else {
		moveunit->xPosition += xStep;
		moveunit->yPosition += yStep;

		return TRUE; // the adventure continues...
	}
}
