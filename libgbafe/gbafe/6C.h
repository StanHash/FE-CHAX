#ifndef GBAFE_6C_H
#define GBAFE_6C_H

#include <stdint.h>

#include "common.h"

typedef struct _Proc Proc;
typedef struct _Proc Struct6C;

typedef struct _ProcCode ProcCode;
typedef struct _ProcCode Code6C;

struct _ProcCode {
	uint16_t code;
	uint16_t sArg;
	void*    lArg;
};

struct _Proc {
	ProcCode* codeStart;
	ProcCode* codeCurrent;
	void (*onDestruct)(Proc*);
	void (*onUpdate)(Proc*);
	const char* name;
	Proc* parent;
	Proc* firstChild;
	Proc* previous;
	Proc* next;
	uint16_t sleepClock;
	uint8_t  mark;
	uint8_t  state;
	uint8_t  blockCount;
};

#define ProcThread(aIndex) (Proc*)(aIndex)

#define _6C_END                       { 0x00, 0x0000, 0 }
#define _6C_SET_NAME(aName)           { 0x01, 0x0000, aName }
#define _6C_CALL_ROUTINE(apRoutine)   { 0x02, 0x0000, (void*)(apRoutine) }
#define _6C_LOOP_ROUTINE(apRoutine)   { 0x03, 0x0000, (void*)(apRoutine) }
#define _6C_SET_DESTRUCTOR(apRoutine) { 0x04, 0x0000, (void*)(apRoutine) }

#define _6C_SLEEP(aDuration)          { 0x0E, aDuration, 0 }
#define _6C_YIELD                     _6C_SLEEP(0)

#endif // GBAFE_6C_H
