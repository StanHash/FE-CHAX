#ifndef HAX_CSS_CORE_H
#define HAX_CSS_CORE_H

#include "css_common.h"
#include "css_lpanel.h"

typedef struct _CSSCoreProc   CSSCoreProc;

struct _CSSCoreProc {
	PROC_HEADER;

	CSSLPanelProc* pLPanel;
};

#endif // HAX_CSS_CORE_H
