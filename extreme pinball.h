#ifndef	_EXTREME_PINBALL_H_
#define	_EXTREME_PINBALL_H_

#include	"Game.h"
#include	<windows.h>

#include	"Platform.h"
#include	"GraphicsClasses.h"
#include	"SoundSystem.h"
#include	"Controls.h"
#include	"Localization.h"
#include	"Preferences.h"

//added by a.s.d
#include	<stdio.h>

class CExtreme : public CGame
{
public:
	// ----
	CExtreme();
	~CExtreme();

	
	// ----
	// <<Pure virtual inherited from CGame>>
	// Check files.
	bool	ValidateResources();

	// ----
	// <<Pure virtual inherited from CGame>>
	// Entry point for game.
	bool	Run();

protected:

	// A great number of properties are inherited from CGame.

	//Variables For Extreme Data
	char	HeightMapIndexArray[60][60];	//holds the index of all our tiles for one map.
	char	ColourMapIndexArray[60][60];
	
	HEIGHTTILE	HeightTileArray[128]; //holds all the height info
	COLOURTILE	ColourTileArray[128]; //holds all my colour info

	char	CurrentHeightData[192][192];
	DWORD	CurrentColourData[192][192];

	char	ViewportHeightData[32][32];
	DWORD	ViewportColourData[32][32];

	int ViewPosX;
	int ViewPosY;
	int MapIndexX;
	int MapIndexY;

	//Methods for Extreme Data
	bool	InitMapData();	//loads up the initial map index array
							//Loads up all the tiles, and sets the initial viewport and 
							//current data setings.

	/*
		Called every turn this moves the small viewports around the current data maps.
		If it get towards the edge of the maps it updates the currentData maps to
		pull in more information
	*/
	void	MoveViewport(int xChange, int yChange);

	/*
		This moves the tile array around by complete blocks thus revealing more of the map
	*/
	void	UpdateCurrentData(int xChange, int yChange);
};

#endif