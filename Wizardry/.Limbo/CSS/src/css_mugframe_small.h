#ifndef HAX_CSS_MUGFRAME_SMALL_H
#define HAX_CSS_MUGFRAME_SMALL_H

#include "css_common.h"

typedef struct _CSSMugFrameSmallProc CSSMugFrameSmallProc;

struct _CSSMugFrameSmallProc {
	PROC_HEADER;

	uint16_t rootTileIndex;
	uint16_t* pBgOutput;
};

CSSMugFrameSmallProc* css_mugframe_small_start(uint16_t* pBgOut, uint16_t rootTileIndex);
void                  css_mugframe_small_end(CSSMugFrameSmallProc*);
void                  css_mugframe_small_redraw(CSSMugFrameSmallProc*);

#endif // HAX_CSS_MUGFRAME_SMALL_H
