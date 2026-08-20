#ifndef	DEVICES_PRTBASE_H
#define	DEVICES_PRTBASE_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif
#ifndef	EXEC_LIBRARIES_H
#include	"exec/libraries.h"
#endif
#ifndef	EXEC_TASKS_H
#include	"exec/tasks.h"
#endif
#ifndef	DEVICES_PARALLEL_H
#include	"devices/parallel.h"
#endif
#ifndef	DEVICES_SERIAL_H
#include	"devices/serial.h"
#endif
#ifndef	DEVICES_TIMER_H
#include	"devices/timer.h"
#endif
#ifndef	LIBRARIES_DOSEXTENS_I
#include	"libraries/dosextens.h"
#endif
#ifndef	INTUITION_INTUITION_H
#include	"intuition/intuition.h"
#endif
struct	DeviceData	{
struct	Library	dd_Device;
APTR	dd_Segment;
APTR	dd_ExecBase;
APTR	dd_CmdVectors;
APTR	dd_CmdBytes;
UWORD	dd_NumCommands;
};
#define	P_STKSIZE	0x0800L
#define	P_BUFSIZE	256L
#define	P_SAFESIZE	128L
struct	PrinterData	{
struct	DeviceData	pd_Device;
struct	MsgPort	pd_Unit;
BPTR	pd_PrinterSegment;
UWORD	pd_PrinterType;
struct	PrinterSegment	*pd_SegmentData;
UBYTE	*pd_PrintBuf;
int	(*pd_PWrite)();
int	(*pd_PBothReady)();
union	{
struct	IOExtPar	pd_p0;
struct	IOExtSer	pd_s0;
}	pd_ior0;
#define	pd_PIOR0	pd_ior0.pd_p0
#define	pd_SIOR0	pd_ior0.pd_s0
union	{
struct	IOExtPar	pd_p1;
struct	IOExtSer	pd_s1;
}	pd_ior1;
#define	pd_PIOR1	pd_ior1.pd_p1
#define	pd_SIOR1	pd_ior1.pd_s1
struct	timerequest	pd_TIOR;
struct	MsgPort	pd_IORPort;
struct	Task	pd_TC;
UBYTE	pd_Stk[P_STKSIZE];
UBYTE	pd_Flags;
UBYTE	pd_pad;
struct	Preferences	pd_Preferences;
UBYTE	pd_PWaitEnabled;
};
#define	PPCB_GFX	0L
#define	PPCF_GFX	0x1L
#define	PPCB_COLOR	1L
#define	PPCF_COLOR	0x2L
#define	PPC_BWALPHA	0x00L
#define	PPC_BWGFX	0x01L
#define	PPC_COLORALPHA	0x02L
#define	PPC_COLORGFX	0x03L
#define	PCC_BW	0x01L
#define	PCC_YMC	0x02L
#define	PCC_YMC_BW	0x03L
#define	PCC_YMCB	0x04L
#define	PCC_4COLOR	0x04L
#define	PCC_ADDITIVE	0x08L
#define	PCC_WB	0x09L
#define	PCC_BGR	0x0AL
#define	PCC_BGR_WB	0x0BL
#define	PCC_BGRW	0x0CL
#define	PCC_MULTI_PASS	0x10L
struct	PrinterExtendedData	{
char	*ped_PrinterName;
VOID	(*ped_Init)();
VOID	(*ped_Expunge)();
int	(*ped_Open)();
VOID	(*ped_Close)();
UBYTE	ped_PrinterClass;
UBYTE	ped_ColorClass;
UBYTE	ped_MaxColumns;
UBYTE	ped_NumCharSets;
UWORD	ped_NumRows;
ULONG	ped_MaxXDots;
ULONG	ped_MaxYDots;
UWORD	ped_XDotsInch;
UWORD	ped_YDotsInch;
char	***ped_Commands;
int	(*ped_DoSpecial)();
int	(*ped_Render)();
LONG	ped_TimeoutSecs;
char	**ped_8BitChars;
LONG	ped_PrintMode;
int	(*ped_ConvFunc)();
};
struct	PrinterSegment	{
ULONG	ps_NextSegment;
ULONG	ps_runAlert;
UWORD	ps_Version;
UWORD	ps_Revision;
struct	PrinterExtendedData	ps_PED;
};
#endif
