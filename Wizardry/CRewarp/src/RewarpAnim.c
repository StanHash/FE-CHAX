#include "Rewarp.h"

// FIXME
static const Vector2* const pCameraDisplayPosition = (const Vector2*) (0x0202BCB0 + 0x0C);

void New6C_SomethingFlashy(Unit* unit, int x, int y) __attribute__((long_call));
#define StartUnitWarpRing New6C_SomethingFlashy

void StartStarExplosionEffect(int x, int y) __attribute__((long_call));
void StartStarImplosionEffect(int x, int y) __attribute__((long_call));

struct RCAProc {
	PROC_HEADER;

	Unit*         pUnit;
	MoveUnitProc* pMoveUnit;

	int xTarget;
	int yTarget;
};

static int  RCAInitCamera(struct RCAProc*);
static void RCAInitAnim(struct RCAProc*);
static void RCARing(struct RCAProc*);
static void RCAFlashBegin(struct RCAProc*);
static void RCAStarsBegin(struct RCAProc*);
static void RCAHide(struct RCAProc*);
static void RCAMove(struct RCAProc*);
static int  RCAMoveCamera(struct RCAProc*);
static void RCAStarsEnd(struct RCAProc*);
static void RCAShow(struct RCAProc*);
static void RCAAnimBack(struct RCAProc*);

static const ProcInstruction sProc_RewarpAnim[] = {
	PROC_SET_NAME("Stan:CRewarp:Anim"),

	PROC_CALL_ROUTINE(LockGameLogic),
	PROC_YIELD,

	PROC_CALL_ROUTINE_2(RCAInitCamera),
	PROC_CALL_ROUTINE(RCAInitAnim),

	PROC_CALL_ROUTINE(RCARing),
	PROC_SLEEP(10),

	PROC_CALL_ROUTINE(RCAFlashBegin),
	PROC_SLEEP(30),

	PROC_CALL_ROUTINE(RCAStarsBegin),
	PROC_SLEEP(2),

	PROC_CALL_ROUTINE(RCAHide),
	PROC_SLEEP(8),

	PROC_CALL_ROUTINE(RCAMove),
	PROC_SLEEP(30),

	PROC_CALL_ROUTINE_2(RCAMoveCamera),

	PROC_CALL_ROUTINE(RCAStarsEnd),
	PROC_SLEEP(40),

	PROC_CALL_ROUTINE(RCARing),
	PROC_SLEEP(10),

	PROC_CALL_ROUTINE(RCAShow),
	PROC_SLEEP(2),

	PROC_CALL_ROUTINE(RCAAnimBack),

	PROC_SLEEP(40),

	PROC_CALL_ROUTINE(UnlockGameLogic),

	PROC_END
};

void StartRewarpEffect(Unit* unit, int xTarget, int yTarget) {
	struct RCAProc* proc = (struct RCAProc*) StartProc(sProc_RewarpAnim, ROOT_PROC_3);

	EndAllMoveUnits();
	HideUnitSMS(unit);

	proc->pUnit     = unit;
	proc->pMoveUnit = StartMoveUnitForUnit(unit);

	proc->xTarget = xTarget;
	proc->yTarget = yTarget;
}

int RCAInitCamera(struct RCAProc* proc) {
	return !EnsureCameraOntoPosition(
		(Proc*)(proc),

		proc->pUnit->xPos,
		proc->pUnit->yPos
	);
}

void RCAInitAnim(struct RCAProc* proc) {
	BeginMoveUnitActionAnimation(proc->pMoveUnit);
}

void RCARing(struct RCAProc* proc) {
	StartUnitWarpRing(
		proc->pUnit,

		// It actually doesn't seem to even use the given position,
		// but it's passed in vanilla some might as well

		proc->pUnit->xPos,
		proc->pUnit->yPos
	);
}

void RCAFlashBegin(struct RCAProc* proc) {
	static const void(*StartUnitWarpFlash)(MoveUnitProc*, int) = (void(*)(MoveUnitProc*, int))(0x8079804+1);

	StartUnitWarpFlash(proc->pMoveUnit, 0);
}

void RCAStarsBegin(struct RCAProc* proc) {
	StartStarExplosionEffect(
		8 + proc->pUnit->xPos * 16 - pCameraDisplayPosition->x,
		8 + proc->pUnit->yPos * 16 - pCameraDisplayPosition->y
	);
}

void RCAHide(struct RCAProc* proc) {
	HideMoveUnit(proc->pMoveUnit);
}

void RCAMove(struct RCAProc* proc) {
	SetMoveUnitDisplayPosition(
		proc->pMoveUnit,

		16 * (proc->pUnit->xPos = proc->xTarget),
		16 * (proc->pUnit->yPos = proc->yTarget)
	);
}

int RCAMoveCamera(struct RCAProc* proc) {
	return !EnsureCameraOntoPosition(
		(Proc*)(proc),

		proc->xTarget,
		proc->yTarget
	);
}

void RCAStarsEnd(struct RCAProc* proc) {
	StartStarImplosionEffect(
		8 + proc->pUnit->xPos * 16 - pCameraDisplayPosition->x,
		8 + proc->pUnit->yPos * 16 - pCameraDisplayPosition->y
	);
}

void RCAShow(struct RCAProc* proc) {
	ShowMoveUnit(proc->pMoveUnit);
	((void(*)(MoveUnitProc*))(0x8079858+1))(proc->pMoveUnit);
}

void RCAAnimBack(struct RCAProc* proc) {
	((void(*)(MoveUnitProc*))(0x80798E8+1))(proc->pMoveUnit);
}
