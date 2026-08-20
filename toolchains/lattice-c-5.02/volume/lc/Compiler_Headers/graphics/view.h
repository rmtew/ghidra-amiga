#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H
/*
**	$Filename: graphics/view.h $
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

#ifndef GRAPHICS_COPPER_H
#include <graphics/copper.h>
#endif

struct ColorMap
{
   UBYTE Flags;
   UBYTE Type;
   UWORD Count;
   APTR	 ColorTable;
};
/* if Type == 0 then ColorTable is a table of UWORDS xRGB */

struct ViewPort
{
   struct   ViewPort *Next;
   struct   ColorMap  *ColorMap;    /* table of colors for this viewport */
		  /* if this is nil, MakeVPort assumes default values */
   struct   CopList  *DspIns;	    /* user by MakeView() */
   struct   CopList  *SprIns;	    /* used by sprite stuff */
   struct   CopList  *ClrIns;	    /* used by sprite stuff */
   struct   UCopList *UCopIns;	    /* User copper list */
   SHORT    DWidth,DHeight;
   SHORT    DxOffset,DyOffset;
   UWORD    Modes;
   UBYTE	SpritePriorities;		/* used by makevp */
   UBYTE	reserved;
   struct   RasInfo *RasInfo;
};

struct View
{
   struct ViewPort *ViewPort;
   struct cprlist *LOFCprList;	 /* used for interlaced and noninterlaced */
   struct cprlist *SHFCprList;	 /* only used during interlace */
   short DyOffset,DxOffset;   /* for complete View positioning */
			      /* offsets are +- adjustments to standard #s */
   UWORD   Modes;	      /* such as INTERLACE, GENLOC */
};

/* defines used for Modes in IVPargs */
#define PFBA		0x40
#define DUALPF		0x400
#define HIRES		0x8000
#define LACE		4
#define HAM		0x800
#define SPRITES		0x4000	    /* reuse one of plane ctr bits */
#define VP_HIDE		0x2000	    /* reuse another plane crt bit */
#define GENLOCK_AUDIO	0x100
#define GENLOCK_VIDEO	2
#define EXTRA_HALFBRITE 0x80

struct RasInfo	/* used by callers to and InitDspC() */
{
   struct   RasInfo *Next;	    /* used for dualpf */
   struct   BitMap *BitMap;
   SHORT    RxOffset,RyOffset;	    /* scroll offsets in this BitMap */
};

#endif	/* GRAPHICS_VIEW_H */
