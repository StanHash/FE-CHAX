#ifndef HAX_CSS_LPANEL_H
#define HAX_CSS_LPANEL_H

#include "css_common.h"
#include "css_objtext.h"

typedef struct _CSSLPanelProc CSSLPanelProc;

struct _CSSLPanelProc {
	PROC_HEADER;
	uint8_t faceDisplayed;

	CSSStatTextProc* pStatText;

	Vector2 origin;
};

CSSLPanelProc* css_lpanel_start(Vector2 origin, Proc*);
void           css_lpanel_end(CSSLPanelProc*);

int  css_lpanel_is_face_displayed(CSSLPanelProc*);
void css_lpanel_set_face_displayed(CSSLPanelProc*, int);

#endif // HAX_CSS_LPANEL_H