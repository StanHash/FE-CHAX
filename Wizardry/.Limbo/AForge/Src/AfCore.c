
#include "Af.h"

static inline
unsigned AfGetRef(int item)
{
	return gpAfData->refs[item >> 8];
}

static inline
struct ForgedItemData* AfRefGetData(unsigned ref)
{
	return gpAfData->items + (ref & 0xFF) - 1;
}

static inline
unsigned AfRefGetUses(unsigned ref)
{
	return ref >> 8;
}

static inline
unsigned AfRefGetUnderlyingItem(unsigned ref)
{
	return (AfRefGetUses(ref) << 8) + (AfRefGetData(ref)->item);
}

static inline
int SignedTruncate(int value, unsigned bits)
{
	const unsigned shift = (sizeof(int)*8) - bits;
	return (value << shift) >> shift;
}

int AfMakeForgedItem(int baseItem)
{
	// Find slot for forge

	int forgeId = -1;

	for (int i = 0; i < AF_FORGED_MAX; ++i)
	{
		if (gpAfData->items[i].item)
			continue;

		struct ForgedItemData* data = gpAfData->items + i;

		data->item = GetItemIndex(baseItem);

		data->name[0] = 0;
		data->rawMight = 0;
		data->rawHit = 0;
		data->rawCrit = 0;

		forgeId = i;

		break;
	}

	if (forgeId < 0)
	{
		// No forge slot available

		*gpAfErr = AF_ERR_FORGEOVERFLOW;
		return 0;
	}

	// Find slot for initial ref

	int refId = -1;

	for (int i = 0; i < AF_REF_MAX; ++i)
	{
		if (gpAfData->refs[i])
			continue;

		gpAfData->refs[i] = (GetItemUses(baseItem) << 8) + (forgeId + 1);

		refId = i;

		break;
	}

	if (refId < 0)
	{
		// No ref slot available

		*gpAfErr = AF_ERR_REFOVERFLOW;
		return 0;
	}

	// Return new item
	return (refId << 8) + gAfItemId;
}

int AfGetMightBonus(int item)
{
	return AF_MIGHT_STEP * SignedTruncate(AfRefGetData(AfGetRef(item))->rawMight, AF_MIGHT_BITS);
}

int AfGetHitBonus(int item)
{
	return AF_HIT_STEP * SignedTruncate(AfRefGetData(AfGetRef(item))->rawHit, AF_HIT_BITS);
}

int AfGetCritBonus(int item)
{
	return AF_CRIT_STEP * SignedTruncate(AfRefGetData(AfGetRef(item))->rawCrit, AF_CRIT_BITS);
}

void AfSetMightBonus(int item, int mightLevel)
{
	AfRefGetData(AfGetRef(item))->rawMight = mightLevel;
}

void AfSetHitBonus(int item, int hitLevel)
{
	AfRefGetData(AfGetRef(item))->rawHit = hitLevel;
}

void AfSetCritBonus(int item, int critLevel)
{
	AfRefGetData(AfGetRef(item))->rawCrit = critLevel;
}

void AfSetName(int item, const char* name)
{
	unsigned len = strlen(name);

	if (len > AF_NAME_BYTES)
		len = AF_NAME_BYTES;

	char* nameData = AfRefGetData(AfGetRef(item))->name;

	memcpy(nameData, name, len);

	for (unsigned i = len; i < AF_NAME_BYTES; ++i)
		nameData[i] = 0;
}

// for VciInfo::getIndex
int AfGetItemIndex(int item)
{
	return AfRefGetData(AfGetRef(item))->item;
}

char* AfGetItemName(int item)
{
	unsigned ref = AfGetRef(item);
	char* result = GetItemName(AfRefGetUnderlyingItem(ref));

	if (*gpAfErr == AF_ERR_BADSAVE)
	{
		String_CopyTo(result, "AF:BADSAVE");
		return result;
	}

	if (*gpAfErr == AF_ERR_REFOVERFLOW)
	{
		String_CopyTo(result, "AF:REFOFLW");
		return result;
	}

	if (*gpAfErr == AF_ERR_FORGEOVERFLOW)
	{
		String_CopyTo(result, "AF:FRGOFLW");
		return result;
	}

	struct ForgedItemData* data = AfRefGetData(ref);

	if (data->name[0])
	{
		memcpy(result, data->name, AF_NAME_BYTES);
		result[AF_NAME_BYTES] = 0;
	}

	return result;
}

// for VciInfo::getUses
int AfGetItemUses(int item)
{
	return AfRefGetUses(AfGetRef(item));
}

// for VciInfo::getMight
int AfGetItemMight(int item)
{
	return GetItemMight(AfRefGetUnderlyingItem(AfGetRef(item))) + AfGetMightBonus(item);
}

// for VciInfo::getHit
int AfGetItemHit(int item)
{
	return GetItemHit(AfRefGetUnderlyingItem(AfGetRef(item))) + AfGetHitBonus(item);
}

// for VciInfo::getCrit
int AfGetItemCrit(int item)
{
	return GetItemCrit(AfRefGetUnderlyingItem(AfGetRef(item))) + AfGetCritBonus(item);
}

// for VciInfo::getAfterUse
int AfGetItemAfterUse(int item)
{
	if (GetItemAttributes(item) & IA_UNBREAKABLE)
		return item;

	unsigned refId = item >> 8;
	unsigned ref = gpAfData->refs[refId];

	if (AfRefGetUses(ref) == 1)
		return 0;

	// Clone reference, and reduce the clone's uses

	// (We need to clone because GetItemAfterUse shouldn't affect the item itself, but rather return a copy)
	// (This is relevant in battle simulations, where item uses decrease in the bu but aren't reflected to the real unit)
	// (This is why we need a double-reference based setup in the first place)

	for (unsigned i = 0; i < AF_REF_MAX; ++i)
	{
		if (!gpAfData->refs[i])
		{
			gpAfData->refs[i] = ref;
			gpAfData->refs[i] -= 0x100; // reduce uses by one

			return gAfItemId + (i << 8);
		}
	}

	*gpAfErr = AF_ERR_REFOVERFLOW;
	return 0;
}

void AfInit(void)
{
	for (unsigned i = 0; i < AF_FORGED_MAX; ++i)
		gpAfData->items[i].item = 0;

	for (unsigned i = 0; i < AF_REF_MAX; ++i)
		gpAfData->refs[i] = 0;

	*gpAfErr = 0;
}

static inline
void ForAllItems(void(*func)(u16* item, void* user), void* user)
{
	for (unsigned i = 0; i < 0x100; ++i)
	{
		struct Unit* unit = GetUnit(i);

		if (UNIT_IS_VALID(unit))
		{
			for (unsigned j=0; j < UNIT_ITEM_COUNT; ++j)
				if (unit->items[j])
					func(unit->items + j, user);
		}
	}

	u16* convoy = GetConvoyItemArray();

	for (unsigned i = 0; i < CONVOY_ITEM_COUNT; ++i)
		if (convoy[i])
			func(convoy + i, user);
}

struct GcRefState
{
	u16 newRefs[AF_REF_MAX];
	u8 newRefCounter;
};

static inline
void AfGcReref(u16* item, void* user)
{
	struct GcRefState* state = user;

	if (((*item) & 0xFF) == gAfItemId)
	{
		state->newRefs[state->newRefCounter] = AfGetRef(*item);
		*item = gAfItemId + (state->newRefCounter << 8);

		state->newRefCounter++;
	}
}

void AfGc(void)
{
	// Rebuild refs from items (implicitely removing unused ones)
	// (this also updates items accordingly, of course)

	struct GcRefState state = {};

	ForAllItems(AfGcReref, &state);
	CpuCopy16(state.newRefs, gpAfData->refs, sizeof(state.newRefs));

	// Remove unreferenced forge data

	// Set gcBit for used forged items
	for (unsigned i = 0; i < AF_REF_MAX; ++i)
		if (gpAfData->refs[i])
			AfRefGetData(gpAfData->refs[i])->gcBit = TRUE;

	// Clear items without gcBit set
	for (unsigned i = 0; i < AF_FORGED_MAX; ++i)
	{
		if (!gpAfData->items[i].gcBit)
			gpAfData->items[i].item = 0;

		gpAfData->items[i].gcBit = FALSE;
	}
}

void AfSave(void* target, unsigned size)
{
	if (size < sizeof(struct AfDataGlobal))
	{
		*gpAfErr = AF_ERR_BADSAVE;
		return;
	}

	WriteSramFast(gpAfData, target, sizeof(struct AfDataGlobal));
}

void AfLoad(void* source, unsigned size)
{
	if (size < sizeof(struct AfDataGlobal))
	{
		*gpAfErr = AF_ERR_BADSAVE;
		return;
	}

	ReadSramFast(source, gpAfData, sizeof(struct AfDataGlobal));
}

void AfTest(void)
{
	AfInit();

	struct Unit* unit = GetUnit(1);

	if (!unit)
		return;

	int item = AfMakeForgedItem(unit->items[0]);

	if (item)
	{
		AfSetMightBonus(item, 3);
		AfSetHitBonus(item, 5);
		AfSetCritBonus(item, 7);
		AfSetName(item, "A Forge!");

		unit->items[0] = item;
	}

	AfGc();
}
