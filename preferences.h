#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

#include	"Object.h"
#include	"RegistryFunctions.h"
#include	"Platform.h"

// ----
// This class encapsulates preference data. 

// Define your default preference values here.
#define	REG_PREFERENCE_PATH			"Software\\Broadsword\\Extreme"
#define	REG_VOLUME_SETTING			"Volume"
#define	REG_MUSIC_SETTING			"Music"
#define	REG_EFFECTS_SETTING			"Effects"
//#define	REG_ENABLE_DB_SETTING		"EnableDoubleBuffering"
//#define	REG_SLOW_VRAM_SETTING		"SlowVRAM"
//#define	REG_STYLUS_SETTING			"StylusControl"
//#define	REG_DEMO_TIMEOUT_SETTING	"DemoTimeOut"
#define	REG_ORIENTATION_SETTING		"PreferredOrientation"

#define	DEFAULT_VOLUME_LEVEL	5
#define	DEFAULT_MUSIC_STATE		true
#define	DEFAULT_EFFECTS_STATE	true
#define	DEFAULT_STYLUS_CONTROL	0
#define	DEFAULT_DEMO_TIME_OUT	30

class CPreferences : public CObject
{
public:
	CPreferences();
	~CPreferences();

	// ----
	bool		LoadPreferences(CPlatform*	_PlatformRef);
	bool		SavePreferences();
	bool		ValidatePreferences();
	void		AssumeDefaults(CPlatform*	_PlatformRef);

	/*// Place any preference data here, then update the 
	// methods (above) to save the data to the registry.
	bool			SlowVRAMWorkAround;
	bool			DoubleBufferingEnabled;
	int				ShowDemoTimeOut;
	int				StylusControl;
	*/

	ELanguage		Language;
	EOrientation	PreferredOrientation;
	int				Volume;
	bool			MusicEnabled;
	bool			EffectsEnabled;
	bool			DoubleBufferingEnabled;

protected:
	CPlatform*	PlatformRef;
};

#endif