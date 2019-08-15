#ifndef M7_INCLUDED
#define M7_INCLUDED

#include "gbafe.h"

// TODO: add to CLib
enum
{
	SCREEN_HEIGHT = 160,
	SCREEN_WIDTH  = 240,
};

enum
{
	M7_D       =  256, //!< Focal length
	M7_D_SHIFT =    8, //!< Focal shift
	M7O_NORM   =    2, //!< Object renormalization shift (by /4)

	// View frustrum limits
	M7_LEFT    = -120, //!< Viewport left
	M7_RIGHT   = +120, //!< Viewport right
	M7_TOP     =  +80, //!< Viewport top (y-axis up)
	M7_BOTTOM  =  -80, //!< Viewport bottom (y-axis up!)
	M7_NEAR    =   24, //!< Near plane (objects)
	M7_FAR     =  512, //!< Far plane (objects)

	M7_FAR_BG  =  768, //!< Far plane (floor)
};

struct M7Vector
{
	int x, y, z;
};

struct M7Sprite
{
	/* 00 */ struct M7Vector pos;    //!< world position
	/* 0C */ struct M7Vector camPos; //!< camera position
	/* 18 */ u16 oam0, oam1, oam2;   //!< oam data
	/* 1E */ u16 config;             //!< config bits
	/* 20 */ s8 xAnchor, yAnchor;    //!< position anchor point within sprite image
	/* 24 */ struct M7Sprite* next;  //!< next element in linked list
};

struct M7Camera
{
	/* 00 */ struct M7Vector pos;
	/* 0C */ int theta, phi;
	/* 18 */ struct M7Vector u, v, w;
	/* 3C */ /* end */
};

struct M7CameraProc
{
	/* 00 */ PROC_HEADER;
	/* 2C */ struct M7Camera data;
	/* 68 */ /* this barely fits into the proc */
};

struct M7Super3dWorld
{
	struct M7CameraProc* cam;
	struct M7Sprite* sprite;
	struct BgControl bg2cnt;
	short horizon;

	short lam[SCREEN_HEIGHT];
};

extern struct M7Super3dWorld* const m7World;

void m7Init(void);
void m7End(void);
void m7Indentity(void);
void m7Rotate(unsigned phi, unsigned theta);
void m7TranslateLevel(int x, int y, int z);
void m7TranslateLocal(int x, int y, int z);
void m7TranslateGlobal(int x, int y, int z);

#define M7_GET_CF (m7World->cam->data.u.x)
#define M7_GET_SF (m7World->cam->data.u.z)
#define M7_GET_CT (m7World->cam->data.v.y)
#define M7_GET_ST (m7World->cam->data.w.y)

#endif // M7_INCLUDED
