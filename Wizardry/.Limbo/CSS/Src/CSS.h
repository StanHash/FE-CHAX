#ifndef HAX_CSS_H
#define HAX_CSS_H

#include "gbafe.h"

#define CSS_PREFIX "Stan:CStatScreen"

#define CSS_PALID_BG_LPANEL 1
#define CSS_PALID_BG_RPANEL 2

enum {
	CSS_PAL_BG_TEXT      = 0x00,
	CSS_PAL_BG_MUG       = 0x01,

	CSS_PAL_OBJ_STATTEXT = 0x10,
	CSS_PAL_OBJ_MUG      = 0x11,
};

enum {
	CSS_TILE_OBJ_STATTEXT = 0x180,
};

extern const struct ProcInstruction proc_css[];
extern const struct ProcInstruction proc_css_lpanel[];

// TODO: maybe move to fe-clib?
extern struct Unit* gpStatScreenUnit;

// Resources definition

extern const u16 css_stattext_pal[];

extern const u8  css_lpbg_face_img[];
extern const u16 css_lpbg_face_pal[];

// CSSCommon

char* css_num2str(char* out, int number);
void css_text_append_number(struct TextHandle* text, int number);
void css_text_append_number_2digit(struct TextHandle* text, int number);
void css_display_hp_exp_line(u16* bgOut);

// CSSObjText

struct CSSStatTextProc {
	PROC_HEADER;

	/* 29 */ u8 statDrawFlags[8];
	/* 31 */ /* pad */
	/* 34 */ struct Vector2 origin;
	/* 38 */ /* end */

	// u8 statValues[0x10];

	// u8 paletteIndex;
	// const u8* pObjTileLookup;
};

struct CSSStatTextProc* css_stattext_start(Vector2 origin);
void                    css_stattext_end(struct CSSStatTextProc*);

void css_stattext_show(struct CSSStatTextProc*, int id, const char* name, int value, int maxValue);
void css_stattext_hide(struct CSSStatTextProc*, int id);

// CSSMugFrameSmall

struct CSSMugFrameSmallProc {
	PROC_HEADER;

	uint16_t rootTileIndex;
	uint16_t* pBgOutput;
};

struct CSSMugFrameSmallProc* css_mugframe_small_start(uint16_t* pBgOut, uint16_t rootTileIndex);
void                         css_mugframe_small_end(struct CSSMugFrameSmallProc*);
void                         css_mugframe_small_redraw(struct CSSMugFrameSmallProc*);

// CSSLeftPanel

struct CSSLeftPanelProc {
	PROC_HEADER;
	uint8_t faceDisplayed;

	struct CSSStatTextProc* pStatText;

	Vector2 origin;
};

struct CSSLeftPanelProc* css_lpanel_start(Vector2 origin, Proc*);
void                     css_lpanel_end(struct CSSLeftPanelProc*);

int  css_lpanel_is_face_displayed(struct CSSLeftPanelProc*);
void css_lpanel_set_face_displayed(struct CSSLeftPanelProc*, int);

// CSSCore

struct CSSCoreProc {
	PROC_HEADER;

	struct CSSLeftPanelProc* pLPanel;
};

#endif // HAX_CSS_H
