
#include "Nm.h"

/* =========================== */
/* = NmBackground proc begin = */
/* =========================== */

struct NmBackground
{
	PROC_HEADER;

	unsigned test;
	int xTest, yTest;
};

static void NmBackground_OnInit(struct NmBackground* proc);
static void NmBackground_OnLoop(struct NmBackground* proc);
static void NmBackground_OnEnd(struct NmBackground* proc);

const struct ProcInstruction gProc_NmBackground[] = {
	PROC_SET_NAME("Stan's NewMap Background"),

	PROC_CALL_ROUTINE(NmBackground_OnInit),
	PROC_SET_DESTRUCTOR(NmBackground_OnEnd),
	PROC_LOOP_ROUTINE(NmBackground_OnLoop),

	PROC_END,
};

static void NmBackground_OnInit(struct NmBackground* proc)
{
	// gLCDIOBuffer.dispControl.bgMode = 1; // BG0, BG1 normal, BG2 affine

	proc->test = 0;

	proc->xTest = 120;
	proc->yTest = 80;

	gPaletteBuffer[15 * 0x10 + 0] = COLOR_RGB(0,  0,  0);
	gPaletteBuffer[15 * 0x10 + 1] = COLOR_RGB(31, 31, 31);

	EnablePaletteSync();

	for (unsigned i = 0x40 * 1; i < 0x40 * 3; i += 2)
	{
		u16* addr = (u16*) (VRAM + i);
		*addr = RandNext();
	}

	CpuFill16(0xF0F0F0F0, VRAM + 0x40 * 1, 0x40); // tile 1
	CpuFill16(0xF1F1F1F1, VRAM + 0x40 * 2, 0x40); // tile 2

	u8* buf = (u8*) gBg2MapBuffer;

	for (unsigned ix = 0; ix < 32; ++ix)
		for (unsigned iy = 0; iy < 32; ++iy)
			buf[ix + 32 * iy] = 1 + NextRN_N(2);

	EnableBgSyncByMask(BG2_SYNC_BIT);

	m7Init();

	m7World->bg2cnt = gLCDIOBuffer.bgControl[2];

	m7World->bg2cnt.screenSize = 1;
	m7World->bg2cnt.tileBaseBlock = 0;
	m7World->bg2cnt.wrap = TRUE;
}

static void NmBackground_OnLoop(struct NmBackground* proc)
{
	proc->xTest += 2 * (!!(gKeyState.heldKeys & KEY_DPAD_RIGHT) - !!(gKeyState.heldKeys & KEY_DPAD_LEFT));
	proc->yTest += 2 * (!!(gKeyState.heldKeys & KEY_DPAD_DOWN)  - !!(gKeyState.heldKeys & KEY_DPAD_UP));

	ObjInsertSafe(4, proc->xTest - 16, proc->yTest - 8, &gObj_16x16, 0);

	if (gKeyState.heldKeys & KEY_BUTTON_L)
	{
		m7Rotate(
			m7World->cam->data.phi   + !!(gKeyState.heldKeys & KEY_DPAD_RIGHT) - !!(gKeyState.heldKeys & KEY_DPAD_LEFT),
			m7World->cam->data.theta + !!(gKeyState.heldKeys & KEY_DPAD_DOWN)  - !!(gKeyState.heldKeys & KEY_DPAD_UP));
	}
	else
	{
		m7TranslateLocal(
			32 * (!!(gKeyState.heldKeys & KEY_DPAD_RIGHT) - !!(gKeyState.heldKeys & KEY_DPAD_LEFT)),
			32 * (!!(gKeyState.heldKeys & KEY_DPAD_DOWN)  - !!(gKeyState.heldKeys & KEY_DPAD_UP)),
			32 * (!!(gKeyState.heldKeys & KEY_BUTTON_A)   - !!(gKeyState.heldKeys & KEY_BUTTON_B)));
	}

	// m7Rotate(proc->xTest - 120, proc->yTest - 80);
	// m7TranslateLevel(50, 50, 50);

	if (m7World->cam->data.pos.y < (2<<8))
		m7World->cam->data.pos.y = (2<<8);

	/*
	int cc = COS_LOOKUP(proc->test) >> 4, ss = SIN_LOOKUP(proc->test) >> 4;

	int pa, pb, pc, pd;

	gLCDIOBuffer.bg2pa = pa =  cc;
	gLCDIOBuffer.bg2pb = pb = -ss;
	gLCDIOBuffer.bg2pc = pc =  ss;
	gLCDIOBuffer.bg2pd = pd = cc;

	gLCDIOBuffer.bg2x = (128<<8) - (pa*(proc->xTest) + pb*(proc->yTest));
	gLCDIOBuffer.bg2y = (128<<8) - (pc*(proc->xTest) + pd*(proc->yTest));

	// */

//	gLCDIOBuffer.bg2x = 0x100 * 240 / 2;
//	gLCDIOBuffer.bg2y = 0x100 * 160 / 2;

	proc->test++;
}

static void NmBackground_OnEnd(struct NmBackground* proc)
{
	// gLCDIOBuffer.dispControl.bgMode = 0;
	m7End();
}

/* ===================== */
/* = NmMain proc begin = */
/* ===================== */

struct NmMain
{
	PROC_HEADER;

	/* 2C */ struct NmBackground* background;
};

static void NmMain_OnInit(struct NmMain* proc);

const struct ProcInstruction gProc_NmMain[] = {
	PROC_SET_NAME("Stan's NewMap Main"),

	PROC_CALL_ROUTINE(NmMain_OnInit),
	PROC_BLOCK,

	PROC_END,
};

static void NmMain_OnInit(struct NmMain* proc)
{
	proc->background = START_PROC(gProc_NmBackground, proc);
}

/* ==================== */
/* = Public interface = */
/* ==================== */

void NmStart(struct Proc* proc)
{
	/* struct NmMain* main = */ proc
		? START_PROC_BLOCKING(gProc_NmMain, proc)
		: START_PROC(gProc_NmMain, ROOT_PROC_3);
}
