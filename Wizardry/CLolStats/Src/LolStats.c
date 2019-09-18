#include "gbafe.h"

int NuggetStatIncrease(int growth)
{
	int stat = 0;
	
	while ((growth -= NextRN_100()) >= 0)
		stat++;
	
	return stat;
}

int NuggetAutoleveledStatIncrease(int growth, int level)
{
	int effGrowth = 0;

	// Here we just go for an approx that assumes growth < 100
	// It's probably also wrong but probably good enough

	effGrowth += 10000 * (growth * level + 50); // chances of getting stat 1
	effGrowth += 100 * (growth * growth * level + 5000) / 2; // chances of getting stat 2
	effGrowth += (growth * growth * growth * level + 500000) / 4; // chances of getting stat 3

	return Div(effGrowth, 1000000);
}
