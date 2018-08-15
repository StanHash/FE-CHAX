#include "LTF.h"

void LTF_DisplayPhasePredictions(struct Proc* maptaskProc) {
	static const u8 phaseToPalIdLookup[4] = { 0xC, 0xE, 0xD, 0xF };

	const unsigned maxCounts[4] = {
		LTF_GetPhaseEffectiveUnitCount(UA_BLUE),
		LTF_GetPhaseEffectiveUnitCount(UA_GREEN),
		LTF_GetPhaseEffectiveUnitCount(UA_RED),
		LTF_GetBerserkEffectiveUnitCount(),
	};

	unsigned ableCounts[4] = {
		LTF_GetPhaseAbleUnitCount(UA_BLUE),
		LTF_GetPhaseAbleUnitCount(UA_GREEN),
		LTF_GetPhaseAbleUnitCount(UA_RED),
		LTF_GetBerserkAbleUnitCount(),
	};

	for (unsigned i = 0; i < 5; ++i) {
		if ((ableCounts[0] == 0) && (ableCounts[1] == 0) && (ableCounts[2] == 0) && (ableCounts[3] == 0))
			break;

		unsigned nextPhase = LTF_PredictNextPhase(ableCounts, maxCounts);

		ableCounts[nextPhase]--;

		PushToHiOAM(
			240 - 10, 55 + (i * 10),
			&gOAM_8x8Obj, 3 + (phaseToPalIdLookup[nextPhase] << 12)
		);
	}
}
