#include "Rewarp.h"

extern const int RewarpActionId;

static int RCOnSelect(struct FSProc*, int, int);
static int RCOnCancel(struct FSProc*, int, int);

static const struct FSDefinition sRewarpFSDefinition = {
	.onInit      = NULL,
	.onEnd       = NULL,
	.onAPress    = RCOnSelect,
	.onBPress    = RCOnCancel,
	.onRPress    = NULL,
	.onPosChange = NULL
};

int RCUsability(const struct MenuCommandDefinition* def, int index) {
	return MCA_USABLE;
}

int RCEffect(struct MenuProc* menu, struct MenuCommandProc* command) {
	StartFreeSelection(&sRewarpFSDefinition);
	return ME_DISABLE | ME_END | ME_PLAY_BEEP | ME_CLEAR_GFX;
}

int RCAction(Proc* proc) {
	StartRewarpEffect(
		gActiveUnit,

		gActionData.xMove = gActionData.xOther,
		gActionData.yMove = gActionData.yOther
	);

	return 0;
}

int RCOnSelect(struct FSProc* proc, int x, int y) {
	Text_ResetTileAllocation();
	HideMoveRangeGraphics();
	EndBottomHelpText();

	gActionData.xOther = x;
	gActionData.yOther = y;

	gActionData.unitActionType = RewarpActionId;

	EnsureCameraOntoPosition(
		(Proc*)(proc),

		gActiveUnit->xPos,
		gActiveUnit->yPos
	);

	return FS_END | FS_SND_BEEP;
}

int RCOnCancel(struct FSProc* proc, int x, int y) {
	Text_ResetTileAllocation();
	HideMoveRangeGraphics();
	EndBottomHelpText();

	SetCursorMapPosition(
		gActiveUnit->xPos,
		gActiveUnit->yPos
	);

	ProcStart(gProc_GoBackToUnitMenu, ROOT_PROC_3);

	return FS_END | FS_SND_BOOP;
}
