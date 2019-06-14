#ifndef AF_INCLUDED
#define AF_INCLUDED

#include "gbafe.h"
#include "assert.h"

// types

enum
{
	AF_FORGED_MAX = 0x20,
	AF_REF_MAX    = AF_FORGED_MAX + 0x10,

	AF_NAME_BYTES = 13,

	AF_MIGHT_BITS = 4,
	AF_HIT_BITS = 4,
	AF_CRIT_BITS = 4,

	AF_MIGHT_STEP = 1,
	AF_HIT_STEP = 5,
	AF_CRIT_STEP = 3,

	AF_ERR_NONE = 0,
	AF_ERR_BADSAVE = 1,
	AF_ERR_REFOVERFLOW = 2,
	AF_ERR_FORGEOVERFLOW = 3,
};

struct ForgedItemData
{
	u8 item;
	char name[AF_NAME_BYTES];

	u16 rawMight : AF_MIGHT_BITS;
	u16 rawHit   : AF_HIT_BITS;
	u16 rawCrit  : AF_CRIT_BITS;
	u16 gcBit    : 1;
};

struct AfDataGlobal
{
	static_assert(sizeof(struct ForgedItemData) <= 0x10, "ForgedItemData takes more space than expected!");

	// <items> here hold the actual forge data (which is: the underlying item id, the forge name and the stat bonuses)
	// They don't hold the forge's *uses* for efficiency reasons.
	struct ForgedItemData items[AF_FORGED_MAX];

	// <refs> here hold forge references: they hold the id of the forged item, and the item uses.
	// The reason refs hold uses is because item copying is trivial, but use depletion should only reflect one copy
	// So each time an item loses an use, we need to copy the item's data before updating its uses
	// Using an extra layer of references here is more efficient as it requires less space overall
	u16 refs[AF_REF_MAX];

	// A forged item (as in: the value that's held by inventories) holds the global forged item id (constant), and a ref id
};

// functions

// Forged item functions
int AfMakeForgedItem(int baseItem);
void AfSetMightBonus(int item, int mightLevel);
void AfSetHitBonus(int item, int hitLevel);
void AfSetHitBonus(int item, int critLevel);
void AfSetName(int item, const char* name);

// Important core system functions
void AfInit(void);
void AfGc(void);

// objects

extern struct AfDataGlobal* const gpAfData;
extern u32* const gpAfErr;

extern const u8 gAfItemId;

#endif // AF_INCLUDED
