#include	"Game.h"
#include	"ErrorCodes.h"

// ----
CGame::CGame()
{
	TheGameWindow		= NULL;
	ThePlatform			= NULL;
	GraphicsRef			= NULL;
	TheSoundSystem		= NULL;
	ControlsRef			= NULL;
	TheLocalization		= NULL;
	ThePreferences		= NULL;
}

// ----
CGame::~CGame()
{
	SAFELY_DELETE(TheGameWindow);
	SAFELY_DELETE(ThePlatform);
	SAFELY_DELETE(TheSoundSystem);
	SAFELY_DELETE(TheLocalization);
	SAFELY_DELETE(ThePreferences);
}

// Global crap to allow threading of sound system.
CSoundSystem	*GlobalSSRef;
unsigned long	__cdecl ProcessAsync(void* _a)	
{ 
	while(GlobalSSRef)
	{
		GlobalSSRef->Process();
	} 
	ExitThread(1);
	return true;
}


// ----
// Opens game window, initialises GAPI, etc.
bool	CGame::Init(HINSTANCE	_hInstance,
					LPTSTR		_lpCmdLine,
					int			_nCmdShow)
{
//	FILE	*FH;

	ClearErrorCode();

	// ----
	// Create an object to store the platform information.
	SAFELY_DELETE(ThePlatform);
	NEW(ThePlatform, CPlatform());
	if (ThePlatform)
	{
		if (!ThePlatform->Init())	
		{
			ErrorCode = ThePlatform->GetErrorCode(); 
			return false;
		}
	}
	else
	{
		ErrorCode = ERROR_OUT_OF_MEMORY; 
		return false;
	}

	// ----
	// Validate the resources
	if (!ValidateResources())
	{
		ErrorCode = ERROR_MISSING_RESOURCE;
		return false;
	}

	// ----
	// Create the preferences object and load in settings.
	SAFELY_DELETE(ThePreferences);
	NEW(ThePreferences, CPreferences());
	if (ThePreferences)
	{
		if (!ThePreferences->LoadPreferences(ThePlatform))	
		{
			ErrorCode = ThePreferences->GetErrorCode(); 
			return false;
		}
	}
	else
	{
		ErrorCode = ERROR_OUT_OF_MEMORY; 
		return false;
	}

	// ----
	// Load localization strings.
	SAFELY_DELETE(TheLocalization);
	NEW(TheLocalization, CLocalization(ThePlatform, ThePreferences));
	if (TheLocalization)
	{
		if (!TheLocalization->LoadLocalizationStrings())	
		{
			ErrorCode = TheLocalization->GetErrorCode(); 
			return false;
		}
	}
	else
	{
		ErrorCode = ERROR_OUT_OF_MEMORY; 
		return false;
	}

	// ----
	// Create app window + Graphics and Controller objects (created internally by CGameWindow::Init).
	NEW(TheGameWindow, CGameWindow());
	if (TheGameWindow)
	{
		if (!TheGameWindow->Init(_hInstance, _lpCmdLine, _nCmdShow, ThePlatform, ThePreferences))	
		{
			ErrorCode = TheGameWindow->GetErrorCode(); 
			return false;
		}
	}
	else
	{
		ErrorCode = ERROR_OUT_OF_MEMORY; 
		return false;
	}

	// ----
	// Create SoundSystem object.
	SAFELY_DELETE(TheSoundSystem);
	NEW(TheSoundSystem, CSoundSystem());
	if (TheSoundSystem)
	{
		if (!TheSoundSystem->Init(ThePlatform, ThePreferences->Volume))	
		{
			ErrorCode = TheSoundSystem->GetErrorCode(); 
			return false;
		}
	}
	else
	{
		ErrorCode = ERROR_OUT_OF_MEMORY; 
		return false;
	}

	// ----
	// Access Graphics object and customise it.
	GraphicsRef = TheGameWindow->GetGraphicsObject();
	if (!GraphicsRef)
	{
		ErrorCode = ERROR_INVALID_OBJECT;
		return false;
	}
	GraphicsRef->AssignPlatform(ThePlatform);
	GraphicsRef->AssignSoundSystem(TheSoundSystem);
	GraphicsRef->SetOrientation(ThePreferences->PreferredOrientation);

	// ----
	// Get Controller object and customise it.
	ControlsRef = TheGameWindow->GetControlObject();
	if (!ControlsRef)
	{
		ErrorCode = ERROR_INVALID_OBJECT;
		return false;
	}
	ControlsRef->SetOrientation(ThePreferences->PreferredOrientation);

	// ----	
	// Music
	TempString = "music.umd";
	TheSoundSystem->LoadMod(&TempString);
	if (!ThePreferences->EffectsEnabled)	{TheSoundSystem->StopAllSamples();	}
	if (ThePreferences->MusicEnabled)		{TheSoundSystem->PlayMod();			} else {TheSoundSystem->StopMod();}

#if ASYNC_SOUND
	HANDLE	SoundProcessThreadHandle;
	DWORD	SoundProcessThreadID;
	GlobalSSRef = TheSoundSystem;
	SoundProcessThreadHandle = CreateThread(NULL, 
											0, 
											ProcessAsync,	// Start method
											0, 
											CREATE_SUSPENDED, 
											&SoundProcessThreadID);
	ResumeThread(SoundProcessThreadHandle);
#endif

//THREAD_PRIORITY_LOWEST

	return true;
}