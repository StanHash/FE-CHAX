#include "Mimic.h"

int IsUnitMimic(const struct Unit* unit) {
	return unit->pCharacterData->number == MimicCharacterId;
}

int AreNeitherUnitMimics(const struct Unit* self, const struct Unit* other) {
	return !IsUnitMimic(self) && !IsUnitMimic(other);
}

int MimicCmdUsability(const struct MenuCommandDefinition* def, int index) {
	// No Mimic command on canto
	if (gActiveUnit->state & US_CANTOING)
		return MCA_NONUSABLE;

	// Check if any existing mimic unit exists
	for (unsigned index = 1; index < 0x40; ++index) {
		const struct Unit* unit = GetUnit(index);

		if (unit && unit->pCharacterData && IsUnitMimic(unit))
			return MCA_NONUSABLE;
	}

	// Check if we can mimic anyone
	if (MimicSetupTargets(gActiveUnit), GetTargetListSize() == 0)
		return MCA_NONUSABLE;

	return MCA_USABLE;
}

int MimicCmdEffect(struct MenuProc* menu, struct MenuCommandProc* command) {
	MimicSetupTargets(gActiveUnit);
	MimicStartTargetSelection();

	return ME_DISABLE | ME_END | ME_PLAY_BEEP | ME_CLEAR_GFX;
}
