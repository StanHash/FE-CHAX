#ifndef GBAFE_HARDWARE_H
#define GBAFE_HARDWARE_H

#include "gba/gba.h"

#include "common.h"
#include "proc.h"

typedef struct LCDIOBuffer LCDIOBuffer;
typedef struct KeyStatusBuffer KeyStatusBuffer;

struct LCDIOBuffer {
	struct DispCnt dispControl;
	struct DispStat dispStat;

	u32 _u08;

	struct BgCnt bgControl[4];
	Vector2U bgOffset[4];
	
	u16 win0h;
	u16 win1h;
	u16 win0v;
	u16 win1v;

	struct WinCnt winControl;
	
	u16 mosaic;
	
	struct BlendCnt blendControl;
	
	u32 _u40;
	u8 blendCoeffA;
	u8 blendCoeffB;
	u8 blendY;
	
	u16 bg2pa;
	u16 bg2pb;
	u16 bg2pc;
	u16 bg2pd;
	u32 bg2x;
	u32 bg2y;

	u16 bg3pa;
	u16 bg3pb;
	u16 bg3pc;
	u16 bg3pd;
	u32 bg3x;
	u32 bg3y;

	s8 colorAddition;
};

struct KeyStatusBuffer {
	u8 repeatDelay; // initial delay before generating auto-repeat presses
	u8 repeatInterval; // time between auto-repeat presses
	u8 repeatTimer; // (decreased by one each frame, reset to repeatDelay when Presses change and repeatInterval when reaches 0)
	u16 heldKeys; // keys that are currently held down
	u16 repeatedKeys; // auto-repeated keys
	u16 pressedKeys; // keys that went down this frame
	u16 prevKeys; // keys that were held down last frame
	u16 lastPressKeys; // keys that were held down last time a new key was
	u16 ABLRPressed; // 1 for Release (A B L R Only), 0 Otherwise
	u16 newKeys2;
	u16 timeSinceNonStartSelect; // Time since last Non-Start Non-Select Button was pressed
};

extern struct LCDIOBuffer gLCDIOBuffer; //! FE8U = 0x3003080

extern u16 gPaletteBuffer[0x200]; //! FE8U = 0x20228A8

extern u16 gBg0MapBuffer[0x400]; //! FE8U = 0x2022CA8
extern u16 gBg1MapBuffer[0x400]; //! FE8U = 0x20234A8
extern u16 gBg2MapBuffer[0x400]; //! FE8U = 0x2023CA8
extern u16 gBg3MapBuffer[0x400]; //! FE8U = 0x20244A8

extern u16* gBgMapTarget[4]; //! FE8U = 0x2024CA8

extern struct KeyStatusBuffer gKeyStatus; //! FE8U = 0x2024CC0

#define BG_LOCATED_TILE(apBg, aXTile, aYTile) (&(apBg)[(aXTile) + (aYTile) * 0x20])

#pragma long_calls

void CopyToPaletteBuffer(const u16 src[], unsigned targetOffset, unsigned size); //! FE8U = 0x8000DB9

void SyncLCDControl(void); //! FE8U = 0x8000E9D

u32 GetBgControlBuffer(int); //! FE8U = 0x8000F45
u32 GetBgTileDataOffset(int); //! FE8U = 0x8000F8D
int GetBgLocalTileIndex(int); //! FE8U = 0x8000FA5
void SetBgTileDataOffset(u32); //! FE8U = 0x8000FDD
void SetBgMapDataOffset(u32); //! FE8U = 0x8001005
void SetBgScreenSize(int); //! FE8U = 0x800104D

void SyncPalettes_AdditiveFilter(int); //! FE8U = 0x8001095
void SyncPalettes_SubstractiveFilter(int); //! FE8U = 0x80010F1

void SyncBgs(void); //! FE8U = 0x800114D
void FillBgMap(u16 map[], u16); //! FE8U = 0x8001221

void RegisterBlankTile(int index); //! FE8U = 0x8001241

void SetInterrupt_LCDVBlank(void(*)(void)); //! FE8U = 0x8001259
void SetInterrupt_LCDVCountMatch(void(*)(void)); //! FE8U = 0x80012AD
void SetLCDVCountSetting(int); //! FE8U = 0x800131D

void SetMainCallback(void(*)(void)); //! FE8U = 0x8001329
void ExecMainCycle(void); //! FE8U = 0x8001335

void UpdateKeyStatus_Private(KeyStatusBuffer*, u16); //! FE8U = 0x800134D
void UpdateKeyStatus(KeyStatusBuffer*); //! FE8U = 0x80013E1
void ResetKeyStatus(void); //! FE8U = 0x8001421
void SetKeyStatusIgnore(u16); //! FE8U = 0x8001441
u16 GetKeyStatusIgnore(); //! FE8U = 0x800144D
void KeyStatusSetterProc_Set(Proc*); //! FE8U = 0x8001459
Proc* StartKeyStatusSetter(u16); //! FE8U = 0x8001471

void SetBgPosition(int bg, u16 x, u16 y); //! FE8U = 0x800148D
void LoadBgConfig(const u16[]); //! FE8U = 0x8001B59
u16* GetBgMapBuffer(int bg); //! FE8U = 0x8001C4D

int ShouldSkipHSScreen(void); //! FE8U = 0x8001C69

void UpdateHBlankHandlerState(void); //! FE8U = 0x8001D29
void SetPrimaryHBlankCallback(void(*)(void)); //! FE8U = 0x8001DD9
void SetSecondaryHBlankCallback(void(*)(void)); //! FE8U = 0x8001DED

int GetBgIdFromMapBuffer(const u16*); //! FE8U = 0x8001E01

void SetBgDepth(int bg, int depth); //! FE8U = 0x8001E6D
int GetBgDepth(int bg); //! FE8U = 0x8001E8D

void SetColorEffectsParameters(int id, int eva, int evb, int evy); //! FE8U = 0x8001EA1
void SetColorEffectsFirstTarget(int bg0, int bg1, int bg2, int bg3, int obj); //! FE8U = 0x8001ED1
void SetColorEffectsSecondTarget(int bg0, int bg1, int bg2, int bg3, int obj); //! FE8U = 0x8001F0D
void SetColorEffectBackdropFirstTarget(int); //! FE8U = 0x8001F49
void SetColorEffectBackdropSecondTarget(int); //! FE8U = 0x8001F65
void SetDefaultColorEffects(void); //! FE8U = 0x8001F81

void EnablePaletteSync(void); //! FE8U = 0x8001F95
void DisablePaletteSync(void); //! FE8U = 0x8001FA1
void EnableBgSyncByMask(u8 mask); //! FE8U = 0x8001FAD
void EnableBgSyncByIndex(int bg); //! FE8U = 0x8001FBD

#pragma long_calls_off

// better names
#define gLCDIOBuffer gLCDIOBuffer

#endif // GBAFE_HARDWARE_H
