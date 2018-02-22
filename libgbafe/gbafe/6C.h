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

#define _6C_END                           { 0x00, 0x0000, 0 }
#define _6C_SET_NAME(aName)               { 0x01, 0x0000, aName }
#define _6C_CALL_ROUTINE(apRoutine)       { 0x02, 0x0000, (void*)(apRoutine) }
#define _6C_LOOP_ROUTINE(apRoutine)       { 0x03, 0x0000, (void*)(apRoutine) }
#define _6C_SET_DESTRUCTOR(apRoutine)     { 0x04, 0x0000, (void*)(apRoutine) }
#define _6C_NEW_CHILD(ap6CChild)          { 0x05, 0x0000, (void*)(ap6CChild) }
#define _6C_NEW_CHILD_BLOCKING(ap6CChild) { 0x06, 0x0000, (void*)(ap6CChild) }
#define _6C_NEW_MAIN_BUGGED(ap6CMain)     { 0x07, 0x0000, ap6CMain }
#define _6C_WHILE_EXISTS(ap6CToCheck)     { 0x08, 0x0000, (void*)(ap6CToCheck) }
#define _6C_END_ALL(ap6CToCheck)          { 0x09, 0x0000, (void*)(ap6CToCheck) }
#define _6C_BREAK_ALL_LOOP(ap6CToCheck)   { 0x0A, 0x0000, (void*)(ap6CToCheck) }
#define _6C_LABEL(aLabelId)               { 0x0B, aLabelId, 0 }
#define _6C_GOTO(aLabelId)                { 0x0C, aLabelId, 0 }
#define _6C_JUMP(ap6CCode)                { 0x0D, 0x0000, (void*)(ap6CCode) }
#define _6C_SLEEP(aTime)                  { 0x0E, aTime,  0 }
#define _6C_SET_MARK(aMark)               { 0x0F, aMark,  0 }
#define _6C_BLOCK                         { 0x10, 0x0000, 0 }
#define _6C_END_IF_DUPLICATE              { 0x11, 0x0000, 0 }
#define _6C_SET_BIT4                      { 0x12, 0x0000, 0 }
#define _6C_13                            { 0x13, 0x0000, 0 }
#define _6C_WHILE_ROUTINE(aprRoutine)     { 0x14, 0x0000, (void*)(aprRoutine) }
#define _6C_15                            { 0x15, 0x0000, 0 }
#define _6C_CALL_ROUTINE_2(aprRoutine)    { 0x16, 0x0000, (void*)(aprRoutine) }
#define _6C_END_DUPLICATES                { 0x17, 0x0000, 0 }
#define _6C_CALL_ROUTINE_ARG(aprRoutine, aArgument) { 0x18, aArgument, (void*)(aprRoutine) }
#define _6C_19                            { 0x19, 0x0000, 0 }

#define _6C_YIELD                         _6C_SLEEP(0)

#endif // GBAFE_6C_H
