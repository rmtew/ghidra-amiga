#ifndef	GRAPHICS_CLIP_H
#define	GRAPHICS_CLIP_H
#ifndef	GRAPHICS_GFX_H
#include	<graphics/gfx.h>
#endif
#ifndef	EXEC_SEMAPHORES_H
#include	<exec/semaphores.h>
#endif
#define	NEWLOCKS
struct	Layer
{
struct	Layer	*front,*back;
struct	ClipRect	*ClipRect;
struct	RastPort	*rp;
struct	Rectangle	bounds;
UBYTE	reserved[4];
UWORD	priority;
UWORD	Flags;
struct	BitMap	*SuperBitMap;
struct	ClipRect	*SuperClipRect;
APTR	Window;
SHORT	Scroll_X,Scroll_Y;
struct	ClipRect	*cr,*cr2,*crnew;
struct	ClipRect	*SuperSaveClipRects;
struct	ClipRect	*_cliprects;
struct	Layer_Info	*LayerInfo;
struct	SignalSemaphore	Lock;
UBYTE	reserved3[8];
struct	Region	*ClipRegion;
struct	Region	*saveClipRects;
UBYTE	reserved2[22];
struct	Region	*DamageList;
};
struct	ClipRect
{
struct	ClipRect	*Next;
struct	ClipRect	*prev;
struct	Layer	*lobs;
struct	BitMap	*BitMap;
struct	Rectangle	bounds;
struct	ClipRect	*_p1,*_p2;
LONG	reserved;
#ifdef	NEWCLIPRECTS_1_1
LONG	Flags;
#endif
};
#define	CR_NEEDS_NO_CONCEALED_RASTERS	1
#define	ISLESSX	1
#define	ISLESSY	2
#define	ISGRTRX	4
#define	ISGRTRY	8
#endif
