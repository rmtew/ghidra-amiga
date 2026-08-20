/* graphics/areas.h - graphics library function prototypes */

/*
**	Copyright (C) 1986, by MTS Associates, Jacksonville FL
**	All rights reserved.  Unauthorized duplication is a violation
**	copyright law.
**
**	Function prototypes for the exec.library of Amiga EXEC
**
**	Updated: 04/10/87, TFH
*/

#ifndef GRAPHICS_AREAS_H
#define GRAPHICS_AREAS_H

#ifndef EXEC_MEMORY_H
#include <exec/memory.h>
#endif

#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif

#ifndef GRAPHICS_RASTPORT_H
#include <graphics/rastport.h>
#endif

struct AreaInfo		// Use as base class for area array def.
{
    SHORT   *VctrTbl;       /* ptr to start of vector table */
    SHORT   *VctrPtr;       /* ptr to current vertex */
    BYTE    *FlagTbl;       /* ptr to start of vector flag table */
    BYTE    *FlagPtr;       /* ptrs to areafill flags */
    SHORT   Count;
    SHORT   MaxCount;
    SHORT   FirstX,FirstY;

	friend void InitArea(AreaInfo *a, UWORD *b, UWORD maxv);

	AreaInfo(unsigned int points)
	{ UWORD *buffer = (UWORD *)AllocMem(points * 5, MEMF_PUBLIC);
	  InitArea(this, buffer, Count = points);
	}
	~AreaInfo() { FreeMem(VctrTbl, Count *5);}
	int error() { return VctrTbl == NULL; }
	void init() { InitArea (this, VctrTbl, Count); }
};

struct TmpRas
{
    PLANEPTR /*BYTE* */ RasPtr;
    LONG Size;
    /* other misc junk for freelist etc. */
};


// Use the following to cleanly allocate and free a temporary raster

struct ScratchRaster : public TmpRas {
	UWORD width, height;
	friend void InitTmpRas(TmpRas *t, PLANEPTR r, const UWORD Size); 

	ScratchRaster(const UWORD wid, const UWORD ht)
	{
	  RasPtr = AllocRaster (wid, ht); width=wid; height=ht;
	  Size = RASSIZE(wid, ht);
	  InitTmpRas(this, RasPtr, Size); 
	}
	~ScratchRaster()
	{ FreeRaster(RasPtr, width, height); }
	int error() { return RasPtr == 0; }
};

int AreaCircle(const RastPort *r, const int x, const int y,
	 const int r);

int AreaDraw(const RastPort *r, const int x, const int y);
int AreaEllipse( const RastPort *r, const int x, const int y,
	const int maj, const int mn);
LONG AreaEnd( const RastPort *r);
LONG AreaMove( const RastPort *r, const int x, const int y);
BOOL Flood( const RastPort *r,
	const ULONG a, const SHORT b, const SHORT c);

#endif
