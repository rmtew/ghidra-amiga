#ifndef	GRAPHICS_GFX_H
#define	GRAPHICS_GFX_H
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif
#define	BITSET	0x8000
#define	BITCLR	0
#define	AGNUS
#ifdef	AGNUS
#define	TOBB(a)	((long)(a))
#else
#define	TOBB(a)	((long)(a)>>1)
#endif
struct	Rectangle
{
SHORT	MinX,MinY;
SHORT	MaxX,MaxY;
};
typedef	struct	tPoint
{
WORD	x,y;
}	Point;
typedef	UBYTE	*PLANEPTR;
struct	BitMap
{
UWORD	BytesPerRow;
UWORD	Rows;
UBYTE	Flags;
UBYTE	Depth;
UWORD	pad;
PLANEPTR	Planes[8];
};
#define	RASSIZE(w,h)	((h)*(	(w+15)>>3&0xFFFE))
#endif
