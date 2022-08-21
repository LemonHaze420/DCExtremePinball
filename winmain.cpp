#include	"CompilerEnvironment.h"

// Pre written headers
#include	<windows.h>
#include	<excpt.h>

// Our headers.
#include	"SafeProgramming.h"
#include	"Object.h"
#include	"String.h"
#include	"GameWindow.h"
#include	"RegistryFunctions.h"
#include	"GameDefines.h"
#include	"Extreme Pinball.h"
#include	"ErrorCodes.h"

// ----
// Global variable to store address of exception data.
LPEXCEPTION_POINTERS	GlobalExecptionPointerStore;

// ----
// Function call as Structured Exception Handling (SEH) 
// exception filter.
int CatchAll(LPEXCEPTION_POINTERS _a)
{
	GlobalExecptionPointerStore = _a;
	return EXCEPTION_EXECUTE_HANDLER;
}

// ----
// Program entry point.
int WINAPI WinMain(	HINSTANCE	_hInstance,
					HINSTANCE	_hPrevInstance,
					LPTSTR		_lpCmdLine,
					int			_nCmdShow)
{
	// No execptions yet.
	GlobalExecptionPointerStore		= NULL;
	bool			ErrorOccured	= false;
	bool			LowMemWarning	= false;
	DWORD			ErrorCode		= ERROR_NO_ERROR;
	CExtreme*		TheGame			= NULL;
	

	// Check memory levels.
	MEMORYSTATUS	StartMem;
	StartMem.dwLength = sizeof(StartMem);
	GlobalMemoryStatus(&StartMem);

	TCHAR memconvert[80];
	wsprintf(memconvert, TEXT("Free mem: %d\n"), StartMem.dwAvailPhys );
	OutputDebugString(memconvert);

	if (StartMem.dwAvailPhys < 2621440) // Demain 2.5Mb
	{
		LowMemWarning = true;
	}

#ifndef NOT_FINAL_BUILD
	__try
	{
#endif
		NEW(TheGame, CExtreme());
		if (	(TheGame) 
			&&	(TheGame->Init(_hInstance, _lpCmdLine, _nCmdShow)))
		{
			TheGame->Run();
			ErrorOccured = TheGame->DidAnErrorOccur();
			ErrorCode = TheGame->GetErrorCode();
		}
		
		ErrorOccured = TheGame->DidAnErrorOccur();
		ErrorCode = TheGame->GetErrorCode();

#ifndef NOT_FINAL_BUILD
	}
	__except(CatchAll(GetExceptionInformation()))
	{
		__try
		{
			if (GlobalExecptionPointerStore)
			{
				FILE* FH;
				SYSTEMTIME ExceptionTime;

				GetSystemTime(&ExceptionTime);

				TCHAR	convert[256];
				wsprintf(	convert, 
							TEXT("\\Extreme_Pinball_Failure%d%d%d-%d%d%d%d.dat"), 
							ExceptionTime.wYear, 
							ExceptionTime.wMonth,
							ExceptionTime.wDay,
							ExceptionTime.wHour,
							ExceptionTime.wMinute,
							ExceptionTime.wSecond,
							ExceptionTime.wMilliseconds);
				
				// Open save file.
				FH = FOPEN(convert, "wb");
				if (FH)
				{
					fwrite("CONTEXT\n", 9, 1, FH);
					fwrite(GlobalExecptionPointerStore->ContextRecord, sizeof(CONTEXT), 1, FH);
					EXCEPTION_RECORD	*ER = GlobalExecptionPointerStore->ExceptionRecord;
					while (ER)
					{
						fwrite("\nRECORD\n", 9, 1, FH);
						fwrite(ER->ExceptionRecord, sizeof(EXCEPTION_RECORD), 1, FH);
						ER = ER->ExceptionRecord;
					}

					fwrite("\nEND", 5, 1, FH);
					fclose(FH);
				}
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			OutputDebugString(TEXT("\n\nFAILURE\nDied in first exeception handler. Giving up!\n"));
		}
		ErrorOccured = true;
	}
#endif
	
	// Take outside the exception handle.
	SAFELY_DELETE(TheGame);

	// Make this give the option to create an automatic error report.
	if (ErrorOccured)
	{
		if (LowMemWarning)
		{
			// Low memory warning ignored
			MessageBox(NULL, TEXT("Unfortunately Extreme Pinball failed as the system is low on free memory. Close other applications and delete old files, and try running the game again."), PROJECT_NAME, MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
		}
		else if (ErrorCode == ERROR_MISSING_RESOURCE)
		{
			// File(s) missing
			MessageBox(NULL, TEXT("Sorry, one (or more) file(s) required by Extreme Pinball are corrupt or missing.\n\nPlease check that the game is correctly installed.\n\nIf your problems persist, or you require further information then please email extreme@broadsword.co.uk stating the nature of the problem."), PROJECT_NAME, MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
		}
		else
		{
			// Generic
			MessageBox(NULL, TEXT("Sorry, an error occured whilst running Extreme Pinball.\n\nPlease check that the game is correctly installed, and that the data files have not been moved or deleted. \n\nIf your problems persist, or you require further information then please email extreme@broadsword.co.uk stating the nature of the problem."), PROJECT_NAME, MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
		}
	}

	// Ensure that all file handles are dropped.
	_fcloseall();

	// Print out a list of undeleted objects...
	OutputDebugString(TEXT("\n\n\nTAKE NOTE:\nA list of CObject based objects in existance at shutdown follows:"));
	CObject::OutputObjectList();

#ifdef NOT_FINAL_BUILD
#ifdef _UNICODE

	TCHAR buffer[256];
	swprintf(buffer, TEXT("%d objects not freed.\n\nA list of objects, and where they were created can be found by looking at the debug output.\n"), CObject::GetObjectCount());
	OutputDebugString(buffer);
	
	#ifdef NOT_FINAL_BUILD
	if (CObject::GetObjectCount())
		MessageBox(NULL, buffer, PROJECT_NAME, MB_OK);
	#endif
#endif
#endif

	return 0;
}