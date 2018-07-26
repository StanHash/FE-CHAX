#ifndef GBAFE_ACTION_H
#define GBAFE_ACTION_H

#include "common.h"

typedef struct ActionData ActionData;

struct ActionData {
	// unknown stuff (sometimes RNs are pushed here) (maybe an union?)
	u16 _u00[6];
	
	u8 subjectIndex;
	u8 targetIndex;
	
	u8 xMove;
	u8 yMove;
	
	u8 moveCount;
	
	u8 actionIndex;
	
	// maybe from this onwards it's an union?
	
	u8 itemSlotIndex;
	
	u8 xOther;
	u8 yOther;
	
	u8 trapType;
};

enum {
	// 0x00?
	UNIT_ACTION_WAIT = 0x01,
	UNIT_ACTION_COMBAT = 0x02,
	UNIT_ACTION_STAFF = 0x03,
	UNIT_ACTION_DANCE = 0x04,
	// 0x05?
	UNIT_ACTION_STEAL = 0x06,
	UNIT_ACTION_SUMMON = 0x07,
	UNIT_ACTION_SUMMON_DK = 0x08,
	UNIT_ACTION_RESCUE = 0x09,
	UNIT_ACTION_DROP = 0x0A,
	UNIT_ACTION_TAKE = 0x0B,
	UNIT_ACTION_GIVE = 0x0C,
	// 0x0D?
	UNIT_ACTION_TALK = 0x0E,
	UNIT_ACTION_SUPPORT = 0x0F,
	UNIT_ACTION_VISIT = 0x10,
	UNIT_ACTION_SEIZE = 0x11,
	UNIT_ACTION_DOOR = 0x12,
	// 0x13?
	UNIT_ACTION_CHEST = 0x14,
	UNIT_ACTION_PICK = 0x15,
	// 0x16?
	UNIT_ACTION_SHOPPED = 0x17,
	// 0x18?
	UNIT_ACTION_ARENA = 0x19,
	UNIT_ACTION_USE_ITEM = 0x1A,
	// 0x1B?
	// 0x1C?
	UNIT_ACTION_TRADED = 0x1D,
	// 0x1E?
	// 0x1F?
	// 0x20?
	UNIT_ACTION_RIDE_BALLISTA = 0x21,
	UNIT_ACTION_EXIT_BALLISTA = 0x22,
};

extern struct ActionData gActionData; //! FE8U = (0x0203A958)

#pragma long_calls
#pragma long_calls_off

#endif // GBAFE_ACTION_H
