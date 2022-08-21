#include	"Object.h"
#include	"BasicTypes.h"
#include	"BasicDefines.h"
#include	"GraphicsClasses.h"

class	CVoxelEngine	: public	CObject
{
public:
	// ----
	// 
	CVoxelEngine();
	~CVoxelEngine();

	// ----
	// Pass in data references, etc.
	bool	Init(	CGraphics		*_GraphicsRef,
					unsigned char	*_MapRef);

	// ----
	// Set view position,
	void	SetCameraPosition(FP16Vec3	*_Pos);

	// ----
	// Set camera rotation - y-axis only for now.
	void	SetCameraRotation(FP16	_YRot);

	// ----
	// Render the bloody view.
	void	Render();

protected:
	CGraphics		*GraphicsRef;
	unsigned char	*Map_Alt;
	long			CamScale[80];
	FP16			CameraRot;
	FP16Vec3		CameraPos;
};
