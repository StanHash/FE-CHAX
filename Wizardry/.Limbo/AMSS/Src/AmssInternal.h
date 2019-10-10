#ifndef AMSS_INTERNAL_INCLUDED
#define AMSS_INTERNAL_INCLUDED

#include "Amss.h"

extern const struct AmssModuleEntry AmssCoreModules[];

extern const struct ProcInstruction AmssMainProcScr[];

struct AmssMainProc* AmssGetMain(void);

void AmssChangeUnit(struct AmssMainProc* proc, struct Unit* unit);

void AmssStartUnitChange(int direction, struct Unit* next, struct AmssMainProc* parent);

void AmssDebugClear(void);
void AmssDebugInit(void);
void AmssDebugDisplayRect(struct AmssGeometry rect);
void AmssDebugColorRect(struct AmssGeometry rect, int color);
void AmssDebugDisplayHelps(void);

#endif // AMSS_INTERNAL_INCLUDED
