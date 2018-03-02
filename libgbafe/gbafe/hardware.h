#ifndef GBAFE_HARDWARE_H
#define GBAFE_HARDWARE_H

#include "gba/gba.h"

#include "common.h"
#include "proc.h"

typedef struct _LCDIOBuffer LCDIOBuffer;
typedef struct _KeyStatusBuffer KeyStatusBuffer;

struct _LCDIOBuffer {
	struct DispCnt dispControl;
	struct DispStat dispStat;

	uint32_t _u08;

	struct BgCnt bgControl[4];
	Vector2U bgOffset[4];
	
	uint16_t win0h;
	uint16_t win1h;
	uint16_t win0v;
	uint16_t win1v;

	struct WinCnt winControl;
	
	uint16_t mosaic;
	
	struct BlendCnt blendControl;
	
	uint32_t _u40;
	uint8_t blendCoeffA;
	uint8_t blendCoeffB;
	uint8_t blendY;
	
	uint16_t bg2pa;
	uint16_t bg2pb;
	uint16_t bg2pc;
	uint16_t bg2pd;
	uint32_t bg2x;
	uint32_t bg2y;

	uint16_t bg3pa;
	uint16_t bg3pb;
	uint16_t bg3pc;
	uint16_t bg3pd;
	uint32_t bg3x;
	uint32_t bg3y;

	int8_t colorAddition;
};

struct _KeyStatusBuffer {
	uint8_t repeatDelay;     // initial delay before generating auto-repeat presses
	uint8_t repeatInterval;  // time between auto-repeat presses
	uint8_t repeatTimer;     // (decreased by one each frame, reset to repeatDelay when Presses change and repeatInterval when reaches 0)
	uint16_t heldKeys;       // keys that are currently held down
	uint16_t repeatedKeys;   // auto-repeated keys
	uint16_t pressedKeys;    // keys that went down this frame
	uint16_t prevKeys;       // keys that were held down last frame
	uint16_t lastPressKeys;  // keys that were held down last time a new key was
	uint16_t ABLRPressed;    // 1 for Release (A B L R Only), 0 Otherwise
	uint16_t newKeys2;
	uint16_t timeSinceNonStartSelect; // Time since last Non-Start Non-Select Button was pressed
};

extern LCDIOBuffer gLCDControlBuffer;  //! FE8U = 0x3003080

extern uint16_t gPaletteBuffer[0x200]; //! FE8U = 0x20228A8

extern uint16_t gBg0MapBuffer[0x400];  //! FE8U = 0x2022CA8
extern uint16_t gBg1MapBuffer[0x400];  //! FE8U = 0x20234A8
extern uint16_t gBg2MapBuffer[0x400];  //! FE8U = 0x2023CA8
extern uint16_t gBg3MapBuffer[0x400];  //! FE8U = 0x20244A8

extern uint16_t* gBgMapTarget[4];      //! FE8U = 0x2024CA8

extern KeyStatusBuffer gKeyStatus;     //! FE8U = 0x2024CC0

#pragma long_calls

void      CopyToPaletteBuffer(const uint16_t src[], size_t targetOffset, size_t size); //! FE8U = 0x8000DB9

void      SyncLCDControl(void);                                //! FE8U = 0x8000E9D

uint32_t  GetBgControlBuffer(int);                             //! FE8U = 0x8000F45
uint32_t  GetBgTileDataOffset(int);                            //! FE8U = 0x8000F8D
int       GetBgLocalTileIndex(int);                            //! FE8U = 0x8000FA5
void      SetBgTileDataOffset(uint32_t);                       //! FE8U = 0x8000FDD
void      SetBgMapDataOffset(uint32_t);                        //! FE8U = 0x8001005
void      SetBgScreenSize(int);                                //! FE8U = 0x800104D

void      SyncPalettes_AdditiveFilter(int);                    //! FE8U = 0x8001095
void      SyncPalettes_SubstractiveFilter(int);                //! FE8U = 0x80010F1

void      SyncBgs(void);                                       //! FE8U = 0x800114D
void      FillBgMap(uint16_t map[], uint16_t);                 //! FE8U = 0x8001221

void      RegisterBlankTile(int index);                        //! FE8U = 0x8001241

void      SetInterrupt_LCDVBlank(void(*)(void));               //! FE8U = 0x8001259
void      SetInterrupt_LCDVCountMatch(void(*)(void));          //! FE8U = 0x80012AD
void      SetLCDVCountSetting(int);                            //! FE8U = 0x800131D

void      SetMainCallback(void(*)(void));                      //! FE8U = 0x8001329
void      ExecMainCycle(void);                                 //! FE8U = 0x8001335

void      UpdateKeyStatus_Private(KeyStatusBuffer*, uint16_t); //! FE8U = 0x800134D
void      UpdateKeyStatus(KeyStatusBuffer*);                   //! FE8U = 0x80013E1
void      ResetKeyStatus(void);                                //! FE8U = 0x8001421
void      SetKeyStatusIgnore(uint16_t);                        //! FE8U = 0x8001441
uint16_t  GetKeyStatusIgnore();                                //! FE8U = 0x800144D
void      KeyStatusSetterProc_Set(Proc*);                      //! FE8U = 0x8001459
Proc*     StartKeyStatusSetter(uint16_t);                      //! FE8U = 0x8001471

void      SetBgPosition(int bg, uint16_t x, uint16_t y);       //! FE8U = 0x800148D
void      LoadBgConfig(const uint16_t[]);                      //! FE8U = 0x8001B59
uint16_t* GetBgMapBuffer(int bg);                              //! FE8U = 0x8001C4D

int       ShouldSkipHSScreen(void);                            //! FE8U = 0x8001C69

void      UpdateHBlankHandlerState(void);                      //! FE8U = 0x8001D29
void      SetPrimaryHBlankCallback(void(*)(void));             //! FE8U = 0x8001DD9
void      SetSecondaryHBlankCallback(void(*)(void));           //! FE8U = 0x8001DED

int       GetBgIdFromMapBuffer(const uint16_t*);               //! FE8U = 0x8001E01

void      SetBgDepth(int bg, int depth);                       //! FE8U = 0x8001E6D
int       GetBgDepth(int bg);                                  //! FE8U = 0x8001E8D

void      SetSpecialColorEffectsParameters(int id, int eva, int evb, int evy); //! FE8U = 0x8001EA1
void      SetDefaultColorEffects(void);                        //! FE8U = 0x8001F81

void      EnablePaletteSync(void);                             //! FE8U = 0x8001F95
void      DisablePaletteSync(void);                            //! FE8U = 0x8001FA1
void      EnableBgSyncByMask(uint8_t mask);                    //! FE8U = 0x8001FAD
void      EnableBgSyncByIndex(int bg);                         //! FE8U = 0x8001FBD

#pragma long_calls_off

// better names
#define gLCDIOBuffer gLCDControlBuffer

#define SetColorEffectParameters SetSpecialColorEffectsParameters

// TODO: add to list
#define SetColorEffectFirstTarget ((void(*)(int, int, int, int, int))(0x08001F0D))
#define SetColorEffectSecondTarget ((void(*)(int, int, int, int, int))(0x08001F0D))
#define SetColorEffectBackdropFirstTarget ((void(*)(int))(0x08001F49))
#define SetColorEffectBackdropSecondTarget ((void(*)(int))(0x08001F65))

#endif // GBAFE_HARDWARE_H
