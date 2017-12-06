#ifndef GBAFE_COMMON_H
#define GBAFE_COMMON_H

#include <stdint.h>

typedef struct _Vector2 Vector2;

enum _UnitState {
	US_NONE         = 0x00000000,
	US_HIDDEN       = 0x00000001,
	US_UNSELECTABLE = 0x00000002,
	US_DEAD         = 0x00000004,
	US_NOT_DEPLOYED = 0x00000008,
	US_RESCUING     = 0x00000010,
	US_RESCUED      = 0x00000020,
	US_CANTOING     = 0x00000040,
	US_UNDER_A_ROOF = 0x00000080,
	//              = 0x00000100,
	//              = 0x00000200,
	//              = 0x00000400,
	US_IN_BALLISTA  = 0x00000800,
	US_DROP_ITEM    = 0x00001000,
	US_GROWTH_BOOST = 0x00002000,
	US_SOLOANIM_1   = 0x00004000,
	US_SOLOANIM_2   = 0x00008000,
	//              = 0x00010000,
	//              = 0x00020000,
	//              = 0x00040000,
	//              = 0x00080000,
	//              = 0x00100000,
	//              = 0x00200000,
	
	US_DUMMY
};

enum _CAttributes {
	CA_NONE = 0x00000000,
	
	CA_DUMMY
};

struct _Vector2 {
	int16_t x, y;
};

#endif // GBAFE_COMMON_H
