#ifndef GBAFE_TRAPS_H
#define GBAFE_TRAPS_H

#include <stdint.h>

#include "common.h"

typedef struct _TrapData TrapData;

typedef struct _Trap Trap;

struct _TrapData {
	uint8_t type;
	uint8_t data[5];
};

struct _Trap {
	uint8_t xPosition;
	uint8_t yPosition;
	
	uint8_t type;
	
	uint8_t data[5];
};

#endif // GBAFE_TRAPS_H
