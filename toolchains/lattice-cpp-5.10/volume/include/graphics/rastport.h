/* Rastport.h - graphics library function prototypes */

/*
**	Copyright (C) 1986, by MTS Associates, Jacksonville FL
**	All rights reserved.  Unauthorized duplication is a violation
**	of copyright law.
**
**	Function prototypes for the exec.library of Amiga EXEC
**
**	Updated: 02/04/87, TFH
*/
/* rastport.h */
#ifndef GRAPHICS_RASTPORT_H
#define GRAPHICS_RASTPORT_H

#ifndef EXEC_MEMORY_H
#include <exec/memory.h>
#endif

#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif

/* unoptimized for 32bit alignment of pointers */
struct GelsInfo
{
    BYTE sprRsrvd;  /* flag of which sprites to reserve from vsprite system */
    UBYTE Flags;  /* system use */
    struct VSprite *gelHead, *gelTail; /* dummy vSprites for list management */
    /* pointer to array of 8 WORDS for sprite available lines */
    WORD *nextLine;
    /* pointer to array of 8 pointers for color-last-assigned to vSprites */
    WORD **lastColor;
    struct collTable *collHandler;  /* addresses of collision routines */
    short leftmost, rightmost, topmost, bottommost;
	APTR firstBlissObj,lastBlissObj;	/* system use only */
};

enum DrawMode {/* drawing modes */
	JAM1 = 0,		// jam 1 color into raster
	JAM2 = 1,		// jam 2 colors into raster
	COMPLEMENT = 2,		// XOR bits into raster
	INVERSVID = 4	// inverse video for drawing modes
};

class TextFont;
class Layer;
class BitMap;
class AreaInfo;
class TmpRas;

extern int strlen(const char *s);

struct RastPort
{
    Layer	*Layer;
    BitMap	*BitMap;
    const UWORD	*AreaPtrn;
    TmpRas	*TmpRas;
    AreaInfo	*AreaInfo;
    GelsInfo	*GelsInfo;
    UBYTE	Mask;		/* write mask for this raster */
    BYTE	FgPen;		/* foreground pen for this raster */
    BYTE	BgPen;		/* background pen  */
    BYTE	AOlPen;		/* areafill outline pen */
    BYTE	DrawMode;	/* drawing mode for fill, lines, and text */
    BYTE	AreaPtSz;	/* 2^n words for areafill pattern */
    BYTE	linpatcnt;	/* current line drawing pattern preshift */
    BYTE	dummy;
    USHORT	Flags;		/* miscellaneous control bits */
    USHORT	LinePtrn;	/* 16 bits for textured lines */
    SHORT	cp_x, cp_y;	/* current pen position */
    UBYTE	minterms[8];
    SHORT	PenWidth;
    SHORT	PenHeight;
    TextFont	*Font;		/* current font address */
    UBYTE	AlgoStyle;	/* the algorithmically generated style */
    UBYTE	TxFlags;	/* text specific flags */
    UWORD	TxHeight;	/* text height */
    UWORD	TxWidth;	/* text nominal width */
    UWORD	TxBaseline;	/* text baseline */
    WORD	TxSpacing;      /* text spacing (per character) */
    APTR	*RP_User;
    UWORD	wordreserved[7];	/* used to be a node */
    ULONG	longreserved[2];
    UBYTE	reserved[8];	/* for future use */

	friend void InitRastPort( const RastPort *r);
	RastPort(const UBYTE fpen, const UBYTE bpen)
	{ InitRastPort(this); FgPen = fpen; BgPen = bpen; }
	RastPort() {}	// So Intuition doesn't go crazy

friend void DrawEllipse( const RastPort *r,
		const SHORT x, const SHORT y,
		const SHORT maj, const SHORT mn);
friend void Draw (const RastPort *r, const SHORT x, const SHORT y);
friend void Text(const RastPort *r, const UBYTE *tx, const int len);
friend int  TextLength(const RastPort *r, const UBYTE *tx, const SHORT len);

	void circle(const USHORT r)
		{ DrawEllipse(this, cp_x, cp_y, r, r); } 
	void ellipse(const USHORT r1,const USHORT r2)
		{ DrawEllipse(this, cp_x, cp_y, r1, r2); }
	void line (USHORT x2, USHORT y2)
		{ Draw (this, x2, y2); }
	void move (const USHORT x, const USHORT y) { Move(this, x, y); }
	void delta(const SHORT x, const SHORT y)
	{ Move (this, cp_x+x, cp_y+y); }

	void text (const TEXT *s) { (void) Text(this, s, strlen(s)); }

	friend Point& operator=(Point& p, RastPort &rp)
	{ p.x = rp.cp_x; p.y = rp.cp_y; return p; }

	friend RastPort& operator=(RastPort& rp, const Point p)
	{ Move(&rp, p.x, p.y); return rp; }

	friend RastPort& operator+=(RastPort& rp, const Point p)
	{ Move(&rp, rp.cp_x + p.x, rp.cp_y + p.y); return rp; }

//	Elementary Text functions:

	RastPort& operator<<(char c)
	{ char xx = c; (void) Text(this, &xx, 1); return *this;}
	RastPort& operator<<(char * s)
	{ (void) Text(this, s, strlen(s)); return *this;}
//	RastPort operator<<(int i);

	friend void BltBitMapRastPort(struct BitMap *bm,
		const UWORD srcX, const UWORD srcY,
		const RastPort *dest,
		const UWORD destX, const UWORD destY,
		const UWORD sizeX, const UWORD sizeY,
		const UBYTE minterm);

	friend BltMaskBitMapRastPort(struct BitMap* bm,
		const UWORD srcX, const UWORD srcY,
		const RastPort *r,
		const UWORD destX, const UWORD destY,
		const UWORD sizeX, const UWORD sizeY,
		const UBYTE minterm, APTR bltmask);

	friend void BltPattern( const RastPort *r, const APTR pat,
		const USHORT xl, const USHORT yl,
		const USHORT maxX, const USHORT maxY,
		const SHORT bytecnt);

friend void BltTemplate(APTR a, const SHORT b, const SHORT c,
		 const RastPort *r, const SHORT d, const SHORT e,
		const SHORT f, const SHORT g);

friend void ClearEOL( const RastPort *r);

friend void ClearScreen( const RastPort *r);
friend void ClipBlit( const RastPort *r, const int a, const int b,
		 const RastPort *r, const int c, const int d,
		const int e, const int f,
		const UBYTE m);
friend BOOL GetGBuffers(APTR x,  const RastPort *r, const BOOL b);

friend void Move( RastPort *, const SHORT x, const SHORT y);
friend void PolyDraw( RastPort *, SHORT a, const SHORT *d);
friend LONG ReadPixel( RastPort *, const SHORT x, const SHORT y);
friend void RectFill( RastPort *, const SHORT x1, const SHORT y1,
	 const SHORT x2, const SHORT y2);

friend void ScrollRaster(const RastPort *rp,
	const SHORT dx, SHORT dy,
	const SHORT xmin, SHORT ymin,
	const SHORT xmax, SHORT ymax);

friend void SetAPen(const RastPort *rp, const UBYTE pen);
friend void SetBPen(const RastPort *rp, const UBYTE pen);
friend void SetDrMd(const RastPort *rp, const UBYTE mode);

friend void SetOPen(const RastPort *rp, const UBYTE pen);
friend void SetRast(const RastPort *rp, const UBYTE b);

friend LONG WritePixel(const RastPort *rp, const SHORT x, const SHORT y);

};

//Point::Point(RastPort& rp)	// create a Point from RastPort
//{
//	this = rp;		// ... looks like we have a bug here...
//}

/* these are the flag bits for RastPort flags */
#define FRST_DOT    0x01    /* draw the first dot of this line ? */
#define ONE_DOT     0x02    /* use one dot mode for drawing lines */
#define DBUFFER     0x04    /* flag set when RastPorts are double-buffered */
			    /* only used for bobs */

#define AREAOUTLINE 0x08
#define NOCROSSFILL 0x20

#endif
