#ifndef GBAFE_MENU_H
#define GBAFE_MENU_H

// bmmenu.c?

#include "proc.h"

typedef struct _MenuGeometry          MenuGeometry;
typedef struct _MenuDefinition        MenuDefinition;
typedef struct _MenuCommandDefinition MenuCommandDefinition;
typedef struct _MenuProc              MenuProc;
typedef struct _MenuCommandProc       MenuCommandProc;

struct _MenuGeometry {
	uint8_t x, y, h, w;
};

struct _MenuDefinition {
	MenuGeometry geometry;

	uint8_t style;

	const MenuCommandDefinition* commandList;

	void(*onInit)(MenuProc*); // 0C
	void(*onEnd)(MenuProc*); // 10
	void(*_u14)(MenuProc*); // 14
	void(*onBPress)(MenuProc*, MenuCommandProc*); // 18
	void(*onRPress)(MenuProc*); // 1C
	void(*onHelpBox)(MenuProc*, MenuCommandProc*); // 20
};

struct _MenuCommandDefinition {
	const char* rawName;

	uint16_t nameId, helpId;
	uint8_t colorId, _u09;

	int(*isAvailable)(const MenuCommandDefinition*, int);

	void(*onDraw)(MenuProc*, MenuCommandProc*);
	
	int(*onEffect)(MenuProc*, MenuCommandProc*);
	int(*onIdle)(MenuProc*, MenuCommandProc*);

	void(*onSwitchIn)(MenuProc*, MenuCommandProc*);
	void(*onSwitchOut)(MenuProc*, MenuCommandProc*);
};

struct _MenuProc {
	ProcState header;

	MenuGeometry geometry;
	const MenuDefinition* pDefinition;

	MenuCommandProc* pCommandProc[11];

	uint8_t commandCount;
	uint8_t commandIndex;
	uint8_t prevCommandIndex;
	uint8_t stateBits;

	uint8_t backBgId  : 3;
	uint8_t frontBgId : 3;

	uint16_t tileBase;
	uint16_t _u68;
};

struct _MenuCommandProc {
	ProcState header;
};

enum _MenuCommandAvailability {
	MCA_USABLE    = 1,
	MCA_GRAYED    = 2,
	MCA_NONUSABLE = 3
};

enum _MenuEffect {
	ME_DISABLE   = 0x01,
	ME_END       = 0x02,
	ME_PLAY_BEEP = 0x04,
	ME_PLAY_BOOP = 0x08,
	ME_CLEAR_GFX = 0x10,
	ME_END_FACE0 = 0x20,
	ME_END_AFTER = 0x80
};

#pragma long_calls

// TODO: move to ui.h
void UpdateHandCursor(int x, int y); //! FE8U = (0804E79C+1)

MenuProc* NewMenu_DefaultAdjusted(const MenuDefinition*, int xScreen, int xLeft, int xRight); //! FE8U = 0x804EB99
MenuProc* NewMenu_DefaultChild(const MenuDefinition*, Proc* parent); //! FE8U = 0x804EBC9
MenuProc* NewMenu_Default(const MenuDefinition*); //! FE8U = 0x804EBE5
MenuProc* NewMenu_804EC34(const MenuDefinition*, int backBgId, uint16_t baseTile, int frontBgId, int idk, Proc* parent); //! FE8U = 0x804EC35
MenuProc* NewMenu_BG0BG1(const MenuDefinition*, MenuGeometry, Proc* parent); //! FE8U = 0x804EC99
MenuProc* NewMenu(const MenuDefinition*, MenuGeometry, int backBgId, uint16_t baseTile, int frontBgId, int idk, Proc* parent); //! FE8U = 0x804ECB1

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
int MenuCommand_UsabilityAlways(const MenuCommandDefinition*, int); //! FE8U = 0x804F449
int UsabilityGrayed(const MenuCommandDefinition*, int); //! FE8U = 0x804F44D
int UsabilityNever(const MenuCommandDefinition*, int); //! FE8U = 0x804F451

// menu state modifiers
void Menu_CallTextBox(MenuProc*, uint16_t textId); //! FE8U = 0x804F581

// void MarkSomethingInMenu(void); // Draw Cursor while disabled //! FE8U = 0x804F60D

// MenuProc* NewMenu_AndDoSomethingCommands(const MenuDefinition*, int xScreen, int xLeft, int xRight); //! FE8U = 0x804F64D
// void Menu_UpdateMovingCursorGfxPosition(MenuProc*, int, int*); //! FE8U = 0x804F6A5
// void ClearMenuRelatedList(void); //! FE8U = 0x804F6F9

#pragma long_calls_off

#define StartMenuExt NewMenu
#define StartMenuExt2 NewMenu_804EC34
#define StartMenuAt NewMenu_BG0BG1
#define StartMenuAdjusted NewMenu_DefaultAdjusted
#define StartMenuChild NewMenu_DefaultChild
#define StartMenu NewMenu_Default

#define MenuCommandAlwaysUsable MenuCommand_UsabilityAlways
#define MenuCommandAlwaysGrayed UsabilityGrayed
#define MenuCommandNeverUsable UsabilityNever

#define MenuCallHelpBox Menu_CallTextBox

#endif // GBAFE_MENU_H
