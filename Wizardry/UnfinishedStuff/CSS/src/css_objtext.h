#ifndef HAX_CSS_OBJTEXT_H
#define HAX_CSS_OBJTEXT_H

#include "css_common.h"

typedef struct _CSSStatTextProc CSSStatTextProc;

struct _CSSStatTextProc {
	PROC_HEADER;

	uint8_t statDrawFlags[8];
	Vector2 origin;

//	uint8_t statValues[0x10];

//	uint8_t paletteIndex;
//	const uint16_t* pObjTileLookup;
};

CSSStatTextProc* css_stattext_start(Vector2 origin);
void             css_stattext_end(CSSStatTextProc*);

void css_stattext_show(CSSStatTextProc*, int id, const char* name, int value, int maxValue);
void css_stattext_hide(CSSStatTextProc*, int id);

#endif // HAX_CSS_OBJTEXT_H
