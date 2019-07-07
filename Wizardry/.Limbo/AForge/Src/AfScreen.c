
#include "Af.h"

void SlInit(unsigned textTileBase, unsigned bgPal, unsigned portrait, struct Proc* proc);
void SlEnd(void);
void SlStartDialogue(const char* cstring, struct Proc* proc);

// TODO: add to CLib reference
// static void(* const StartSafeFade)(int kind, int duration, struct Proc* parent, void(*onEnd)(void)) = (void(*)(int, int, struct Proc*, void(*)(void)))(0x08014098+1);
static void(* const StartSafeFadeOutBlack)(struct Proc* parent) = (const void*)(0x08013FC4+1);
static void(* const StartSafeFadeInBlack)(struct Proc* parent) = (const void*)(0x08013F58+1);
static void(* const ShopFadeIn)(struct Proc* parent) = (const void*)(0x080B4B7C+1);
static void(* const ShopFadeOut)(struct Proc* parent) = (const void*)(0x080B4BB0+1);

enum
{
	// Tile indices
	AF_TILE_DTXT = 0x200,
	AF_TILE_MTXT = 0x180,

	// Bg Palettes
	AF_BPAL_BACK = 14,
};

struct AfScreenProc
{
	PROC_HEADER;

	/* 2C */ struct Unit* unit;
};

enum
{
	// AfScreen proc labels
	AfScreen_LFirst,
	AfScreen_LChooseWeapon,
	AfScreen_LNoWeapon,
	AfScreen_LBye,
	AfScreen_LEnd,
};

// Utility function
static int UnitHasForgeableItem(struct Unit* unit)
{
	for (unsigned i = 0; i < UNIT_ITEM_COUNT; ++i)
	{
		if (unit->items[i] && AfCanForgeItem(unit->items[i]))
			return TRUE;
	}

	return FALSE;
}

static void AfScreen_OnInit(struct AfScreenProc* proc);

static void AfScreen_Greetings(struct AfScreenProc* proc);

static void AfScreen_FirstPrompt(struct AfScreenProc* proc);
static void AfScreen_HandleFirstPrompt(struct AfScreenProc* proc);

static void AfScreen_WeaponPrompt(struct AfScreenProc* proc);
static void AfScreen_WeaponPromptMenu(struct AfScreenProc* proc);

static void AfScreen_NoWeapon(struct AfScreenProc* proc);

static void AfScreen_Goodbye(struct AfScreenProc* proc);

static const struct ProcInstruction sProc_AForgeScreen[] = {
	PROC_SET_NAME("Stan : A Forge"),

	PROC_CALL_ROUTINE(ShopFadeIn),
	PROC_YIELD,

	PROC_CALL_ROUTINE(AfScreen_OnInit),

	PROC_CALL_ROUTINE(LockGameGraphicsLogic),

	PROC_CALL_ROUTINE(StartSafeFadeOutBlack),
	PROC_YIELD,

	PROC_CALL_ROUTINE(AfScreen_Greetings),
	PROC_YIELD,

PROC_LABEL(AfScreen_LFirst),
	PROC_CALL_ROUTINE(AfScreen_FirstPrompt),
	PROC_YIELD,

	PROC_CALL_ROUTINE(AfScreen_HandleFirstPrompt),

PROC_LABEL(AfScreen_LNoWeapon),
	PROC_CALL_ROUTINE(AfScreen_NoWeapon),
	PROC_YIELD,

	PROC_GOTO(AfScreen_LBye),

PROC_LABEL(AfScreen_LChooseWeapon),
	PROC_CALL_ROUTINE(AfScreen_WeaponPrompt),
	PROC_YIELD,

	PROC_CALL_ROUTINE(AfScreen_WeaponPromptMenu),
	PROC_YIELD,

PROC_LABEL(AfScreen_LBye),
	PROC_CALL_ROUTINE(AfScreen_Goodbye),
	PROC_YIELD,

PROC_LABEL(AfScreen_LEnd),
	PROC_CALL_ROUTINE(StartSafeFadeInBlack),
	PROC_YIELD,

	PROC_CALL_ROUTINE(SlEnd),
	PROC_CALL_ROUTINE(UnlockGameGraphicsLogic),

	PROC_CALL_ROUTINE(ShopFadeOut),
	PROC_YIELD,

	PROC_END
};

void AfStartScreen(struct Proc* parent)
{
	struct AfScreenProc* proc = NULL;

	if (parent)
		proc = START_PROC_BLOCKING(sProc_AForgeScreen, parent);
	else
		proc = START_PROC(sProc_AForgeScreen, ROOT_PROC_3);

	proc->unit = gActiveUnit;
}

static void AfScreen_OnInit(struct AfScreenProc* proc)
{
	SlInit(AF_TILE_DTXT, AF_BPAL_BACK, 0x65, (Proc*)(proc));
}

static void AfScreen_Greetings(struct AfScreenProc* proc)
{
	SlStartDialogue("Welcome to the blacksmith!\x03", (Proc*)(proc));
}

static void AfScreen_FirstPrompt(struct AfScreenProc* proc)
{
	SlStartDialogue("Are you here to request a forge?\x01\x18", (Proc*)(proc));
}

static void AfScreen_HandleFirstPrompt(struct AfScreenProc* proc)
{
	ProcGoto((Proc*)(proc),
		(GetLastDialoguePromptResult() == 1 /* 1 is YES, 2 is NO, 0 is backed out */
			? (UnitHasForgeableItem(proc->unit)
				? AfScreen_LChooseWeapon
				: AfScreen_LNoWeapon)
			: AfScreen_LBye));
}

static void AfScreen_WeaponPrompt(struct AfScreenProc* proc)
{
	SlStartDialogue("Which weapon do you want\x01me to forge?", (Proc*)(proc));
}

static int ItemSelectAvailable(const MenuCommandDefinition* def, int id)
{
	int item = gActiveUnit->items[id];

	return item
		? (AfCanForgeItem(item)
			? MCA_USABLE
			: MCA_GRAYED)
		: MCA_NONUSABLE;
	// */
}

static void ItemSelectDraw(MenuProc* menu, MenuCommandProc* cmd)
{
	// int item = gActiveUnit->items[cmd->commandDefinitionIndex];

	/*

	DrawItemMenuLineLong(
		&cmd->text,
		item,
		!AfCanForgeItem(item),
		TILEMAP_LOCATED(GetBgMapBuffer(menu->frontBgId), cmd->xDrawTile, cmd->yDrawTile));

	// */
}

#define CMD { .rawName = "=)", .isAvailable = ItemSelectAvailable, .onDraw = ItemSelectDraw }

static const
struct MenuCommandDefinition sMenuCommand_ItemSelect[] = {
	CMD,
	CMD,
	CMD,
	CMD,
	CMD,

	{}
};

#undef CMD

static const
struct MenuDefinition sMenu_ItemSelect = {
	.geometry = { .x = 8, .y = 8, .w = 10 },
	.commandList = sMenuCommand_ItemSelect
};

static void AfScreen_WeaponPromptMenu(struct AfScreenProc* proc)
{
	StartMenuExt(
		&sMenu_ItemSelect,
		sMenu_ItemSelect.geometry,
		1,
		AF_TILE_MTXT,
		2,
		0,
		(Proc*)(proc));
}

static void AfScreen_NoWeapon(struct AfScreenProc* proc)
{
	SlStartDialogue("You don't seem to hold any\x01weapon I can work with.\x03", (Proc*)(proc));
}

static void AfScreen_Goodbye(struct AfScreenProc* proc)
{
	SlStartDialogue("Come back again.\x03", (Proc*)(proc));
}
