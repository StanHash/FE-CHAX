#include "gbafe.h"

// thanks to SEVA for the formula
// (3*(A^2)) - ((2*(A^3))/100) = TrueRN*100

int RollRNForHit(int chance) {
	int rn = NextRN_100();
	
	int rn2 = rn*rn;
	int rn3 = rn*rn*rn;
	
	return (chance*10000) > ((300*rn2) - (2*rn3));
}
