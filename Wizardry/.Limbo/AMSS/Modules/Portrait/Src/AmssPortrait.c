
#include "Amss.h" // TODO: fix

// TODO: update reference and add to CLib
static void(*DisplayBgFace)(struct Proc* proc, u16* map, unsigned portrait, unsigned tileId, unsigned palId) = (const void*)(0x08005E98+1);
static const void(*DrawMiniMug)(unsigned portrait, u16* map, int tileId, int palId, int side) = (const void*)(0x08005988+1);

static void AmssPortraitInit(u16* fg, u16* bg, u32 user, struct AmssGeometry rect, struct AmssMainProc* proc);
// static void AmssPortraitIdle(u16* fg, u16* bg, u32 user, const struct AmssGeometry* geometry, struct Proc* proc);

//*

static
const struct AmssHelpInfo sTestHelpInfo[] = {
	{ { 0, 0, 4, 4 }, 2,  NULL },
	{ { 0, 5, 4, 4 }, 7,  NULL },
	{ { 6, 0, 4, 4 }, 12, NULL },
	{ { 6, 5, 4, 4 }, 13, NULL },
	{}
};

// */

const struct AmssModule AmssPortraitModule = {
	.onInit = AmssPortraitInit,
	.helpInfo = sTestHelpInfo
};

static void AmssPortraitInit(u16* fg, u16* bg, u32 user, struct AmssGeometry rect, struct AmssMainProc* proc)
{
	unsigned portrait = GetUnitPortraitId(gStatScreenData.unit);

	if (gStatScreenData.unit->state & US_BIT23)
		portrait += 1;

	if (!user)
		user = 0x000B04E0;

	DisplayBgFace((struct Proc*) proc, bg + TILEMAP_INDEX(rect.x, rect.y), portrait, user & 0xFFFF, (user >> 16) & 0xF);
}

static void AmssMiniPortraitInit(u16* fg, u16* bg, u32 user, struct AmssGeometry rect, struct AmssMainProc* proc);

const struct AmssModule AmssMiniPortraitModule = {
	.onInit = AmssMiniPortraitInit,
};

static void AmssMiniPortraitInit(u16* fg, u16* bg, u32 user, struct AmssGeometry rect, struct AmssMainProc* proc)
{
	unsigned portrait = GetUnitPortraitId(gStatScreenData.unit);

	if (gStatScreenData.unit->state & US_BIT23)
		portrait += 1;

	if (!user)
		user = 0x000B04E0;

	DrawMiniMug(portrait, bg + TILEMAP_INDEX(rect.x, rect.y), user & 0xFFFF, user >> 16, 0);
}

