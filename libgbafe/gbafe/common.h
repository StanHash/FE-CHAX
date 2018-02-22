#ifndef GBAFE_COMMON_H
#define GBAFE_COMMON_H

#include <stdint.h>

typedef struct _Vector2  Vector2;
typedef struct _Vector2U Vector2U;

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
	//              = 0x00400000,
	//              = 0x00800000,
	//              = 0x01000000,
	//              = 0x02000000,
	//              = 0x04000000,
	//              = 0x08000000,
	//              = 0x10000000,
	//              = 0x20000000,
	//              = 0x40000000,
	//              = 0x80000000,
	
	US_DUMMY
};

enum _CAttributes {
	CA_NONE                  = 0x00000000,
	CA_MOUNTEDAID            = 0x00000001,
	CA_CANTO                 = 0x00000002,
	CA_STEAL                 = 0x00000004,
	CA_LOCKPICK              = 0x00000008,
	CA_DANCE                 = 0x00000010,
	CA_PLAY                  = 0x00000020,
	CA_CRITBONUS             = 0x00000040,
	CA_BALLISTAE             = 0x00000080,
	CA_PROMOTED              = 0x00000100,
	CA_SUPPLY                = 0x00000200,
	CA_MOUNTED               = 0x00000400,
	CA_WYVERN                = 0x00000800,
	CA_PEGASUS               = 0x00001000,
	CA_LORD                  = 0x00002000,
	CA_FEMALE                = 0x00004000,
	CA_BOSS                  = 0x00008000,
	CA_LOCK_1                = 0x00010000,
	CA_LOCK_2                = 0x00020000,
	CA_LOCK_3                = 0x00040000, // Dragons or Monster depending of game
	CA_MAXLEVEL10            = 0x00080000,
	CA_UNSELECTABLE          = 0x00100000,
	CA_TRIANGLEATTACK_PEGASI = 0x00200000,
	CA_TRIANGLEATTACK_ARMORS = 0x00400000,
	//                       = 0x00800000,
	//                       = 0x01000000,
	CA_LETHALITY             = 0x02000000,
	//                       = 0x04000000,
	CA_SUMMON                = 0x08000000,
	CA_LOCK_4                = 0x10000000,
	CA_LOCK_5                = 0x20000000,
	CA_LOCK_6                = 0x40000000,
	CA_LOCK_7                = 0x80000000,

	CA_DUMMY
};

enum _IAttributes {
	IA_NONE           = 0x00000000,
	
	IA_WEAPON         = 0x00000001,
	IA_MAGIC          = 0x00000002,
	IA_STAFF          = 0x00000004,
	IA_UNBREAKABLE    = 0x00000008,
	IA_UNSELLABLE     = 0x00000010,
	IA_BRAVE          = 0x00000020,
	IA_MAGICDAMAGE    = 0x00000040,
	IA_UNCOUNTERABLE  = 0x00000080,
	IA_REVERTTRIANGLE = 0x00000100,
	IA_HAMMERNE       = 0x00000200, // Defined as Hammerne effect in FE6 Nightmare module, but as ??? in FE7 & FE8.
	IA_LOCK_3         = 0x00000400, // Dragons or Monster depending of game
	IA_LOCK_1         = 0x00000800,
	IA_LOCK_2         = 0x00001000,
	IA_LOCK_0         = 0x00002000, // King in FE6
	IA_NEGATE_FLYING  = 0x00004000,
	IA_NEGATE_CRIT    = 0x00008000,
	IA_UNUSABLE       = 0x00010000,
	IA_NEGATE_DEFENSE = 0x00020000,
	IA_LOCK_4         = 0x00040000,
	IA_LOCK_5         = 0x00080000,
	IA_LOCK_6         = 0x00100000,
	IA_LOCK_7         = 0x00200000,
	//                = 0x00400000,
	//                = 0x00800000,
	//                = 0x01000000,
	//                = 0x02000000,
	//                = 0x04000000,
	//                = 0x08000000,
	//                = 0x10000000,
	//                = 0x20000000,
	//                = 0x40000000,
	//                = 0x80000000,

	IA_DUMMY
};

struct _Vector2 {
	int16_t x, y;
};

struct _Vector2U {
	uint16_t x, y;
};

#endif // GBAFE_COMMON_H
