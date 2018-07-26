#ifndef GBAFE_MENU_H
#define GBAFE_MENU_H

// bmmenu.c?

#include "proc.h"

typedef struct MenuGeometry MenuGeometry;

typedef struct MenuDefinition MenuDefinition;
typedef struct MenuCommandDefinition MenuCommandDefinition;

typedef struct MenuProc MenuProc;
typedef struct MenuCommandProc MenuCommandProc;

struct MenuGeometry {
	u8 x, y, h, w;
};

struct MenuDefinition {
	struct MenuGeometry geometry;

	u8 style;

	const struct MenuCommandDefinition* commandList;

	void(*onInit)(MenuProc*); // 0C
	void(*onEnd)(MenuProc*); // 10
	void(*_u14)(MenuProc*); // 14
	void(*onBPress)(MenuProc*, MenuCommandProc*); // 18
	void(*onRPress)(MenuProc*); // 1C
	void(*onHelpBox)(MenuProc*, MenuCommandProc*); // 20
};

struct MenuCommandDefinition {
	const char* rawName;

	u16 nameId, helpId;
	u8 colorId, _u09;

	int(*isAvailable)(const MenuCommandDefinition*, int);

	void(*onDraw)(MenuProc*, MenuCommandProc*);
	
	int(*onEffect)(MenuProc*, MenuCommandProc*);
	int(*onIdle)(MenuProc*, MenuCommandProc*);

	void(*onSwitchIn)(MenuProc*, MenuCommandProc*);
	void(*onSwitchOut)(MenuProc*, MenuCommandProc*);
};

struct MenuProc {
	PROC_FIELDS

	struct MenuGeometry geometry;
	const MenuDefinition* pDefinition;

	struct MenuCommandProc* pCommandProc[11];

	u8 commandCount;
	u8 commandIndex;
	u8 prevCommandIndex;
	u8 stateBits;

	u8 backBgId : 3;
	u8 frontBgId : 3;

	u16 tileBase;
	u16 _u68;
};

struct MenuCommandProc {
	PROC_FIELDS

	u16 xDrawTile;
	u16 yDrawTile;

	const struct MenuCommandDefinition* pDefinition;

	u32 _temp[2]; // TODO: TextHandle

	u8 commandDefinitionIndex;
	u8 availability;
};

enum MenuCommandAvailability {
	MCA_USABLE = 1,
	MCA_GRAYED = 2,
	MCA_NONUSABLE = 3
};

enum MenuEffect {
	ME_NONE = 0x00,

	ME_DISABLE = 0x01,
	ME_END = 0x02,
	ME_PLAY_BEEP = 0x04,
	ME_PLAY_BOOP = 0x08,
	ME_CLEAR_GFX = 0x10,
	ME_END_FACE0 = 0x20,
	ME_END_AFTER = 0x80
};

#pragma long_calls

// TODO: move to ui.h
void UpdateHandCursor(int x, int y); //! FE8U = (0804E79C+1)

MenuProc* StartMenuAdjusted(const MenuDefinition*, int xScreen, int xLeft, int xRight); //! FE8U = 0x804EB99
MenuProc* StartMenuChild(const MenuDefinition*, Proc* parent); //! FE8U = 0x804EBC9
MenuProc* StartMenu(const MenuDefinition*); //! FE8U = 0x804EBE5
MenuProc* StartMenuExt2(const MenuDefinition*, int backBgId, u16 baseTile, int frontBgId, int idk, Proc* parent); //! FE8U = 0x804EC35
MenuProc* StartMenuAt(const MenuDefinition*, MenuGeometry, Proc* parent); //! FE8U = 0x804EC99
MenuProc* StartMenuExt(const MenuDefinition*, MenuGeometry, int backBgId, u16 baseTile, int frontBgId, int idk, Proc* parent); //! FE8U = 0x804ECB1

Proc* EndMenu(MenuProc*); //! FE8U = 0x804EEA9
void EndAllMenus(MenuProc*); //! FE8U = 0x804EF21

// internal stuff
// Menu_CallDefinedConstructors //! FE8U = 0x804EF39
// Menu_Draw //! FE8U = 0x804EF71
// Menu_DrawHoverThing //! FE8U = 0x804F0E1
// Menu_Idle //! FE8U = 0x804F165
// Menu_HandleDirectionInputs //! FE8U = 0x804F295
// Menu_HandleSelectInputs //! FE8U = 0x804F375
// Menu_GetCursorGfxCurrentPosition //! FE8U = 0x804F401

// default usability
int MenuCommandAlwaysUsable(const MenuCommandDefinition*, int); //! FE8U = 0x804F449
int MenuCommandAlwaysGrayed(const MenuCommandDefinition*, int); //! FE8U = 0x804F44D
int MenuCommandNeverUsable(const MenuCommandDefinition*, int); //! FE8U = 0x804F451

// menu state modifiers
void MenuCallHelpBox(MenuProc*, u16 textId); //! FE8U = 0x804F581

// void MarkSomethingInMenu(void); // Draw Cursor while disabled //! FE8U = 0x804F60D

// MenuProc* StartMenu_AndDoSomethingCommands(const MenuDefinition*, int xScreen, int xLeft, int xRight); //! FE8U = 0x804F64D
// void Menu_UpdateMovingCursorGfxPosition(MenuProc*, int, int*); //! FE8U = 0x804F6A5
// void ClearMenuRelatedList(void); //! FE8U = 0x804F6F9

#pragma long_calls_off

#endif // GBAFE_MENU_H
