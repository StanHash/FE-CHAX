#include "css_objtext.h"

enum StatTextPaletteIndices {
	STPAL_NONE = 0, // transparent

	STPAL_STATTEXT1 = 1,
	STPAL_STATTEXT2 = 2,
	STPAL_STATTEXT3 = 3,

	STPAL_NAMETEXT1 = 4,
	STPAL_NAMETEXT2 = 5,
	STPAL_NAMETEXT3 = 6,

	STPAL_BARVALUE1 = 7,
	STPAL_BARVALUE2 = 8,

	STPAL_BARBACKG1 = 9,
	STPAL_BARBACKG2 = 10
};

enum StatTextDrawFlags {
	STDRAW_HIDDEN = (1 << 0),
	STDRAW_ALTBUF = (1 << 1)
};

static void css_stattext_on_update(CSSStatTextProc*);

static const ProcCode css_stattext_proc[] = {
	PROC_SET_NAME(CSS_PREFIX":StatText"),
	PROC_LOOP_ROUTINE(css_stattext_on_update),
	PROC_END
};

static const unsigned sStatTextTileWidth = 6; // 48 pixels

static const uint16_t sStatTextObjTileLookup[] = {
	0x180, 0x1C0,
	0x186, 0x1C6,
	0x18C, 0x1CC,
	0x192, 0x1D2,
	0x198, 0x1D8
};

static const Vector2 sStatTextPositionLookup[] = {
	{ 0,  0  },
	{ 56, 0  },
	{ 0,  12 },
	{ 56, 12 },
	{ 0,  24 },
	{ 56, 24 },
	{ 0,  36 },
	{ 56, 36 }
};

// TODO: move the obj font stuff elsewhere

#define THM_LLSL ((uint64_t(*)(uint64_t, unsigned))(0x80D188D))

static void css_stats_draw_glyph(TextHandle* text, const FontGlyphData* glyph) {
	uint32_t* const target = gpCurrentFont->getDrawTarget(text);

	const uint16_t* const colorLookup = Get2bppTo4bppLookup(text->colotId);
	const uint16_t* const maskLookup  = Get2bppTo4bppLookup(9);

	const unsigned x2bppOffset = (text->xCursor % 8) * 2;

	#define MASK32(aLine) (maskLookup[(aLine) & 0xFF] | (maskLookup[((aLine)>>8) & 0xFF]<<16))
	#define COLOR32(aLine) (colorLookup[(aLine) & 0xFF] | (colorLookup[((aLine)>>8) & 0xFF]<<16))

	for (unsigned i = 0; i < 8; ++i) {
		uint64_t line = THM_LLSL(glyph->lines2bpp[i], x2bppOffset);

		target[i + 0x000] = (target[i + 0x000] & MASK32(line))     | COLOR32(line);
		target[i + 0x008] = (target[i + 0x008] & MASK32(line>>16)) | COLOR32(line>>16);
		target[i + 0x010] = (target[i + 0x010] & MASK32(line>>32)) | COLOR32(line>>32);
	}

	for (unsigned i = 0; i < 8; ++i) {
		uint64_t line = THM_LLSL(glyph->lines2bpp[i+8], x2bppOffset);

		target[i + 0x100] = (target[i + 0x100] & MASK32(line))     | COLOR32(line);
		target[i + 0x108] = (target[i + 0x108] & MASK32(line>>16)) | COLOR32(line>>16);
		target[i + 0x110] = (target[i + 0x110] & MASK32(line>>32)) | COLOR32(line>>32);
	}

	#undef MASK32
	#undef COLOR32

	text->xCursor += glyph->pxLength;
}

#undef THM_LLSL

static void css_stats_text_init(TextHandle* text, int xCursor, int colorIndex) {
	text->tileIndexOffset = 0;
	text->xCursor = xCursor;
	text->colotId = colorIndex;
	text->tileWidth = sStatTextTileWidth;
	text->useDoubleBuffer = FALSE;
	text->currentBufferId = 0;
	text->_u07 = 0;
}

static char* css_stats_num2str(char* out, int number) {
	out[0] = '0' + Div(number, 10);
	out[1] = '0' + Mod(number, 10);
	out[2] = 0;

	return (out[0] == '0') ? out+1 : out;
}

static void css_stats_draw_bar_2D(uint16_t* target, unsigned statValue, unsigned statCap) {
	static const unsigned statMax   = 30; // Should be enough
	static const unsigned startLine = 5;  // lines 5-6 of the tile are drawn used

	// TODO: redo

	if (statCap > statMax)
		statCap = statMax;

	if (statValue > statMax)
		statValue = statMax;

	unsigned sizeCap  = Div(sStatTextTileWidth*8 * statCap,   statMax);
	unsigned sizeStat = Div(sStatTextTileWidth*8 * statValue, statMax);

	for (unsigned col = 0; col < sStatTextTileWidth*2; ++col) {
		for (unsigned line = 0; line < 2; ++line) {
			uint16_t gfx = 0;

			for (unsigned j = 0; j < 4; ++j) {
				unsigned px = col*4 + j;

				gfx |= ((px < sizeStat)
					? (STPAL_BARBACKG1+line)
					: ((px < sizeCap)
						? (STPAL_BARVALUE1+line)
						: (STPAL_NONE)))
					<< j*4;
			}

			target[(col/2) * 0x10 + startLine*2 + line*2 + col%2] = gfx;
		}
	}
}

static void css_stats_draw_text_2D(void* target, const char* statName, unsigned statValue) {
	static const void(*Font_InitForObj)(FontData*, void*, int) = (void(*)(FontData*, void*, int))(0x800459D);

	FontData   font;
	TextHandle text;

	Font_InitForObj(&font, target, 0);

	Font_SetGlyphSet(0);
	font.drawGlyph = css_stats_draw_glyph;

	css_stats_text_init(&text, 0, 1);
	Text_AppendString(&text, statName);

	char buf[0x10];
	char* const str = css_stats_num2str(buf, statValue);

	css_stats_text_init(&text, sStatTextTileWidth*8 - 1 - GetStringTextWidth(str), 0);
	Text_AppendString(&text, str);

	SetFont(NULL);
}

static void css_stats_draw(int targetObjTile, const char* statName, int statValue, int statMax) {
	void* const target = (OBJ_VRAM0 + (targetObjTile * 0x20));

	// Clear gfx
	CpuFastFill16(0, target,         0x20 * sStatTextTileWidth);
	CpuFastFill16(0, target + 0x400, 0x20 * sStatTextTileWidth);

	css_stats_draw_bar_2D(target + 0x400, statValue, statMax);
	css_stats_draw_text_2D(target, statName, statValue);
}

static void css_stats_display(int sourceObjTile, int xPosition, int yPosition) {
	static const ObjData obj = {
		2, { {
			0x0000, // Square
			0x4000, // 16x16p
			0x0000
		}, {
			0x4000, // Horizontal
			0x8010, // 32x16p
			0x0002
		} }
	};

	HiObjInsert(0,
		xPosition,
		yPosition,
		&obj,
		sourceObjTile | ((CSS_PAL_OBJ_STATTEXT&0xF)<<0xC)
	);
}

void css_stattext_on_update(CSSStatTextProc* proc) {
	for (unsigned i = 0; i < 8; ++i) {
		if (proc->statDrawFlags[i] & STDRAW_HIDDEN) {
			proc->statDrawFlags[i] &= ~STDRAW_ALTBUF;
			continue;
		}

		css_stats_display(
			sStatTextObjTileLookup[i],
			proc->origin.x + sStatTextPositionLookup[i].x,
			proc->origin.y + sStatTextPositionLookup[i].y
		);
	}

}

CSSStatTextProc* css_stattext_start(Vector2 origin) {
	CSSStatTextProc* result;

	result = (CSSStatTextProc*) Proc_Create(css_stattext_proc, ROOT_PROC_3);

	if (!result)
		return NULL;

	result->origin.x = origin.x;
	result->origin.y = origin.y;

	for (unsigned i = 0; i < 8; ++i)
		result->statDrawFlags[i] = STDRAW_HIDDEN;

	return result;
}

void css_stattext_end(CSSStatTextProc* proc) {
	Proc_Delete((Proc*)(proc));
}

void css_stattext_show(CSSStatTextProc* proc, int id, const char* name, int value, int maxValue) {
	if (!(proc->statDrawFlags[id] & STDRAW_HIDDEN))
		proc->statDrawFlags[id] ^= STDRAW_ALTBUF;

	css_stats_draw(sStatTextObjTileLookup[id], name, value, maxValue);
	proc->statDrawFlags[id] &= ~STDRAW_HIDDEN;
}

void css_stattext_hide(CSSStatTextProc* proc, int id) {
	proc->statDrawFlags[id] |= STDRAW_HIDDEN;
}
