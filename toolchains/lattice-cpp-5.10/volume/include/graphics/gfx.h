// graphics/gfx.h
//
// (C) 1987, MTS Associates
//
#ifndef GRAPHICS_GFX_H
#define GRAPHICS_GFX_H

#ifndef GRAPHICS_REV
#define GRAPHICS_REV 33 /* Current version of Graphics library (Exec 1.2) */
#endif

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef EXEC_ALERTS_H
#include <exec/alerts.h>
#endif

#define BITSET  0x8000
#define BITCLR  0
#define AGNUS
#ifdef AGNUS
#define TOBB(a)      ((long)(a))
#else
#define TOBB(a)      ((long)(a)>>1)
#endif

struct Rectangle
{
    SHORT   MinX,MinY;
    SHORT   MaxX,MaxY;
};

class RastPort;

struct Point
{
public:
    WORD x,y;

	Point(const WORD zx, const WORD zy) { x=zx; y=zy; }
	Point() { x=0; y=0; }
//	inline Point(RastPort& rp);
};

typedef UBYTE *PLANEPTR;

struct BitMap
{
	UWORD   BytesPerRow;
	UWORD   Rows;
	UBYTE   Flags;
	UBYTE   Depth;
	UWORD   pad;
	PLANEPTR Planes[8];
};

void InitBitMap(BitMap *b, const UWORD z, const UWORD x, const UWORD y);

PLANEPTR AllocRaster(const UWORD wid, const UWORD ht);
void FreeRaster(PLANEPTR p, UWORD wid, const UWORD ht);

inline LONG RASSIZE(const UWORD w, const UWORD h)
	{ return ((h)*( (w+15) >> 3 & 0xFFFE)); }

#endif
