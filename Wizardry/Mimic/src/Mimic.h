#ifndef MIMIC_H
#define MIMIC_H

#include "gbafe.h"

// Mimic.c
int IsUnitMimic(const struct Unit* unit);
int AreNeitherUnitMimics(const struct Unit* self, const struct Unit* other);

// MimicSelection.c
void MimicSetupTargets(const struct Unit* unit);
void MimicStartTargetSelection(void);

// <Events>
extern const unsigned MimicCharacterId;
extern const unsigned MimicCommandActionId;
extern const unsigned MimicUnitSelectHelpTextId;
extern const unsigned MimicPositionSelectHelpTextId;

#endif // MIMIC_H
