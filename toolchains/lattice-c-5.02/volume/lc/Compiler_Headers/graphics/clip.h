#ifndef GRAPHICS_CLIP_H
#define GRAPHICS_CLIP_H
/*
**	$Filename: graphics/clip.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif
#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

/* structures used by and constructed by windowlib.a */
/* understood by rom software */

#define NEWLOCKS

struct Layer
{
    struct  Layer *front,*back;	    /* ignored by roms */
    struct  ClipRect	*ClipRect;  /* read by roms to find first cliprect */
    struct  RastPort	*rp;	    /* ignored by roms, I hope */
    struct  Rectangle	bounds;	    /* ignored by roms */
	UBYTE	reserved[4];
	UWORD	priority;				/* system use only */
    UWORD   Flags;		    /* obscured ?, Virtual BitMap? */
    struct  BitMap *SuperBitMap;
    struct  ClipRect *SuperClipRect; /* super bitmap cliprects if 
					VBitMap != 0*/
				  /* else damage cliprect list for refresh */
    APTR    Window;		    /* reserved for user interface use */
    SHORT   Scroll_X,Scroll_Y;
	struct	ClipRect *cr,*cr2,*crnew;  /* used by dedice */
	struct	ClipRect *SuperSaveClipRects; /* preallocated cr's */
	struct	ClipRect *_cliprects;	/* system use during refresh */
	struct	Layer_Info  *LayerInfo; /* points to head of the list */
	struct	SignalSemaphore Lock;
	UBYTE	reserved3[8];
	struct Region *ClipRegion;
	struct Region *saveClipRects;	/* used to back out when in trouble*/
	UBYTE	reserved2[22];
	/* this must stay here */
    struct  Region  *DamageList;    /* list of rectangles to refresh 
				       through */
};

struct ClipRect
{
    struct  ClipRect *Next;	    /* roms used to find next ClipRect */
    struct  ClipRect *prev;	    /* ignored by roms, used by windowlib */
    struct  Layer   *lobs;	    /* ignored by roms, used by windowlib */
    struct  BitMap  *BitMap;
    struct  Rectangle	bounds;	    /* set up by windowlib, used by roms */
    struct  ClipRect *_p1,*_p2;	    /* system reserved */
    LONG    reserved;		    /* system use */
#ifdef NEWCLIPRECTS_1_1
    LONG    Flags;		    /* only exists in layer allocation */
#endif
};

/* internal cliprect flags */
#define CR_NEEDS_NO_CONCEALED_RASTERS  1

/* defines for code values for getcode */
#define ISLESSX 1
#define ISLESSY 2
#define ISGRTRX 4
#define ISGRTRY 8

#endif	/* GRAPHICS_CLIP_H */
