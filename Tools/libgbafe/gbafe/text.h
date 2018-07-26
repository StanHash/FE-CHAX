#ifndef GBAFE_TEXT_H
#define GBAFE_TEXT_H

#include "common.h"

typedef struct FontData FontData;
typedef struct TextHandle TextHandle;
typedef struct TextBatchEntry TextBatchEntry;
typedef struct FontGlyphData FontGlyphData;

typedef enum FontGlyphType FontGlyphType;

struct FontData {
	/* 00 */ void* pVRAMTileRoot;
	/* 04 */ const void* pGlyphData;

	/* 08 */ void (*drawGlyph)(TextHandle*, const FontGlyphData*);
	/* 0C */ void*(*getDrawTarget)(TextHandle*);

	/* 10 */ u16 tileBase;
	/* 12 */ u16 tileNext;
	/* 14 */ u16 palIndex;

	/* 16 */ u8 _u16;
	/* 17 */ u8 _u17;
};

struct TextHandle {
	/* 00 */ u16 tileIndexOffset;

	/* 02 */ u8 xCursor;
	/* 03 */ u8 colotId;
	
	/* 04 */ u8 tileWidth;

	/* 05 */ u8 useDoubleBuffer;
	/* 06 */ u8 currentBufferId;

	/* 07 */ u8 _u07;
};

struct TextBatchEntry {
	/* 00 */ TextHandle* textHandle;
	/* 04 */ unsigned tileWidth;
};

struct FontGlyphData {
	/* 00 */ const FontGlyphData* pNextMaybe;
	/* 04 */ u8 wcbyte2;
	/* 05 */ u8 pxLength;
	/* 08 */ u32 lines2bpp[0x10];
};

enum FontGlyphType {
	FONT_GLYPH_UI = 0,
	FONT_GLYPH_DIALOGUE = 1
};

extern FontData gDefaultFontData;
extern FontData* gpCurrentFont;

#pragma long_calls

void Font_InitDefault(void); //! FE8U = 0x8003C95
void Font_InitForUI(FontData* pData, void* pVRAMTileRoot, u16 tileBase, int palIndex); //! FE8U = 0x8003CB9
void Font_SetGlyphSet(FontGlyphType); //! FE8U = 0x8003CF5
void Font_ResetAllocation(void); //! FE8U = 0x8003D21
void SetFont(FontData*); //! FE8U = 0x8003D39

void Text_InitClear(TextHandle*, unsigned tileWidth); //! FE8U = 0x8003D5D
void Text_InitDB(TextHandle*, unsigned tileWidth); //! FE8U = 0x8003D85
void InitClearTextBatch(const TextBatchEntry[]); //! FE8U = 0x8003DAD

void Text_Clear(TextHandle*); //! FE8U = 0x8003DC9

int Text_GetXCursor(TextHandle*); //! FE8U = 0x8003E51
TextHandle* Text_SetXCursor(TextHandle*, int); //! FE8U = 0x8003E55
TextHandle* Text_Advance(TextHandle*, int); //! FE8U = 0x8003E59

TextHandle* Text_SetColorId(TextHandle*, int); //! FE8U = 0x8003E61
int Text_GetColorId(TextHandle*); //! FE8U = 0x8003E65

TextHandle* Text_SetParameters(TextHandle*, int cursor, int color); //! FE8U = 0x8003E69

void Text_Draw(TextHandle*, u16* bgMap); //! FE8U = 0x8003E71
void Text_DrawBlank(TextHandle*, u16* bgMap); //! FE8U = 0x8003EBD

unsigned GetStringTextWidth(const char*); //! FE8U = 0x8003EDD
const char* GetCharTextWidth(const char* in, unsigned* out); //! FE8U = 0x8003F3D
unsigned GetStringTextCenteredPos(const char*); //! FE8U = 0x8003F91

const char* String_GetEnd(const char*); //! FE8U = 0x8003FF5

void Text_AppendString(TextHandle*, const char*); //! FE8U = 0x8004005
void Text_AppendDecNumber(TextHandle*, int); //! FE8U = 0x8004075
void Text_AppendNumberOr2Dashes(TextHandle*, u8); //! FE8U = 0x8004145
void Text_AppendChar(TextHandle*, char); //! FE8U = 0x8004181

// TODO: figure more out
// (I only have vague knowledge on what most things past here does)

void* Text_Get1DVRAMTarget(TextHandle*); //! FE8U = 0x80041E9
const u16* Get2bppTo4bppLookup(int); //! FE8U = 0x8004209
void DrawGlyph1DTile(TextHandle*, const FontGlyphData*); //! FE8U = 0x8004219
void DrawGlyph1DTileNoClear(TextHandle*, const FontGlyphData*); //! FE8U = 0x8004269

void Font_LoadForUI(void); //! FE8U = 0x80043A9
void Font_LoadForDialogue(void); //! FE8U = 0x80043E9
void Font_SetDraw1DTileNoClear(void); //! FE8U = 0x8004429

void DrawTextInline(TextHandle*, u16* bg, int color, int xStart, int tileWidth, const char* cstring); //! FE8U = 0x800443D
void Text_InsertString(TextHandle*, int xPos, int color, const char* str); //! FE8U = 0x8004481
void Text_InsertNumberOr2Dashes(TextHandle*, int xPos, int color, u8); //! FE8U = 0x80044A5

void Text_AppendStringAscii(TextHandle*, const char*); //! FE8U = 0x80044C9
void Text_AppendCharAscii(TextHandle*, char); //! FE8U = 0x8004505

const char* GetCharTextWidthAscii(const char* in, unsigned* out); //! FE8U = 0x8004539
unsigned GetStringTextWidthAscii(const char*); //! FE8U = 0x8004569

void Font_InitForObj(FontData*, void* vram, int pal); //! FE8U = 0x800459D
void Text_Init2DLine(TextHandle*); //! FE8U = 0x80045D9
void Text_Fill2DLine(TextHandle*); //! FE8U = 0x80046B5

#pragma long_calls_off

#endif // GBAFE_TEXT_H
