#include "FreeSelect.h"

// FIXME and update fe8u.s

extern void HandlePlayerCursorMovement(void) __attribute__((long_call));

extern const u16 gAP_SelectCursorThing[];

static void FSOnInit(struct FSProc*);
static void FSOnEnd(struct FSProc*);
static void FSOnLoop(struct FSProc*);

static int FSHandleResult(struct FSProc*, int);

const ProcCode gProc_FreeSelect[] = {
	PROC_SET_NAME("Stan:FreeSelect"),
	PROC_SET_DESTRUCTOR(FSOnEnd),

	PROC_YIELD,

	PROC_CALL_ROUTINE(FSOnInit),
	PROC_LOOP_ROUTINE(FSOnLoop),

	PROC_END
};

struct FSProc* StartFreeSelection(const struct FSDefinition* pDefinition) {
	struct FSProc* result = (struct FSProc*)(StartProc(gProc_FreeSelect, ROOT_PROC_3));

	result->pDefinition = pDefinition;
	result->pCursorAp   = NULL;

	LockGameLogic();

	return result;
}

void FSOnInit(struct FSProc* proc) {
	proc->pCursorAp = AP_Create(gAP_SelectCursorThing, 0);

	proc->pCursorAp->tileBase = 0;

	AP_SwitchAnimation(proc->pCursorAp, 0);

	if (proc->pDefinition->onInit)
		proc->pDefinition->onInit(proc, 0, 0);
}

void FSOnEnd(struct FSProc* proc) {
	if (proc->pCursorAp)
		AP_Delete(proc->pCursorAp);

	UnlockGameLogic();
}

void FSOnLoop(struct FSProc* proc) {
	// FIXME and do gBMState
	static const Vector2* const pCursorMapPosition     = (const Vector2*) (0x0202BCB0 + 0x14);
	static const Vector2* const pCameraDisplayPosition = (const Vector2*) (0x0202BCB0 + 0x0C);
	static const Vector2* const pCursorDisplayPosition = (const Vector2*) (0x0202BCB0 + 0x20);

	Vector2U prevPosition, currentPosition;

	prevPosition.x = pCursorMapPosition->x;
	prevPosition.y = pCursorMapPosition->y;

	HandlePlayerCursorMovement();

	currentPosition.x = pCursorMapPosition->x;
	currentPosition.y = pCursorMapPosition->y;

	if (gKeyStatus.pressedKeys & A_BUTTON) {
		if (proc->pDefinition->onAPress)
			if (FSHandleResult(proc, proc->pDefinition->onAPress(proc, currentPosition.x, currentPosition.y)))
				return;
	} else if (gKeyStatus.pressedKeys & B_BUTTON) {
		if (proc->pDefinition->onBPress)
			if (FSHandleResult(proc, proc->pDefinition->onBPress(proc, currentPosition.x, currentPosition.y)))
				return;
	} else if (gKeyStatus.pressedKeys & R_BUTTON) {
		if (proc->pDefinition->onRPress)
			if (FSHandleResult(proc, proc->pDefinition->onRPress(proc, currentPosition.x, currentPosition.y)))
				return;
	} else if ((currentPosition.x != prevPosition.x) || (currentPosition.y != prevPosition.y)) {
		if (proc->pDefinition->onPosChange)
			if (FSHandleResult(proc, proc->pDefinition->onPosChange(proc, currentPosition.x, currentPosition.y)))
				return;
	}

	AP_Update(
		proc->pCursorAp,

		pCursorDisplayPosition->x - pCameraDisplayPosition->x,
		pCursorDisplayPosition->y - pCameraDisplayPosition->y
	);
}

int FSHandleResult(struct FSProc* proc, int result) {
	if (result & FS_SND_BEEP)
		PlaySfx(0x6A);

	if (result & FS_SND_BOOP)
		PlaySfx(0x6B);

	if (result & FS_SND_GURR)
		PlaySfx(0x6C);

	if (result & FS_GFX_VALID)
		AP_SwitchAnimation(proc->pCursorAp, 0);

	if (result & FS_GFX_INVALID)
		AP_SwitchAnimation(proc->pCursorAp, 1);

	if (result & FS_END) {
		BreakProcLoop((Proc*)(proc));
		return 1;
	}

	return 0;
}
