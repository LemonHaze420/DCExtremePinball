#ifndef _GAMEDEFINES_H_
#define _GAMEDEFINES_H_

// Project specific stuff.
#define	PROJECT_NAME			TEXT("Extreme Pinball")
#define GAME_WINDOW_CLASS_NAME	TEXT("06D3D508-62E9-4024-84FB-7DDE39BC421C")

// ----
// Message passing
#ifndef WM_MAIN_THREAD_ACTIVATE
#define WM_MAIN_THREAD_ACTIVATE	(WM_USER+0)
#endif

#ifndef WM_MAIN_THREAD_KEYUP
#define WM_MAIN_THREAD_KEYUP	(WM_USER+1)
#endif

#ifndef WM_MAIN_THREAD_KEYDOWN
#define WM_MAIN_THREAD_KEYDOWN	(WM_USER+2)
#endif

#ifndef WM_MAIN_THREAD_DESTROY
#define WM_MAIN_THREAD_DESTROY	(WM_USER+3)
#endif

#ifndef	WM_MAIN_THREAD_CHAR
#define	WM_MAIN_THREAD_CHAR		(WM_USER+4)
#endif

// ----
#define	MAX_SAVE_NAME_LENGTH	8

// ----
// Available langauges
enum ELanguage
{
	EL_English				= 0,
	EL_French				= 1,
	EL_German				= 2,
	EL_Spanish				= 3,
	EL_Welsh				= 4
};

// ----
// Device orientations.
enum EOrientation
{
	EO_Portrait_Normal,		// 0
	EO_Landscape_Flipped,	// 90
	EO_Portrait_Flipped,	// 180
	EO_Landscape_Normal,	// 270
};


#define	GAME_FRAME_RATE_LIMIT	8
#define	INTRO_FRAME_RATE_LIMIT	8
#define	QUIT_EXCEPTION			0xEFFFFFFF
#define	BATTERY_LOW_FLASH_TIME	500

#endif