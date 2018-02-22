#ifndef GBAFE_ACTION_H
#define GBAFE_ACTION_H

#include <stdint.h>

#include "common.h"

typedef struct _ActionData ActionData;

struct _ActionData {
	uint16_t stuff[6]; // sometimes those are pushed RN states
	
	uint8_t subjectIndex;
	uint8_t targetIndex;
	
	uint8_t xMove;
	uint8_t yMove;
	
	uint8_t moveCount;
	
	uint8_t actionIndex;
	
	uint8_t itemSlotIndex;
	
	uint8_t xOther;
	uint8_t yOther;
	
	uint8_t trapType;
};

#endif // GBAFE_ACTION_H
