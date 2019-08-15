
#include "M7.h"

// TODO: sprite stuff!

extern u8* const      m7RamFuncRom;
extern const unsigned m7RamFuncSize;
extern u8* const      m7RamFunc;

static
const struct ProcInstruction sM7DataProc[] = { PROC_BLOCK };

static
void m7VSync(void);

static
const struct ProcInstruction sM7VSyncProc[] = {
	PROC_SET_NAME("m7 VSync"),
	PROC_LOOP_ROUTINE(m7VSync),
	PROC_END
};

static inline
void* StartDataProc(void) { return START_PROC(sM7DataProc, ROOT_PROC_7); }

void m7Init(void)
{
	m7World->cam = StartDataProc();
	m7World->horizon = 80;

	m7Indentity();

	START_PROC(sM7VSyncProc, ROOT_PROC_0);

	CpuCopy32(m7RamFuncRom, m7RamFunc, m7RamFuncSize);
	SetSecondaryHBlankCallback((void(*)(void))(m7RamFunc));
}

void m7End(void)
{
	EndEachProc(sM7DataProc);
	EndEachProc(sM7VSyncProc);

	SetSecondaryHBlankCallback(NULL);
}

static
struct M7Sprite* m7SortedMerge(struct M7Sprite* a, struct M7Sprite* b)
{
	if (!a)
		return b;

	if (!b)
		return a;

	struct M7Sprite* result = NULL;

	if (a->camPos.z < b->camPos.z) // <---- this is the important bit!
	{
		result = a;
		result->next = m7SortedMerge(a, b->next);
	}
	else
	{
		result = b;
		result->next = m7SortedMerge(a->next, b);
	}

	return result;
}

static
void m7SortSplit(struct M7Sprite* head, struct M7Sprite** outLeft, struct M7Sprite** outRight)
{
	struct M7Sprite* mid = head;
	struct M7Sprite* end = head->next;

	while (end)
	{
		if ((end = end->next))
		{
			mid = mid->next;
			end = end->next;
		}
	}

	*outLeft  = head;
	*outRight = mid->next;

	mid->next = NULL;
}

static
void m7MergeSort(struct M7Sprite** headAddr)
{
	struct M7Sprite* head = *headAddr;

	if (!head || !head->next)
		return; // 0 or 1 element, no sorting required

	struct M7Sprite* left  = NULL;
	struct M7Sprite* right = NULL;

	m7SortSplit(head, &left, &right);

	m7MergeSort(&left);
	m7MergeSort(&right);

	*headAddr = m7SortedMerge(left, right);
}

void m7SortSprites(void)
{
	m7MergeSort(&m7World->sprite);
}

void m7DisplaySprites(void)
{
	for (struct M7Sprite* sprite = m7World->sprite; sprite; sprite = sprite->next)
	{
		const u16 obj[] = { 1, sprite->oam0, sprite->oam1, sprite->oam2 };
		PushToLoOAM(0, 0, (const void*)obj, 0);
	}
}

void m7InitSprite(struct M7Sprite* sprite)
{
	// TODO: actually initialize the sprite

	// Linking
	sprite->next = m7World->sprite;
	m7World->sprite = sprite;
}

void m7Indentity(void)
{
	static const struct M7Camera sDefaultCam = {
		{ 0x0D100, 0x1900, 0x38800 }, 
		0x0A00, 0x2600,
		{ 256, 0, 0 }, { 0, 256, 0 }, { 0, 0, 256 }
	};

	m7World->cam->data = sDefaultCam;
}

void m7Rotate(unsigned phi, unsigned theta)
{
	struct M7Camera* const cam = &m7World->cam->data;

	cam->phi   = phi;
	cam->theta = theta;

	int cf = COS_LOOKUP(phi)   >> 4, sf = SIN_LOOKUP(phi)   >> 4;
	int ct = COS_LOOKUP(theta) >> 4, st = SIN_LOOKUP(theta) >> 4;

	// camera X-axis (right)
	cam->u.x = cf;
	cam->u.y = 0;
	cam->u.z = sf;

	// camera Y-axis (up)
	cam->v.x = sf*st >> 8;
	cam->v.y = ct;
	cam->v.z = -cf*st >> 8;

	// camera Z-axis (back)
	cam->w.x = -sf*ct >> 8;
	cam->w.y = st;
	cam->w.z = cf*ct >> 8;
}

void m7TranslateLevel(int x, int y, int z)
{
	struct M7Camera* const cam = &m7World->cam->data;

	cam->pos.x += (cam->u.x * x - cam->u.z * z) >> 8;
	cam->pos.y += y;
	cam->pos.z += (cam->u.z * x + cam->u.x * z) >> 8;
}

void m7TranslateLocal(int x, int y, int z)
{
	struct M7Camera* const cam = &m7World->cam->data;

	cam->pos.x += (cam->u.x * x + cam->v.x * y + cam->w.x * z) >> 8;
	cam->pos.y += (0            + cam->v.y * y + cam->w.y * z) >> 8;
	cam->pos.z += (cam->u.z * x + cam->v.z * y + cam->w.z * z) >> 8;
}

void m7TranslateGlobal(int x, int y, int z)
{
	struct M7Camera* const cam = &m7World->cam->data;

	cam->pos.x += x;
	cam->pos.y += y;
	cam->pos.z += z;
}

static
void m7PrepHorizon(void)
{
	struct M7Camera* const cam = &m7World->cam->data;

	if (cam->v.y != 0)
	{

#ifndef M7_HORZ_INFINITE

		int tmp = M7_FAR_BG*cam->w.y - cam->pos.y;
		m7World->horizon = M7_TOP - Div(tmp*M7_D, M7_FAR_BG*cam->v.y);

#else

		m7World->horizon = M7_TOP - Div(cam->w.y*M7_D, cam->v.y);

#endif

	}
	else
	{
		// looking straight down (w.y > 0) means horizon at -inf scanline
		m7World->horizon = cam->w.y > 0 ? INT16_MIN : INT16_MAX;
	}

	if (m7World->horizon < 0)
		m7World->horizon = 0;
}

static
void m7PrepAffines(void)
{
	if (m7World->horizon < 0 || m7World->horizon >= SCREEN_HEIGHT)
		return;

	struct M7Camera* const cam = &m7World->cam->data;

	for (unsigned i = m7World->horizon; i < SCREEN_HEIGHT; ++i)
	{
		int yb = (i - M7_TOP)*M7_GET_CT + M7_D*M7_GET_ST;
		m7World->lam[i] = Div(cam->pos.y << 12, yb);
	}
}

static
void m7VSync(void)
{
	m7PrepHorizon();
	m7PrepAffines();
}
