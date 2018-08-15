#ifndef LTF_H
#define LTF_H

#include "gbafe.h"

// in LTFCore.c

unsigned LTF_IsUnitReadyToMove(const struct Unit*);
unsigned LTF_IsUnitAbleToMove(const struct Unit*);

unsigned LTF_GetPhaseEffectiveUnitCount(unsigned phase);
unsigned LTF_GetPhaseAbleUnitCount(unsigned phase);

unsigned LTF_GetBerserkEffectiveUnitCount(void);
unsigned LTF_GetBerserkAbleUnitCount(void);

unsigned LTF_PredictNextPhase(const unsigned ableCounts[4], const unsigned maxCounts[4]);

// in LTFPhaseSwitch.c

int LTF_MapMainPhaseSwitch(struct Proc* mapMainProc);

// in LTFDisplay.c

void LTF_DisplayPhasePredictions(struct Proc* maptaskProc);

#endif // LTF_H
