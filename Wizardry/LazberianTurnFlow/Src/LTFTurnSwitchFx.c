#include "LTF.h"

extern const u8  gLTFTurnSwitchSheetImg[];
extern const u16 gLTFTurnSwitchSheetPal[];

static void StartObject(int xOrigin, int yOrigin, int chr, const void* obj, struct Proc* parent);
static void SetObjectAllBlend(void);
static void SetObjectAllFlat(void);
static void EndAllObjects(void);

enum
{
	LTF_TURNSWITCH_GFX_OBJPAL = 2,
	LTF_TURNSWITCH_GFX_OBJCHR = 0x180,
};

static const struct ObjData sSpr_Turn =
{
	1, { 0x4000, 0xC000, TILEREF(0, 0) } // h, 64x32
};

static const struct ObjData sSpr_Number =
{
	1, { 0x8000, 0x8000, TILEREF(0, 0) } // v, 16x32
};

struct TurnSwitchFxProc
{
	PROC_HEADER;

	/* 2C */ unsigned blendTimer;
	/* 30 */ unsigned turnDigitCnt;
	/* 34 */ u8 turnDigits[4];
	/* 38 */ struct Vec2u origin;
};

static void TurnSwitchFx_OnInit(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnBlendInLoop(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnBlendInEnd(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnBlendOutInit(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnBlendOutLoop(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnEnd(struct TurnSwitchFxProc* proc);

const struct ProcInstruction gProc_TurnSwitchFx[] =
{
	PROC_SET_NAME("Stan:LTF:TurnSwitchFx"),

//	PROC_NEW_CHILD(0x0859AF40),

PROC_LABEL(1),
	PROC_CALL_ROUTINE(TurnSwitchFx_OnInit),

PROC_LABEL(0),
	PROC_SLEEP(1),

	PROC_LOOP_ROUTINE(TurnSwitchFx_OnBlendInLoop),
	PROC_CALL_ROUTINE(TurnSwitchFx_OnBlendInEnd),

	PROC_SLEEP(60),

	PROC_CALL_ROUTINE(TurnSwitchFx_OnBlendOutInit),
	PROC_LOOP_ROUTINE(TurnSwitchFx_OnBlendOutLoop),

	PROC_CALL_ROUTINE(TurnSwitchFx_OnEnd),

//	PROC_WHILE_EXISTS(0x0859AF40),

	PROC_END
};

static void TurnSwitchFx_OnInit(struct TurnSwitchFxProc* proc)
{
	memset(proc->turnDigits, 0, sizeof(proc->turnDigits));

	unsigned turn = 776; // gChapterData.turnNumber+1;
	unsigned digicnt = 0;

	do
	{
		proc->turnDigits[3] = proc->turnDigits[2];
		proc->turnDigits[2] = proc->turnDigits[1];
		proc->turnDigits[1] = proc->turnDigits[0];
		proc->turnDigits[0] = Mod(turn, 10);

		digicnt = digicnt+1;
		turn = Div(turn, 10);
	}
	while (turn);

	proc->origin.x = 120 - (70 + digicnt*16)/2;
	proc->origin.y = 80 - 16;

	SetColorEffectsParameters(BLEND_EFFECT_ALPHA, 0, 0x10, 0);
	SetColorEffectsFirstTarget(0, 0, 0, 0, 0);
	SetColorEffectsSecondTarget(1, 1, 1, 1, 1);

	Decompress(gLTFTurnSwitchSheetImg, VRAM_OBJ + 0x20 * LTF_TURNSWITCH_GFX_OBJCHR);
	ApplyPalette(gLTFTurnSwitchSheetPal, 0x10 + LTF_TURNSWITCH_GFX_OBJPAL);

	StartObject(proc->origin.x, proc->origin.y, LTF_TURNSWITCH_GFX_OBJCHR, &sSpr_Turn, (struct Proc*) proc);

	SetObjectAllFlat();
	SetObjectAllBlend();

	proc->blendTimer = 0;
	proc->turnDigitCnt = 0;

	// UPDATE SOUNDS

	if (Sound_GetCurrentSong() != GetCurrentMapMusicIndex())
		Sound_FadeSongOut(8);
}

static
void TurnSwitchFx_OnBlendInLoop(struct TurnSwitchFxProc* proc)
{
	const unsigned eva = proc->blendTimer;
	const unsigned evb = 0x10 - proc->blendTimer;

	SetColorEffectsParameters(BLEND_EFFECT_ALPHA, eva, evb, 0);

	if (++proc->blendTimer > 0x10)
		BREAK_PROC(proc);
}

static void TurnSwitchFx_OnBlendInEnd(struct TurnSwitchFxProc* proc)
{
	SetObjectAllFlat();

	if (proc->turnDigitCnt < 4 && (proc->turnDigits[proc->turnDigitCnt] || proc->turnDigitCnt == 0))
	{
		const unsigned turnDigit = proc->turnDigits[proc->turnDigitCnt];

		StartObject(
			proc->origin.x + 70 + 16*proc->turnDigitCnt,
			proc->origin.y,
			LTF_TURNSWITCH_GFX_OBJCHR + 8 + 2 * (turnDigit == 0 ? 9 : turnDigit-1),
			&sSpr_Number, (struct Proc*) proc);

		proc->turnDigitCnt++;
		proc->blendTimer = 0;

		ProcGoto((struct Proc*) proc, 0);
	}
}

static void TurnSwitchFx_OnBlendOutInit(struct TurnSwitchFxProc* proc)
{
	proc->blendTimer = 0;
	SetObjectAllBlend();

	PlaySfx(0x73);
}

static void TurnSwitchFx_OnBlendOutLoop(struct TurnSwitchFxProc* proc)
{
	const unsigned eva = proc->blendTimer < 0x20 ? 0x10 : 0x10 - (proc->blendTimer-0x20)/2;
	const unsigned evb = proc->blendTimer < 0x20 ? proc->blendTimer/2 : 0x10;

	SetColorEffectsParameters(BLEND_EFFECT_ALPHA, eva, evb, 0);

	if (++proc->blendTimer > 0x40)
		BREAK_PROC(proc);
}

static void TurnSwitchFx_OnEnd(struct TurnSwitchFxProc* proc)
{
	EndAllObjects();

	SetPrimaryHBlankCallback(NULL);
	ClearBG0BG1();
}

struct ObjectProc
{
	PROC_HEADER;

	/* 2A */ u16 imgChr;
	/* 2C */ struct Vec2 origin;

	/* 30 */ unsigned positionTimer;
	/* 34 */ int      xDisplayOffset;

	/* 38 */ const struct ObjData* obj;
};

static void TurnObject_OnInit(struct ObjectProc* proc);
static void TurnObject_OnLoopBlend(struct ObjectProc* proc);
static void TurnObject_OnLoopFlat(struct ObjectProc* proc);

static const struct ProcInstruction sProc_Object[] =
{
	PROC_SET_NAME("Stan:LTF:TurnObject"),

	PROC_CALL_ROUTINE(TurnObject_OnInit),
	PROC_SLEEP(1),

PROC_LABEL(0),
	PROC_CALL_ROUTINE(TurnObject_OnLoopBlend),
	PROC_LOOP_ROUTINE(TurnObject_OnLoopBlend),

PROC_LABEL(1),
	PROC_CALL_ROUTINE(TurnObject_OnLoopFlat),
	PROC_LOOP_ROUTINE(TurnObject_OnLoopFlat),

	PROC_END
};

static void TurnObject_OnInit(struct ObjectProc* proc)
{
	proc->positionTimer = 0;
}

static void TurnObject_PositionUpdate(struct ObjectProc* proc)
{
	static const unsigned OFFSET_MAX = 0x20;

	if (proc->positionTimer < (TRIG_PI/2))
	{
		const unsigned angle = proc->positionTimer += 2;
		proc->xDisplayOffset = OFFSET_MAX - (SIN_LOOKUP(angle) * OFFSET_MAX / TRIG_MAX);
	}
	else
	{
		proc->xDisplayOffset = 0;
	}
}

static void TurnObject_OnLoopBlend(struct ObjectProc* proc)
{
	TurnObject_PositionUpdate(proc);

	ObjInsert(0,
		0x1FF & (proc->origin.x + proc->xDisplayOffset),
		(0x0FF & (proc->origin.y)) | 0x0400,

		proc->obj, TILEREF(proc->imgChr, LTF_TURNSWITCH_GFX_OBJPAL));
}

static void TurnObject_OnLoopFlat(struct ObjectProc* proc)
{
	TurnObject_PositionUpdate(proc);

	ObjInsert(0,
		0x1FF & (proc->origin.x + proc->xDisplayOffset),
		0x0FF & (proc->origin.y),

		proc->obj, TILEREF(proc->imgChr, LTF_TURNSWITCH_GFX_OBJPAL));
}

static
void StartObject(int xOrigin, int yOrigin, int chr, const void* obj, struct Proc* parent)
{
	struct ObjectProc* proc = START_PROC(sProc_Object, parent);

	proc->origin.x = xOrigin;
	proc->origin.y = yOrigin;

	proc->imgChr = chr;

	proc->obj = obj;
}

static
void SetObjectBlend(struct Proc* proc)
{
	ProcGoto(proc, 0);
}

static
void SetObjectAllBlend(void)
{
	ForEachProc(sProc_Object, SetObjectBlend);
}

static
void SetObjectFlat(struct Proc* proc)
{
	ProcGoto(proc, 1);
}

static
void SetObjectAllFlat(void)
{
	ForEachProc(sProc_Object, SetObjectFlat);
}

static
void EndAllObjects(void)
{
	EndEachProc(sProc_Object);
}
