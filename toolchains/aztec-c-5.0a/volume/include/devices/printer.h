#ifndef	DEVICES_PRINTER_H
#define	DEVICES_PRINTER_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif
#define	PRD_RAWWRITE	(CMD_NONSTD+0)
#define	PRD_PRTCOMMAND	(CMD_NONSTD+1)
#define	PRD_DUMPRPORT	(CMD_NONSTD+2)
#define	PRD_QUERY	(CMD_NONSTD+3)
#define	aRIS	0L
#define	aRIN	1L
#define	aIND	2L
#define	aNEL	3L
#define	aRI	4L
#define	aSGR0	5L
#define	aSGR3	6L
#define	aSGR23	7L
#define	aSGR4	8L
#define	aSGR24	9L
#define	aSGR1	10L
#define	aSGR22	11L
#define	aSFC	12L
#define	aSBC	13L
#define	aSHORP0	14L
#define	aSHORP2	15L
#define	aSHORP1	16L
#define	aSHORP4	17L
#define	aSHORP3	18L
#define	aSHORP6	19L
#define	aSHORP5	20L
#define	aDEN6	21L
#define	aDEN5	22L
#define	aDEN4	23L
#define	aDEN3	24L
#define	aDEN2	25L
#define	aDEN1	26L
#define	aSUS2	27L
#define	aSUS1	28L
#define	aSUS4	29L
#define	aSUS3	30L
#define	aSUS0	31L
#define	aPLU	32L
#define	aPLD	33L
#define	aFNT0	34L
#define	aFNT1	35L
#define	aFNT2	36L
#define	aFNT3	37L
#define	aFNT4	38L
#define	aFNT5	39L
#define	aFNT6	40L
#define	aFNT7	41L
#define	aFNT8	42L
#define	aFNT9	43L
#define	aFNT10	44L
#define	aPROP2	45L
#define	aPROP1	46L
#define	aPROP0	47L
#define	aTSS	48L
#define	aJFY5	49L
#define	aJFY7	50L
#define	aJFY6	51L
#define	aJFY0	52L
#define	aJFY3	53L
#define	aJFY1	54L
#define	aVERP0	55L
#define	aVERP1	56L
#define	aSLPP	57L
#define	aPERF	58L
#define	aPERF0	59L
#define	aLMS	60L
#define	aRMS	61L
#define	aTMS	62L
#define	aBMS	63L
#define	aSTBM	64L
#define	aSLRM	65L
#define	aCAM	66L
#define	aHTS	67L
#define	aVTS	68L
#define	aTBC0	69L
#define	aTBC3	70L
#define	aTBC1	71L
#define	aTBC4	72L
#define	aTBCALL	73L
#define	aTBSALL	74L
#define	aEXTEND	75L
#define	aRAW	76L
struct	IOPrtCmdReq	{
struct	Message	io_Message;
struct	Device	*io_Device;
struct	Unit	*io_Unit;
UWORD	io_Command;
UBYTE	io_Flags;
BYTE	io_Error;
UWORD	io_PrtCommand;
UBYTE	io_Parm0;
UBYTE	io_Parm1;
UBYTE	io_Parm2;
UBYTE	io_Parm3;
};
struct	IODRPReq	{
struct	Message	io_Message;
struct	Device	*io_Device;
struct	Unit	*io_Unit;
UWORD	io_Command;
UBYTE	io_Flags;
BYTE	io_Error;
struct	RastPort	*io_RastPort;
struct	ColorMap	*io_ColorMap;
ULONG	io_Modes;
UWORD	io_SrcX;
UWORD	io_SrcY;
UWORD	io_SrcWidth;
UWORD	io_SrcHeight;
LONG	io_DestCols;
LONG	io_DestRows;
UWORD	io_Special;
};
#define	SPECIAL_MILCOLS	0x0001L
#define	SPECIAL_MILROWS	0x0002L
#define	SPECIAL_FULLCOLS	0x0004L
#define	SPECIAL_FULLROWS	0x0008L
#define	SPECIAL_FRACCOLS	0x0010L
#define	SPECIAL_FRACROWS	0x0020L
#define	SPECIAL_CENTER	0x0040L
#define	SPECIAL_ASPECT	0x0080L
#define	SPECIAL_DENSITY1	0x0100L
#define	SPECIAL_DENSITY2	0x0200L
#define	SPECIAL_DENSITY3	0x0300L
#define	SPECIAL_DENSITY4	0x0400L
#define	SPECIAL_DENSITY5	0x0500L
#define	SPECIAL_DENSITY6	0x0600L
#define	SPECIAL_DENSITY7	0x0700L
#define	SPECIAL_NOFORMFEED	0x0800L
#define	SPECIAL_TRUSTME	0x1000L
#define	SPECIAL_NOPRINT	0x2000L
#define	PDERR_NOERR	0L
#define	PDERR_CANCEL	1L
#define	PDERR_NOTGRAPHICS	2L
#define	PDERR_INVERTHAM	3L
#define	PDERR_BADDIMENSION	4L
#define	PDERR_DIMENSIONOVFLOW	5L
#define	PDERR_INTERNALMEMORY	6L
#define	PDERR_BUFFERMEMORY	7L
#define	PDERR_TOOKCONTROL	8L
#define	SPECIAL_DENSITYMASK	0x0700L
#define	SPECIAL_DIMENSIONSMASK	\
(SPECIAL_MILCOLS|SPECIAL_MILROWS|SPECIAL_FULLCOLS|SPECIAL_FULLROWS\
|SPECIAL_FRACCOLS|SPECIAL_FRACROWS|SPECIAL_ASPECT)
#endif
