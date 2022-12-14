#include	"Extreme Pinball.h"
#include	"BasicTypes.h"
#include	"BasicDefines.h"
#include	"Misc.h"
#include	"VoxelEngine.h"
#include	"Map.c"

extern unsigned char Map_Alt[256 * 256];

// ----
CExtreme::CExtreme()
{
	// Inherited variables initialised by super constructor.
}

CExtreme::~CExtreme()
{
	// Inherited variables deleeted by super deconstructor.
}


// ----
// Check files.
bool	CExtreme::ValidateResources()
{
	return true;	// No resources or files to check.
}

/*
unsigned char Map_Alt[16*16 + 0] =  {
0x7c,0x7c,0x84,0x92,0xaf,0xd7,0xea,0xcc, // 0
0xd4,0xfb,0xfb,0xfb,0xee,0xd1,0x9e,0x87,

0x8a,0x6d,0x6e,0x85,0x85,0x88,0xc5,0xe3, // 1
0xef,0xfb,0xfb,0xfb,0xd9,0x9a,0x98,0x7f,

0x79,0x64,0x6a,0x26,0x49,0x9c,0xc6,0xd8, // 2
0xd2,0xd8,0xd8,0xe8,0xb8,0xac,0x87,0x87,

0x65,0x64,0x03,0x12,0x7b,0x9f,0xa9,0xc3, // 3
0xb8,0xaa,0xb3,0x93,0xad,0x9e,0x7f,0x68,

0x7f,0x5d,0x03,0x56,0x7e,0x7b,0x90,0x9f, // 4
0x8d,0x87,0x88,0x75,0x86,0x88,0x65,0x6a,

0x8b,0x58,0x59,0x74,0x70,0x65,0x6b,0x77, // 5
0x79,0x64,0x6b,0x8d,0x66,0x37,0x5c,0x7e,

0x84,0x52,0x71,0x74,0x79,0x6d,0x6e,0x4e, // 6
0x4b,0x3c,0x46,0x4e,0x5c,0x1b,0x28,0x73,

0x7b,0x66,0x8a,0x85,0x6d,0x76,0x6d,0x4c, // 7
0x3a,0x2f,0x37,0x2e,0x3f,0x45,0x31,0x77,

0x7f,0x61,0x71,0x9b,0x90,0x82,0x6b,0x45, // 8
0x25,0x28,0x2e,0x37,0x15,0x38,0x1e,0x77,

0x90,0x98,0x7b,0xb8,0xb8,0xaf,0x82,0x54, // 9
0x3c,0x45,0x53,0x45,0x2c,0x48,0x1f,0x79,

0x96,0x9c,0x77,0xc7,0xd2,0xba,0x99,0x6b, // 10
0x59,0x53,0x56,0x6e,0x57,0x70,0x2d,0x8a,

0xb6,0xa9,0x8b,0xa0,0xd2,0xc0,0xaa,0x92, // 11
0x79,0x67,0x73,0x7f,0x93,0x90,0x4e,0xa4,

0xa7,0x9e,0xa4,0x9e,0xb4,0xbd,0xbb,0xb5, // 12
0xb0,0x99,0xb5,0xaf,0x9f,0x73,0x4b,0x88,

0x77,0x9c,0xb8,0xaf,0xc0,0x99,0xa0,0xc6, // 13
0xd7,0xd8,0xbf,0xa4,0x99,0x78,0x7b,0x76,

0x68,0x79,0xa4,0xc0,0xcc,0xe0,0xd5,0xe0, // 14
0xc0,0xc5,0xd5,0xd4,0xce,0xb6,0x93,0x7c,

0x71,0x71,0x93,0xb3,0xe3,0xfa,0xf7,0xdb, // 15
0xe3,0xe0,0xf1,0xec,0xe0,0xcf,0x99,0x84,
};
*/


// ----
// Entry point for game.
bool	CExtreme::Run()
{
	CVoxelEngine	TheVoxelEngine;
	FP16Vec3		Dakar_ViewPosition;
	FP16Vec3		Dakar_ViewDirection;

	//init map file
#ifdef	LOAD_DATA
	if(!InitMapData())
	{
		return false;
	}
#endif
	

	Dakar_ViewPosition.X = IntToFP16(50);
	Dakar_ViewPosition.Y = IntToFP16(50);
	Dakar_ViewDirection.Y = IntToFP16(0);

	TheVoxelEngine.Init(GraphicsRef, Map_Alt);
	while (TheGameWindow->Process())
	{
		if (ControlsRef->IsButtonPressedDB(Select))
			return true;

		if (ControlsRef->IsButtonPressed(Left))
			Dakar_ViewDirection.Y -= 3;
		if (ControlsRef->IsButtonPressed(Right))
			Dakar_ViewDirection.Y += 3;

		if (ControlsRef->IsButtonPressed(Up))
		{
			Dakar_ViewPosition.X += _Sin(Dakar_ViewDirection.Y & 0xFF) >> 3;
			Dakar_ViewPosition.Y += _Cos(Dakar_ViewDirection.Y & 0xFF) >> 3;
		}
		
		if (ControlsRef->IsButtonPressed(Down))
		{
			Dakar_ViewPosition.X -= _Sin(Dakar_ViewDirection.Y & 0xFF) >> 3;
			Dakar_ViewPosition.Y -= _Cos(Dakar_ViewDirection.Y & 0xFF) >> 3;	
		}

		// Clear the screen.
		GraphicsRef->StartDraw();
		GraphicsRef->SetActiveScreen(1);
		GraphicsRef->ClearScreen(0,0,180);

		TheVoxelEngine.SetCameraPosition(&Dakar_ViewPosition);
		TheVoxelEngine.SetCameraRotation(Dakar_ViewDirection.Y);
		TheVoxelEngine.Render();

		GraphicsRef->SwapScreens();
		GraphicsRef->EndDraw();

		MoveViewport(0,0);
	}
	return true;
}

