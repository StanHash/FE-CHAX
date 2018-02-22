#ifndef GBAFE_ACTION_H
#define GBAFE_ACTION_H

#include <stdint.h>

#include "common.h"

typedef struct _ActionData ActionData;

struct _ActionData {
	// unknown stuff (sometimes RNs are pushed here) (maybe an union?)
	uint16_t _u00[6];
	
	uint8_t subjectIndex;
	uint8_t targetIndex;
	
	uint8_t xMove;
	uint8_t yMove;
	
	uint8_t moveCount;
	
	uint8_t actionIndex;
	
	// maybe from this onwards it's an union?
	
	uint8_t itemSlotIndex;
	
	uint8_t xOther;
	uint8_t yOther;
	
	uint8_t trapType;
};

extern ActionData action_data; //! FE8U = (0x0203A958)

#endif // GBAFE_ACTION_H
