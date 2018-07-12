#include "gbafe.h"

typedef void (*BCFunc) (struct BattleUnit*, struct BattleUnit*);

extern const BCFunc BattleStatCalcList[];

void ComputeBattleStats(struct BattleUnit* subject, struct BattleUnit* target) {
	const BCFunc* it = &BattleStatCalcList[0];

	while (*it)
		(*it++)(subject, target);
}
