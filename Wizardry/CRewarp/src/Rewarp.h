#ifndef REWARP_H
#define REWARP_H

#include "gbafe.h"
#include "FreeSelect.h"

int RCUsability(const struct MenuCommandDefinition* def, int index);
int RCEffect(struct MenuProc* menu, struct MenuCommandProc* command);
int RCAction(Proc* proc);

void StartRewarpEffect(Unit* unit, int xTarget, int yTarget);

#endif // REWARP_H
