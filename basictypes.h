// This file contains the definition of several useful basic types.

#ifndef _BASICTYPES_H_
#define _BASICTYPES_H_

typedef signed short	FP16;
typedef signed long		FP32;

// ----
struct	_IntCouple
{
	int	A;
	int B;
};

// ----
// Doesn't use as much memory as a RECT;
struct _SmallRect
{
	SHORT	top;
	SHORT	left;
	SHORT	bottom;
	SHORT	right;
};

struct	_IntTriple
{
	INT		X, Y, Z;
};

// A three point vector
struct	_FP16Vec3
{
	FP16	X, Y, Z;
};


// ----
// Typedefs
typedef _IntCouple		IntCouple;
typedef _SmallRect		SmallRect;
typedef _IntTriple		IntTriple;
typedef _IntTriple		IntTriVector;
typedef	_FP16Vec3		FP16Vec3;





//define meself a basic tile type. haz a flag part to hold extra info.
typedef
struct	_COLOURTILE
{
	SHORT	TileNum;
	DWORD	Data[64][64];
}
COLOURTILE;

typedef
struct	_HEIGHTTILE
{
	SHORT	TileNum;
	char	Data[64][64];
	INT		Flags;
}
HEIGHTTILE;

#endif