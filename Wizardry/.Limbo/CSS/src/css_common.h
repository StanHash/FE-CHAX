#ifndef HAX_CSS_COMMON_H
#define HAX_CSS_COMMON_H

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

extern const ProcCode proc_css[];
extern const ProcCode proc_css_lpanel[];

// Resources definition

extern const uint16_t css_stattext_pal[];

extern const uint8_t  css_mugframe_small_gfx[];
extern const uint16_t css_mugframe_small_pal[];

#endif // HAX_CSS_COMMON_H
