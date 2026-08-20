#ifndef	GRAPHICS_GFXBASE_H
#define	GRAPHICS_GFXBASE_H
#ifndef	EXEC_LISTS_H
#include	<exec/lists.h>
#endif
#ifndef	EXEC_LIBRARIES_H
#include	<exec/libraries.h>
#endif
#ifndef	EXEC_INTERRUPTS_H
#include	<exec/interrupts.h>
#endif
struct	GfxBase
{
struct	Library	LibNode;
struct	View	*ActiView;
struct	copinit	*copinit;
long	*cia;
long	*blitter;
UWORD	*LOFlist;
UWORD	*SHFlist;
struct	bltnode	*blthd,*blttl;
struct	bltnode	*bsblthd,*bsblttl;
struct	Interrupt	vbsrv,timsrv,bltsrv;
struct	List	TextFonts;
struct	TextFont	*DefaultFont;
UWORD	Modes;
BYTE	VBlank;
BYTE	Debug;
SHORT	BeamSync;
SHORT	system_bplcon0;
UBYTE	SpriteReserved;
UBYTE	bytereserved;
USHORT	Flags;
SHORT	BlitLock;
short	BlitNest;
struct	List	BlitWaitQ;
struct	Task	*BlitOwner;
struct	List	TOF_WaitQ;
UWORD	DisplayFlags;
struct	SimpleSprite	**SimpleSprites;
UWORD	MaxDisplayRow;
UWORD	MaxDisplayColumn;
UWORD	NormalDisplayRows;
UWORD	NormalDisplayColumns;
UWORD	NormalDPMX;
UWORD	NormalDPMY;
struct	SignalSemaphore	*LastChanceMemory;
UWORD	*LCMptr;
UWORD	MicrosPerLine;
UWORD	MinDisplayColumn;
ULONG	reserved[23];
};
#define	NTSC	1
#define	GENLOC	2
#define	PAL	4
#define	BLITMSG_FAULT	4
#endif
