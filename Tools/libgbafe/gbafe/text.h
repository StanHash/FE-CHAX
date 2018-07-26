#ifndef GBAFE_TEXT_H
#define GBAFE_TEXT_H

#include "common.h"

typedef struct _FontData       FontData;
typedef struct _TextHandle     TextHandle;
typedef struct _TextBatchEntry TextBatchEntry;
typedef struct _FontGlyphData  FontGlyphData;

typedef enum   _FontGlyphType  FontGlyphType;

struct _FontData {
	/* 00 */ void* pVRAMTileRoot;
	/* 04 */ const void* pGlyphData;

	/* 08 */ void (*drawGlyph)(TextHandle*, const FontGlyphData*);
	/* 0C */ void*(*getDrawTarget)(TextHandle*);

	/* 10 */ uint16_t tileBase;
	/* 12 */ uint16_t tileNext;
	/* 14 */ uint16_t palIndex;

	/* 16 */ uint8_t _u16;
	/* 17 */ uint8_t _u17;
};

struct _TextHandle {
	/* 00 */ uint16_t tileIndexOffset;

	/* 02 */ uint8_t xCursor;
	/* 03 */ uint8_t colotId;
	
	/* 04 */ uint8_t tileWidth;

	/* 05 */ uint8_t useDoubleBuffer;
	/* 06 */ uint8_t currentBufferId;

	/* 07 */ uint8_t _u07;
};

struct _TextBatchEntry {
	/* 00 */ TextHandle* textHandle;
	/* 04 */ size_t      tileWidth;
};

struct _FontGlyphData {
	/* 00 */ const FontGlyphData* pNextMaybe;
	/* 04 */ uint8_t wcbyte2;
	/* 05 */ uint8_t pxLength;
	/* 08 */ uint32_t lines2bpp[0x10];
};

enum _FontGlyphType {
	FONT_GLYPH_UI       = 0,
	FONT_GLYPH_DIALOGUE = 1
};

extern FontData gDefaultFontData;
extern FontData* gpCurrentFont;

#pragma long_calls

void            Font_InitDefault(void);                                                           //! FE8U = 0x8003C95
void            Font_InitForUI(FontData* pData, void* pVRAMTileRoot, uint16_t tileBase, int palIndex); //! FE8U = 0x8003CB9
void            Font_SetGlyphSet(FontGlyphType);                                                        //! FE8U = 0x8003CF5
void            Font_ResetAllocation(void);                                                            //! FE8U = 0x8003D21
void            SetFont(FontData*);                                                                    //! FE8U = 0x8003D39

void            Text_InitClear(TextHandle*, size_t tileWidth);                                              //! FE8U = 0x8003D5D
void            Text_InitDB(TextHandle*, size_t tileWidth);                                          //! FE8U = 0x8003D85
void            InitClearTextBatch(const TextBatchEntry[]);                                                 //! FE8U = 0x8003DAD

void            Text_Clear(TextHandle*);                                                               //! FE8U = 0x8003DC9

int             Text_GetXCursor(TextHandle*);                                                          //! FE8U = 0x8003E51
TextHandle*     Text_SetXCursor(TextHandle*, int);                                                     //! FE8U = 0x8003E55
TextHandle*     Text_Advance(TextHandle*, int);                                                        //! FE8U = 0x8003E59

TextHandle*     Text_SetColorId(TextHandle*, int);                                                     //! FE8U = 0x8003E61
int             Text_GetColorId(TextHandle*);                                                          //! FE8U = 0x8003E65

TextHandle*     Text_SetParameters(TextHandle*, int cursor, int color);                                //! FE8U = 0x8003E69

void            Text_Draw(TextHandle*, uint16_t* bgMap);                                               //! FE8U = 0x8003E71
void            Text_DrawBlank(TextHandle*, uint16_t* bgMap);                                          //! FE8U = 0x8003EBD

size_t          GetStringTextWidth(const char*);                                                       //! FE8U = 0x8003EDD
const char*     GetCharTextWidth(const char* in, size_t* out);                                         //! FE8U = 0x8003F3D
size_t          GetStringTextCenteredPos(const char*);                                                 //! FE8U = 0x8003F91

const char*     String_GetEnd(const char*);                                                            //! FE8U = 0x8003FF5

void            Text_AppendString(TextHandle*, const char*);                                           //! FE8U = 0x8004005
void            Text_AppendDecNumber(TextHandle*, int);                                                //! FE8U = 0x8004075
void            Text_AppendNumberOr2Dashes(TextHandle*, uint8_t);                                      //! FE8U = 0x8004145
void            Text_AppendChar(TextHandle*, char);                                                    //! FE8U = 0x8004181

// TODO: figure more out
// (I only have vague knowledge on what most things past here does)

void*           Text_Get1DVRAMTarget(TextHandle*);                                               //! FE8U = 0x80041E9
const uint16_t* Get2bppTo4bppLookup(int);                                            //! FE8U = 0x8004209
void            DrawGlyph1DTile(TextHandle*, const FontGlyphData*);                           //! FE8U = 0x8004219
void            DrawGlyph1DTileNoClear(TextHandle*, const FontGlyphData*);                        //! FE8U = 0x8004269

void            Font_LoadForUI(void);                                                                  //! FE8U = 0x80043A9
void            Font_LoadForDialogue(void);                                                            //! FE8U = 0x80043E9
void            Font_SetDraw1DTileNoClear(void);                                               //! FE8U = 0x8004429

void            DrawTextInline(TextHandle*, uint16_t* bg, int color, int xStart, int tileWidth, const char* cstring); //! FE8U = 0x800443D
void            Text_InsertString(TextHandle*, int xPos, int color, const char* str);                  //! FE8U = 0x8004481
void            Text_InsertNumberOr2Dashes(TextHandle*, int xPos, int color, uint8_t);                 //! FE8U = 0x80044A5

void            Text_AppendStringAscii(TextHandle*, const char*);                                     //! FE8U = 0x80044C9
void            Text_AppendCharAscii(TextHandle*, char);                                              //! FE8U = 0x8004505

const char*     GetCharTextWidthAscii(const char* in, size_t* out);                                   //! FE8U = 0x8004539
size_t          GetStringTextWidthAscii(const char*);                                                 //! FE8U = 0x8004569

void            Font_InitForObj(FontData*, void* vram, int pal);                    //! FE8U = 0x800459D
void            Text_InitClear3(TextHandle*);                                                               //! FE8U = 0x80045D9
void            Text_Fill2DLine(TextHandle*);                                                             //! FE8U = 0x80046B5

#pragma long_calls_off

#endif // GBAFE_TEXT_H
