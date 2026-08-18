#ifndef	GRAPHICS_VIEW_H
#define	GRAPHICS_VIEW_H
#ifndef	GRAPHICS_GFX_H
#include	<graphics/gfx.h>
#endif
#ifndef	GRAPHICS_COPPER_H
#include	<graphics/copper.h>
#endif
struct	ColorMap
{
UBYTE	Flags;
UBYTE	Type;
UWORD	Count;
APTR	ColorTable;
};
struct	ViewPort
{
struct	ViewPort	*Next;
struct	ColorMap	*ColorMap;
struct	CopList	*DspIns;
struct	CopList	*SprIns;
struct	CopList	*ClrIns;
struct	UCopList	*UCopIns;
SHORT	DWidth,DHeight;
SHORT	DxOffset,DyOffset;
UWORD	Modes;
UBYTE	SpritePriorities;
UBYTE	reserved;
struct	RasInfo	*RasInfo;
};
struct	View
{
struct	ViewPort	*ViewPort;
struct	cprlist	*LOFCprList;
struct	cprlist	*SHFCprList;
short	DyOffset,DxOffset;
UWORD	Modes;
};
#define	PFBA	0x40
#define	DUALPF	0x400
#define	HIRES	0x8000
#define	LACE	4
#define	HAM	0x800
#define	SPRITES	0x4000
#define	VP_HIDE	0x2000
#define	GENLOCK_AUDIO	0x100
#define	GENLOCK_VIDEO	2
#define	EXTRA_HALFBRITE	0x80
struct	RasInfo
{
struct	RasInfo	*Next;
struct	BitMap	*BitMap;
SHORT	RxOffset,RyOffset;
};
#endif
