#include	"VoxelEngine.h"
#include	<math.h>
#include	"Misc.h"

// ----
CVoxelEngine::CVoxelEngine()
{
	Map_Alt = NULL;
}

// ----
CVoxelEngine::~CVoxelEngine()
{
}

// ----
// Pass in data references, etc.
bool	CVoxelEngine::Init(CGraphics		*_GraphicsRef,
						   unsigned char	*_MapRef)
{
	int	i;

	// Set up camera scaling factors - this will eventually be a hardcoded lookup table.
	for (i = 1; i < 80; i++)
	{
		CamScale[i] = (8192 * 2) / (i);
	}

	GraphicsRef	= _GraphicsRef;
	Map_Alt		= _MapRef;

	return true;
}


// ----
// Set view position,
void	CVoxelEngine::SetCameraPosition(FP16Vec3	*_Pos)
{
	CameraPos = *_Pos;
}

// ----
// Set camera rotation - y-axis only for now.
void	CVoxelEngine::SetCameraRotation(FP16	_YRot)
{
	CameraRot = _YRot;
}

// ----
// Macros.

// ----
// Render the bloody view.
void	CVoxelEngine::Render()
{
	unsigned char	Col;

	unsigned char	sx;			// X-screen pos (ray line)
	signed short	sy0, sy1;	// Y-screen variables.
	
	// All map co-ords are in 1.7.8 fixed point format
	signed short	mx, my;		// Map x and y (initilised as above, and modified by the following two variables per rayline step)
	signed short	mxd,myd;	// Map x, y deltas
	unsigned char	mxfp, myfp;	// Map fractional part.
	
	unsigned char	steps;		// Number of steps per rayline.
//	unsigned char	section;
	unsigned char	lower, upper;

	signed short	cheight;	// Camera height
	signed short	py;			// Height of land at map point, (mx,my)

	int				j;
	unsigned char	*plocale;

	unsigned char	*AddrStore;
	unsigned char	*Addr;

	if (GraphicsRef->ActiveScreen)
	{
		AddrStore = (unsigned char*)GraphicsRef->Screens[GraphicsRef->ActiveScreen];
	}
	else
	{
		AddrStore = (unsigned char*)GraphicsRef->GraphicsMemAddress;
	}

	if (!AddrStore)
		return;
	AddrStore += 319 * GraphicsRef->DispProperties.cbyPitch;

	// Ray cast loop. (one per line of the horizontal display).
	for (sx = 0; sx < 120; sx ++)
	{
		mx = CameraPos.X;	// Centre point for camera.
		my = CameraPos.Y;	// and inital render pos.

		// Calculate interpolated camera height.
		plocale = &Map_Alt[(my & 0x7F00) + (mx >> 8)];
		//plocale = &Map_Alt[((my >> 8) << 4) + (mx >> 8)];
		cheight =	(	(	(*(unsigned char*)(plocale -1))		* (256 -	(mx & 0xFF))
						+	(*(unsigned char*)(plocale +1))		* (			(mx & 0xFF))
						+	(*(unsigned char*)(plocale -256))	* (256 -	(my & 0xFF))
						+	(*(unsigned char*)(plocale +256))	* (		 	(my & 0xFF))) >> 1) + 2560;
	 
		// Set up offset values. 
		// These values are added to the mx, my values per rayline step.
//		mxd = (signed short)(_Sin((CameraRot + ((sx - 40) ))) * 256) >> 4;		// Calculate angle for ray line based on overall 
//		myd = (signed short)(_Cos((CameraRot + ((sx - 40) ))) * 256) >> 4;		// camera orientation and the ray line number.

		mxd = _Sin((CameraRot + ((sx - 60) )) & 0xFF) >> 3;		// Calculate angle for ray line based on overall 
		myd = _Cos((CameraRot + ((sx - 60) )) & 0xFF) >> 3;		// camera orientation and the ray line number.

		sy0 = 0;

		lower = 6;
		upper = 40;

		Addr = AddrStore + 2 *sx * GraphicsRef->DispProperties.cbxPitch;

		// Step coverns the distance scaling factor, and 
		// the number of sections that will be rendered.
		for (steps = lower; steps < upper; steps++)	// These values need to be set to give optamal fps and appearance.
		{
			// Step down the ray line. 
			mx += mxd;
			my += myd;

			// Cache info.
			mxfp = mx & 0xFF;
			myfp = my & 0xFF;

			// Get point height, minus observation piont.
			plocale = &Map_Alt[(my & 0x7F00) + (mx >> 8)];
			//plocale = &Map_Alt[((my >> 8) << 4) + (mx >> 8)];
			py =	(	(	(*(unsigned char*)(plocale -1))		* (256 -	mxfp)
						+	(*(unsigned char*)(plocale +1))		* (			mxfp)
						+	(*(unsigned char*)(plocale -256))	* (256 -	myfp)
						+	(*(unsigned char*)(plocale +256))	* (		 	myfp)) >> 1)
					-  cheight;

			// Project into screen space.
			sy1 = FP16ToInt(FP16Mul(py, CamScale[steps])) + 120;	// Fixed point maths and a conversion back to a plain integer.

			/*
			Ys = ( Yv - Ye ) * Yk / Z + Yc

			Ys: coordinate projected onto the screen
			Yv: altitude of the voxel column
			Ye: coordinate of the eye
			Z: distance from the eye to the considered point
			Yk: constant to scale the projection, possibly negative
			Yc: constant to centre the projection, usually half the screen resolution
			*/

			//sy1 *= 2;

			// If pos is higher than previous so far...
			if (sy1 > sy0)
			{
				// Get bar colour
				Col = min(Map_Alt[my + (mx >> 8)] - (steps >> 1) + 40 + (( ((mx >> 8)%2) ^ ((my >> 8)%2) )?10:-10), 160);	

				sy1 = min(320, sy1);
				for (j = sy0; j < sy1; j++)
				{
					*(unsigned short*)(Addr + 0 * GraphicsRef->DispProperties.cbxPitch)	= ((0 >> 3) << 10) | ((Col >> 3) << 5) | ((0 >> 3) << 0);
					*(unsigned short*)(Addr + 1 * GraphicsRef->DispProperties.cbxPitch)	= ((0 >> 3) << 10) | ((Col >> 3) << 5) | ((0 >> 3) << 0);
				//	*(unsigned short*)(Addr + 2 * GraphicsRef->DispProperties.cbxPitch)	= ((0 >> 3) << 10) | ((Col >> 3) << 5) | ((0 >> 3) << 0);
					Addr -= GraphicsRef->DispProperties.cbyPitch;
				}
			
				// Update highest.
				// Break if highest is off top of screen.
				sy0 = sy1;
				if (sy0 >= 320)
					break;
			}
		}
	}
}


