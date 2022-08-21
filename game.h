#ifndef	_GAME_H_
#define	_GAME_H_

#include	<windows.h>

#include	"GameWindow.h"
#include	"Platform.h"
#include	"GraphicsClasses.h"
#include	"SoundSystem.h"
#include	"Controls.h"
#include	"Localization.h"
#include	"Preferences.h"
#include	"String.h"

class CGame : public CObject
{
public:
	// Items in this category can be messaged for any code.

	// ----
	CGame();
	~CGame();

	// ----
	// Opens game window, initialises GAPI, etc.
	virtual	bool	Init(	HINSTANCE	_hInstance,
							LPTSTR		_lpCmdLine,
							int			_nCmdShow);

	// ----
	// A pure virtual method to ensure that we have all the
	// resources we need.
	// Note:	This is a pure-virtual method, and as such 
	//			must be implemented by a derived class.
	virtual	bool	ValidateResources()	= 0;// PURE VIRTUAL

	// ----
	// This method is intended to be called after Init, and
	// is the entry point for the game. 
	// Note:	This is a pure-virtual method, and as such 
	//			must be implemented by a derived class.
	virtual	bool	Run()				= 0;	// PURE VIRTUAL
	
	

protected:
	// Items in this category will be inherited.
	CPlatform		*ThePlatform;
	CPreferences	*ThePreferences;
	CLocalization	*TheLocalization;

	CGameWindow		*TheGameWindow;
	CGraphics		*GraphicsRef;
	CSoundSystem	*TheSoundSystem;
	CControls		*ControlsRef;

	CString			TempString;
	char			convert[MAX_PATH];
private:
	// Items in this category can not be inherited.
};

#endif