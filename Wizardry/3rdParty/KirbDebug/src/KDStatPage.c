#include "KirbDebug.h"

struct StatPageProc {
	PROC_HEADER;

	u8 stats[UnitEditor_PAGE1ENTRIES];
	u8 classId;
	u16 cursor;
};

static struct Proc* KDStartStatPage(UnitEditorProc* parent);
static void KDStatPageLoadUnit(struct Proc*, UnitEditorProc*, struct Unit*);
static void KDStatPageSaveUnit(struct Proc*, UnitEditorProc*, struct Unit*);
static void KDStatPageOnLoop(struct Proc*);

static const struct ProcInstruction sProc_KDStatPage[] = {
	PROC_SET_NAME("Kirb:DUE:StatPage"),

	PROC_LOOP_ROUTINE(KDStatPageOnLoop),

	PROC_END
};

const struct KDPageDefinition gKDStatPageDefinition = {
	.start = KDStartStatPage,

	.onLoadUnit = KDStatPageLoadUnit,
	.onSaveUnit = KDStatPageSaveUnit,
};

struct StatPageFieldDefinition {
	struct Vector2U position;
	const char* label;
};

const struct StatPageFieldDefinition sKDStatPageFields[] = {
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 1  }, "power"      }, // 0
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 2  }, "skill"      }, // 1
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 3  }, "speed"      }, // 2
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 4  }, "luck"       }, // 3
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 5  }, "defense"    }, // 4
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 6  }, "resistance" }, // 5
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 7  }, "max hp"     }, // 6
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 8  }, "current hp" }, // 7
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 9  }, "con"        }, // 8
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 10 }, "mov"        }, // 9
	{ { KD_PAGE_FRAME_X + 1,  KD_PAGE_FRAME_Y + 11 }, "status"     }, // 10
	{ { KD_PAGE_FRAME_X + 13, KD_PAGE_FRAME_Y + 11 }, "duration"   }, // 11
};

static struct Proc* KDStartStatPage(UnitEditorProc* parent) {
	struct StatPageProc* proc = (struct StatPageProc*) StartProc(sProc_KDStatPage, (struct Proc*) parent);

	for (unsigned i = 0; i < UnitEditor_PAGE1ENTRIES; ++i)
		proc->stats[i] = 0;

	proc->classId = 0;
	proc->cursor = 0;

	for (unsigned i = 0; i < UnitEditor_PAGE1ENTRIES; ++i)
		DBG_BG_Print(
			BG_LOCATED_TILE(gBg0MapBuffer, sKDStatPageFields[i].position.x + 4, sKDStatPageFields[i].position.y),
			sKDStatPageFields[i].label
		);

	return (struct Proc*) proc;
}

static void KDStatPageLoadUnit(struct Proc* p, UnitEditorProc* parent, struct Unit* unit) {
	struct StatPageProc* const proc = (struct StatPageProc*) p;

	proc->stats[0]  = unit->pow;
	proc->stats[1]  = unit->skl;
	proc->stats[2]  = unit->spd;
	proc->stats[3]  = unit->lck;
	proc->stats[4]  = unit->def;
	proc->stats[5]  = unit->res;
	proc->stats[6]  = unit->maxHP;
	proc->stats[7]  = unit->curHP;
	proc->stats[8]  = unit->conBonus;
	proc->stats[9]  = unit->movBonus;
	proc->stats[10] = unit->statusIndex;
	proc->stats[11] = unit->statusDuration;

	proc->classId   = unit->pClassData->number;
}

static void KDStatPageSaveUnit(struct Proc* p, UnitEditorProc* parent, struct Unit* unit) {
	struct StatPageProc* const proc = (struct StatPageProc*) p;

	unit->pow = proc->stats[0];
	unit->skl = proc->stats[1];
	unit->spd = proc->stats[2];
	unit->lck = proc->stats[3];
	unit->def = proc->stats[4];
	unit->res = proc->stats[5];
	unit->maxHP = proc->stats[6];
	unit->curHP = proc->stats[7];
	unit->conBonus = proc->stats[8];
	unit->movBonus = proc->stats[9];
	unit->statusIndex = proc->stats[10];
	unit->statusDuration = proc->stats[11];
	unit->pClassData = GetClassData(proc->classId);
}

static void KDStatPageOnLoop(struct Proc* p) {
	struct StatPageProc* const proc = (struct StatPageProc*) p;

	//Get the class name and prints it
	if (proc->classId) {
		char buf[0x20];

		String_GetFromIndexExt(GetClassData(proc->classId)->nameTextId, buf);
		PrintDebugStringAsOBJ(48, 128, buf);
	}

	//Prints the stats
	for (unsigned i = 0; i < UnitEditor_PAGE1ENTRIES; ++i) {
		PrintDebugNumberDec(
			8 * sKDStatPageFields[i].position.x,
			8 * sKDStatPageFields[i].position.y,
			proc->stats[i], 3
		);
	}

	PrintDebugNumberHex(16, 128, proc->classId, 3);

	//*

	UpdateHandCursor(
		sKDStatPageFields[proc->cursor].position.x * 8,
		sKDStatPageFields[proc->cursor].position.y * 8 - 4
	); // */
}
