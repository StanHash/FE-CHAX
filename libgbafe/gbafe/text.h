#ifndef GBAFE_TEXT_H
#define GBAFE_TEXT_H

#include "common.h"

typedef struct _FontData       FontData;
typedef struct _TextHandle     TextHandle;
typedef struct _TextBatchEntry TextBatchEntry;

typedef enum   _FontGlyphType  FontGlyphType;

struct _FontData {
	void* pVRAMTileRoot;
	const void* pGlyphData;

	void (*drawGlyph)(TextHandle*, const void*);
	void*(*getDrawTarget)(TextHandle*);

	uint16_t tileBase;
	uint16_t tileNext;
	uint16_t palIndex;

	uint8_t _u16;
	uint8_t _u17;
};

struct _TextHandle {
	uint16_t tileIndexOffset;

	uint8_t xCursor;
	uint8_t colotId;
	
	uint8_t tileWidth;

	uint8_t useDoubleBuffer;
	uint8_t currentBufferId;

	uint8_t _u07;
};

struct _TextBatchEntry {
	TextHandle* textHandle;
	size_t      tileWidth;
};

enum _FontGlyphType {
	FONT_GLYPH_UI       = 0,
	FONT_GLYPH_DIALOGUE = 1
};

#pragma long_calls

void        Font_InitForUIDefault(void);                                                           //! FE8U = 0x8003C95
void        Font_InitForUI(FontData* pData, void* pVRAMTileRoot, uint16_t tileBase, int palIndex); //! FE8U = 0x8003CB9
void        SetFontGlyphSet(FontGlyphType);                                                        //! FE8U = 0x8003CF5
void        Font_ResetAllocation(void);                                                            //! FE8U = 0x8003D21
void        SetFont(FontData*);                                                                    //! FE8U = 0x8003D39

void        Text_Init(TextHandle*, size_t tileWidth);                                              //! FE8U = 0x8003D5D
void        Text_Allocate(TextHandle*, size_t tileWidth);                                          //! FE8U = 0x8003D85
void        InitTextBatch(const TextBatchEntry[]);                                                 //! FE8U = 0x8003DAD

void        Text_Clear(TextHandle*);                                                               //! FE8U = 0x8003DC9

int         Text_GetXCursor(TextHandle*);                                                          //! FE8U = 0x8003E51
TextHandle* Text_SetXCursor(TextHandle*, int);                                                     //! FE8U = 0x8003E55
TextHandle* Text_Advance(TextHandle*, int);                                                        //! FE8U = 0x8003E59

TextHandle* Text_SetColorId(TextHandle*, int);                                                     //! FE8U = 0x8003E61
int         Text_GetColorId(TextHandle*);                                                          //! FE8U = 0x8003E65

TextHandle* Text_SetParameters(TextHandle*, int cursor, int color);                                //! FE8U = 0x8003E69

void        Text_Draw(TextHandle*, uint16_t* bgMap);                                               //! FE8U = 0x8003E71
void        Text_DrawBlank(TextHandle*, uint16_t* bgMap);                                          //! FE8U = 0x8003EBD

size_t      GetStringTextWidth(const char*);                                                       //! FE8U = 0x8003EDD
size_t      GetCharTextWidth(char);                                                                //! FE8U = 0x8003F3D
size_t      GetStringTextCenteredPos(const char*);                                                 //! FE8U = 0x8003F91

const char* String_GetEnd(const char*);                                                            //! FE8U = 0x8003FF5

void        Text_AppendString(TextHandle*, const char*);                                           //! FE8U = 0x8004005
void        Text_AppendDecNumber(TextHandle*, int);                                                //! FE8U = 0x8004075
void        Text_AppendNumberOr2Dashes(TextHandle*, uint8_t);                                      //! FE8U = 0x8004145
void        Text_AppendChar(TextHandle*, char);                                                    //! FE8U = 0x8004181

// TODO: figure more out
// (I only have vague knowledge on what most things past here does)

void*       GetVRAMPointerForTextMaybe(TextHandle*);                                               //! FE8U = 0x80041E9
const void* GetSomeTextDrawingRelatedTablePointer(int);                                            //! FE8U = 0x8004209
void        Font_StandardGlyphDrawer(TextHandle*, const void*);                                    //! FE8U = 0x8004219
void        Font_SpecializedGlyphDrawer(TextHandle*, const void*);                                 //! FE8U = 0x8004269

void        Font_LoadForUI(void);                                                                  //! FE8U = 0x80043A9
void        Font_LoadForDialogue(void);                                                            //! FE8U = 0x80043E9
void        Font_SetSomeSpecialDrawingRoutine(void);                                               //! FE8U = 0x8004429

void        DrawTextInline(TextHandle*, uint16_t* bg, int color, int xStart, const char* cstring); //! FE8U = 0x800443D
void        Text_InsertString(TextHandle*, int xPos, int color, const char* str);                  //! FE8U = 0x8004481
void        Text_InsertNumberOr2Dashes(TextHandle*, int xPos, int color, uint8_t);                 //! FE8U = 0x80044A5

void        Text_AppendStringSimple(TextHandle*, const char*);                                     //! FE8U = 0x80044C9
void        Text_AppendCharSimple(TextHandle*, char);                                              //! FE8U = 0x8004505

size_t      GetCharTextWidthSimple(char);                                                          //! FE8U = 0x8004539
size_t      GetStringTextWidthSimple(const char*);                                                 //! FE8U = 0x8004569

void        InitSomeOtherGraphicsRelatedStruct(FontData*, void* vram, int pal);                    //! FE8U = 0x800459D
void        Text_Init3(TextHandle*);                                                               //! FE8U = 0x80045D9
void        Text_80046B4(TextHandle*);                                                             //! FE8U = 0x80046B5

#pragma long_calls_off

#endif // GBAFE_TEXT_H
