
#ifndef  DEVICES_PRTBASE_H
#define  DEVICES_PRTBASE_H
#ifndef  EXEC_NODES_H
#include "exec/nodes.h"
#endif
#ifndef  EXEC_LISTS_H
#include "exec/lists.h"
#endif
#ifndef  EXEC_PORTS_H
#include "exec/ports.h"
#endif
#ifndef  EXEC_LIBRARIES_H
#include "exec/libraries.h"
#endif
#ifndef  EXEC_TASKS_H
#include "exec/tasks.h"
#endif
#ifndef  DEVICES_PARALLEL_H
// devices/prtbase.h
//
// (C) 1987, MTS Associates
//
#include "devices/parallel.h"
#endif
#ifndef  DEVICES_SERIAL_H
#include "devices/serial.h"
#endif
#ifndef  DEVICES_TIMER_H
#include "devices/timer.h"
#endif
#ifndef  LIBRARIES_DOSEXTENS_I
#include "libraries/dosextens.h"
#endif
#ifndef  INTUITION_INTUITION_H
#include "intuition/intuition.h"
#endif

struct DeviceData : public Library {
    APTR dd_Segment;
    APTR dd_ExecBase;
    APTR dd_CmdVectors;
    APTR dd_CmdBytes;
    UWORD   dd_NumCommands;
};

#define  P_STKSIZE   0x800

struct   PrinterData {
   struct	DeviceData pd_Device;
   MsgPort	pd_Unit;
   BPTR		pd_PrinterSegment;
   UWORD	pd_PrinterType;
   struct	PrinterSegment *pd_SegmentData;
   UBYTE	*pd_PrintBuf;
   int		(*pd_PWrite)(const char *buffer, const int len);
   int		(*pd_PBothReady)();
   union {
   struct IOExtPar pd_p0;
   struct IOExtSer pd_s0;
    } pd_ior0;
#define  pd_PIOR0 pd_ior0.pd_p0
#define  pd_SIOR0 pd_ior0.pd_s0
    union {
   struct IOExtPar pd_p1;
   struct IOExtSer pd_s1;
    } pd_ior1;
#define  pd_PIOR1 pd_ior1.pd_p1
#define  pd_SIOR1 pd_ior1.pd_s1
    struct timerequest pd_TIOR;
    MsgPort pd_IORPort;
    Task  pd_TC;
    UBYTE pd_Stk[P_STKSIZE];
    UBYTE pd_Flags;
    UBYTE pd_pad;
    struct Preferences pd_Preferences;
    UBYTE   pd_PWaitEnabled;

    operator PrinterSegment()
	{ return (PrinterSegment *) (pd_PrinterSegment << 2); }
};
#define  PPCB_GFX 0
#define  PPCF_GFX 0x01
#define  PPCB_COLOR  1
#define  PPCF_COLOR  0x02
#define  PPC_BWALPHA 0
#define  PPC_BWGFX   1
#define  PPC_COLORGFX   3
#define PCC_BW          1
#define PCC_YMC         2
#define PCC_YMC_BW      3
#define PCC_YMCB        4
#define PCC_4COLOR      0x4
#define PCC_ADDITIVE    0x8
#define PCC_WB          0x9
#define PCC_BGR         0xa
#define PCC_BGR_WB      0xb
#define PCC_BGRW        0xc

struct PrinterExtendedData {
    char    *ped_PrinterName;
    VOID    (*ped_Init)();
    VOID    (*ped_Expunge)();
    int     (*ped_Open)();
    VOID    (*ped_Close)();
    UBYTE   ped_PrinterClass;
    UBYTE   ped_ColorClass;
    UBYTE   ped_MaxColumns;
    UBYTE   ped_NumCharSets;
    UWORD   ped_NumRows;
    ULONG   ped_MaxXDots;
    ULONG   ped_MaxYDots;
    UWORD   ped_XDotsInch;
    UWORD   ped_YDotsInch;
    char    ***ped_Commands;
    int     (*ped_DoSpecial)();
    int     (*ped_Render)();
    LONG    ped_TimeoutSecs;
    char    **ped_8BitChars;
};

struct PrinterSegment {
    ULONG   ps_NextSegment;	// BPTR
    ULONG   ps_runAlert;
    UWORD   ps_Version;
    UWORD   ps_Revision;
    PrinterExtendedData ps_PED;

    PrinterSegment *next()
    { return (PrinterSegment *) (ps_NextSegment << 2)} ;
};
#endif
