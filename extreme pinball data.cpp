#include	"Extreme Pinball.h"



/*********************************
	DEFINES and TYPEDEFS and STUFF
**********************************/





/*********************************
	FUNCTION definitions
**********************************/

/**********************
 * This function will init all of our map data.
 * As this is just a test a lot of temporary code for loading up 
 * predetermined stuff will be in there. This is marked with the label //TEMP
 * If this is turned into an actual project then these bits will have to
 * change to reflect multiple maps and things.
 *********************/
bool	CExtreme::InitMapData()
{
	//TEMP
	MapIndexX = 1;
	MapIndexY = 1;

	ViewPosY = 64; // top corner of the centre tile in the 192 arrays
	ViewPosX = 64;

	FILE *in;
	CString		FileName;
	

	//LOAD DATA*********************************************************************************

	//Read in the data for the map index array
	FileName = ThePlatform->GetPath(EP_Data);

	//TEMP
	FileName += "Track.map";


	in = FOPEN(FileName.GetString(), "rb");
	if(in != NULL)
	{
		fread(&HeightMapIndexArray, sizeof(HeightMapIndexArray), 1, in);
		fread(&ColourMapIndexArray, sizeof(ColourMapIndexArray), 1, in);

		fclose(in);
	}
	else
	{
		ODSN("Couldn't open MAP file");
		return false;
	}


	//read in all my height tile data
	FileName = ThePlatform->GetPath(EP_Data);

	//TEMP
	FileName += "HeightTileData.htd";


	in = FOPEN(FileName.GetString(), "rb");
	if(in)
	{
		for(int i = 0; i < 128; i++)
		{
			int k = 0;
			HeightTileArray[i].TileNum = i;
			HeightTileArray[i].Flags = 0x00;

			for(int y = 0; y < 64; y++)
				for(int x = 0; x < 64; x++)
					k = fread(&HeightTileArray[i].Data[x][y], sizeof(char), 1, in);

			if(k == 0)
				break;
		}

		fclose(in);

		//sanity check
		
		//for(int j = 0; j < 128; j++)
		{
			if(HeightTileArray[0].Data[0][0] == NULL)
			{
				ODSN("Failed to Load Height Tile Data");
				return false;
			}
		}
		
	}
	else
	{
		ODSN("Couldn't open HEIGHT DATA file");
		return false;
	}

	//read in all my Colour tile data
	FileName = ThePlatform->GetPath(EP_Data);

	//TEMP
	FileName += "ColourTileData.asd";


	in = FOPEN(FileName.GetString(), "rb");
	if(in)
	{
		for(int i = 0; i < 128; i++)
		{
			int k = 0;
			ColourTileArray[i].TileNum = i;

			for(int y = 0; y < 64; y++)
				for(int x = 0; x < 64; x++)
					k = fread(&ColourTileArray[i].Data[x][y], sizeof(DWORD), 1, in);

			if(k == 0)
				break;

		}

		fclose(in);

		//sanity check
		
		//for(int j = 0; j < 128; j++)
		{
			if(ColourTileArray[0].Data[0][0] == NULL)
			{
				ODSN("Failed to Load Colour Tile Data");
				return false;
			}
		}
		
	}
	else
	{
		ODSN("Couldn't open COLOUR DATA file");
		return false;
	}

	//ALL DATA LOADED************************************************************************
	UpdateCurrentData(0, 0);

	return true;
}

/**************************************************************
 * This function will load up te raw tile data for both sets.
 * xChange and yChange set how much to move MapIndexs by.
 **************************************************************/
void	CExtreme::UpdateCurrentData(int xChange, int yChange)
{
	int x,y,i,j = 0;

	MapIndexX += xChange;
	MapIndexY += yChange;

	for(y = 0, (i = MapIndexY - 1); y < 192; y += 64, i++)
	{
		for(x = 0, (j = MapIndexX - 1), j = 0; x < 192; x += 64, j++)
		{
			//ok those last 2 for loops get us to the correct starting positions in the
			//data arrays and the correct places in the tile map
			
			//now I have to memcpy the 64 lines of the bitmap to the correct arrays
			int HeightTileNum = HeightMapIndexArray[j][i];
			int ColourTileNum = ColourMapIndexArray[j][i];

			for(int p = 0; p < 64; p++)
			{
				//for(int l = 0; l < 64; l++)
				//{
				//	CurrentHeightData[x][y + p] = HeightTileArray[HeightTileNum].Data[l][p];
				//	CurrentColourData[x][y + p] = ColourTileArray[ColourTileNum].Data[l][p];
				//}
				memcpy(&CurrentHeightData[y + p][x], &HeightTileArray[HeightTileNum].Data[p][0], 64);
				memcpy(&CurrentColourData[y + p][x], &ColourTileArray[ColourTileNum].Data[p][0], (64 * sizeof(DWORD)) );
			}
		}
	}
	return;
}

 /*************************************************************
  * This next fuction moves the viewport around inside the smaller data arrays
  * If the viewport moves too close to the edge of one of the arrays it calls
  * updatecurrentdata to move the array and then resets you position to be in the 
  * centre tile.
  *************************************************************/

void	CExtreme::MoveViewport(int xChange, int yChange)
{
	ViewPosX += xChange;
	ViewPosY += yChange;

	//first we have to work out wether the viewport has moved far enough to 
	//need to move the larger map.
	int xDiff = 0; int yDiff = 0;
	
	if(ViewPosX <= 32)
	{
		xDiff--;
		ViewPosX += 32;
	}
	else if(ViewPosX >= 96)
	{
		xDiff++;
		ViewPosX -= 32;
	}

	if(ViewPosY <= 32)
	{
		yDiff--;
		ViewPosY += 32;
	}
	else if(ViewPosY >= 96)
	{
		yDiff++;
		ViewPosY -= 32;
	}

	if( (xDiff != 0) || (yDiff != 0) )
	{
		UpdateCurrentData(xDiff, yDiff);
	}
	//********* DONE ***********

	//Now I must get the data from the large array and copy it into my 32 * 32
	for(int i = 0; i < 32; i++)
	{
		memcpy(&ViewportHeightData[i][0], &CurrentHeightData[ViewPosY + i][ViewPosX], 32);
		memcpy(&ViewportColourData[i][0], &CurrentColourData[ViewPosY + i][ViewPosX], (sizeof(DWORD) * 32) );
	}

}