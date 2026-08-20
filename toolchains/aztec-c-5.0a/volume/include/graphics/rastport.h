#ifndef	GRAPHICS_RASTPORT_H
#define	GRAPHICS_RASTPORT_H
#ifndef	GRAPHICS_GFX_H
#include	<graphics/gfx.h>
#endif
struct	AreaInfo
{
SHORT	*VctrTbl;
SHORT	*VctrPtr;
BYTE	*FlagTbl;
BYTE	*FlagPtr;
SHORT	Count;
SHORT	MaxCount;
SHORT	FirstX,FirstY;
};
struct	TmpRas
{
BYTE	*RasPtr;
LONG	Size;
};
struct	GelsInfo
{
BYTE	sprRsrvd;
UBYTE	Flags;
struct	VSprite	*gelHead,	*gelTail;
WORD	*nextLine;
WORD	**lastColor;
struct	collTable	*collHandler;
short	leftmost,	rightmost,	topmost,	bottommost;
APTR	firstBlissObj,lastBlissObj;
};
struct	RastPort
{
struct	Layer	*Layer;
struct	BitMap	*BitMap;
USHORT	*AreaPtrn;
struct	TmpRas	*TmpRas;
struct	AreaInfo	*AreaInfo;
struct	GelsInfo	*GelsInfo;
UBYTE	Mask;
BYTE	FgPen;
BYTE	BgPen;
BYTE	AOlPen;
BYTE	DrawMode;
BYTE	AreaPtSz;
BYTE	linpatcnt;
BYTE	dummy;
USHORT	Flags;
USHORT	LinePtrn;
SHORT	cp_x,	cp_y;
UBYTE	minterms[8];
SHORT	PenWidth;
SHORT	PenHeight;
struct	TextFont	*Font;
UBYTE	AlgoStyle;
UBYTE	TxFlags;
UWORD	TxHeight;
UWORD	TxWidth;
UWORD	TxBaseline;
WORD	TxSpacing;
APTR	*RP_User;
ULONG	longreserved[2];
#ifndef	GFX_RASTPORT_1_2
UWORD	wordreserved[7];
UBYTE	reserved[8];
#endif
};
#define	JAM1	0
#define	JAM2	1
#define	COMPLEMENT	2
#define	INVERSVID	4
#define	FRST_DOT	0x01
#define	ONE_DOT	0x02
#define	DBUFFER	0x04
#define	AREAOUTLINE	0x08
#define	NOCROSSFILL	0x20
#endif
