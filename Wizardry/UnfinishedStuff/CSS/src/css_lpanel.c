#include "css_lpanel.h"
#include "css_mugframe_small.h"

static void css_lpanel_init(CSSLPanelProc*);
static void css_lpanel_idle(CSSLPanelProc*);
static void css_lpanel_on_end(CSSLPanelProc*);

static int  css_lpanel_moveup(CSSLPanelProc*);

static void css_lpanel_show_face(CSSLPanelProc*);
static void css_lpanel_hide_face(CSSLPanelProc*);

const ProcCode proc_css_lpanel[] = {
	PROC_SET_NAME(CSS_PREFIX":LPanel"),
	PROC_SET_DESTRUCTOR(css_lpanel_on_end),
	PROC_YIELD,

	PROC_CALL_ROUTINE(css_lpanel_init),

PROC_LABEL(0),
	PROC_LOOP_ROUTINE(css_lpanel_idle),

PROC_LABEL(1),
	PROC_CALL_ROUTINE_2(css_lpanel_moveup),
	PROC_GOTO(0),

	PROC_END
};

/*
static const uint16_t sCssLPanelTextObjLookup[] = {
	0x180, 0x1C0,
	0x186, 0x1C6,
	0x18C, 0x1CC,
	0x186, 0x1C6,
	0x188, 0x1C8,
	0x18A, 0x1CA,
	0x18C, 0x1CC,
	0x18E, 0x1CE
}; //*/

static void css_lpanel_init(CSSLPanelProc* proc) {
	Vector2 stOrigin = {
		proc->origin.x + 4,
		proc->origin.y + 160 - 70
	};

	proc->pStatText     = css_stattext_start(stOrigin);
	proc->faceDisplayed = 0;

	css_mugframe_small_start(gBg0MapBuffer, 0xF0);

	css_lpanel_show_face(proc);

	css_stattext_show(proc->pStatText, 0, (UnitHasMagicRank(gActiveUnit) ? "Mag" : "Str"), gActiveUnit->pow, gActiveUnit->pClassData->maxPow);
	css_stattext_show(proc->pStatText, 2, "Skl", gActiveUnit->skl, gActiveUnit->pClassData->maxSkl);
	css_stattext_show(proc->pStatText, 4, "Spd", gActiveUnit->spd, gActiveUnit->pClassData->maxSpd);
	css_stattext_show(proc->pStatText, 6, "Lck", gActiveUnit->lck, 30);
	css_stattext_show(proc->pStatText, 1, "Def", gActiveUnit->def, gActiveUnit->pClassData->maxDef);
	css_stattext_show(proc->pStatText, 3, "Res", gActiveUnit->res, gActiveUnit->pClassData->maxRes);
	css_stattext_show(proc->pStatText, 5, "Con",
		gActiveUnit->pClassData->baseCon + gActiveUnit->pCharacterData->baseCon + gActiveUnit->conBonus,
		gActiveUnit->pClassData->maxCon
	);
	css_stattext_show(proc->pStatText, 7, "Mov",
		gActiveUnit->pClassData->baseMov + gActiveUnit->movBonus,
		15
	);

	CopyToPaletteBuffer(css_stattext_pal, CSS_PAL_OBJ_STATTEXT * 0x20, 0x20);
}

static void css_lpanel_idle(CSSLPanelProc* proc) {
	if (gKeyStatus.pressedKeys & A_BUTTON)
		css_lpanel_set_face_displayed(proc, !css_lpanel_is_face_displayed(proc));
	
	/*
	for (int i = 0; i < 8; ++i)
		css_stattext_display(
			proc->pStatText,
			i,
			proc->origin.x + 8 + 64 - 16,
			proc->origin.y + 48 + (12 * i)
		); //*/
}

static void css_lpanel_on_end(CSSLPanelProc* proc) {
	css_lpanel_hide_face(proc);

	css_stattext_end(proc->pStatText);
}

static int css_lpanel_moveup(CSSLPanelProc* proc) {
	
	
	return 1;
}

static void css_lpanel_show_face(CSSLPanelProc* proc) {
	if (!proc->faceDisplayed)
		StartFace(0, GetUnitPortraitId(gActiveUnit), 0x38, 0x38, 2);
	
	proc->faceDisplayed = 1;
}

static void css_lpanel_hide_face(CSSLPanelProc* proc) {
	if (proc->faceDisplayed)
		EndFaceById(0);
	
	proc->faceDisplayed = 0;
}

CSSLPanelProc* css_lpanel_start(Vector2 origin, Proc* parent) {
	CSSLPanelProc* result;

	result = (CSSLPanelProc*) StartProc(proc_css_lpanel, parent);
	
	result->origin = origin;

	return result;
}

void css_lpanel_end(CSSLPanelProc* proc) {
	EndProc((Proc*)(proc));
}

int css_lpanel_is_face_displayed(CSSLPanelProc* proc) {
	return proc->faceDisplayed;
}

void css_lpanel_set_face_displayed(CSSLPanelProc* proc, int display) {
	if (display)
		css_lpanel_show_face(proc);
	else
		css_lpanel_hide_face(proc);
}
