#include "gbafe.h"

int GetStatIncrease(int growth) {
	int stat = 0;
	
	while ((growth -= NextRN_100()) >= 0)
		stat++;
	
	return stat;
}
