#include "gbafe.h"

typedef int(*TradeCheckFunc)(const struct Unit*, const struct Unit*);

extern const TradeCheckFunc TradeCheckList[];

int CanUnitTradeWith(const struct Unit* self, const struct Unit* other) {
	const TradeCheckFunc* it = &TradeCheckList[0];

	while (*it)
		if (!((*it++)(self, other)))
			return FALSE;

	return TRUE;
}
