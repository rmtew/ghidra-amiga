// Workbench/workbench.h
//
// (C) 1987, MTS Associates
//

#ifndef WORKBENCH_WORKBENCH_H
#define WORKBENCH_WORKBENCH_H 1
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif !EXEC_TYPES_H
#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H
#ifndef EXEC_LISTS_H
#include "exec/lists.h"
#endif !EXEC_LISTS_H
#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif !EXEC_TASKS_H
#ifndef INTUITION_INTUITION_H
#include "intuition/intuition.h"
#endif !INTUITION_INTUITION_H

enum WBenchObjectType {
	WBDISK		= 1,
	WBDRAWER	= 2,
	WBTOOL		= 3,
	WBPROJECT	= 4,
	WBGARBAGE	= 5,
	WBDEVICE	= 6,
	WBKICK		= 7
};

struct DrawerData : public NewWindow {
    LONG                dd_CurrentX;
    LONG                dd_CurrentY;
};

#define DRAWERDATAFILESIZE      (sizeof( struct DrawerData ))

struct DiskObject {
    UWORD	do_Magic;
    UWORD	do_Version;
    Gadget	do_Gadget;
    UBYTE	do_Type;
    char *	do_DefaultTool;
    char **	do_ToolTypes;
    LONG	do_CurrentX;
    LONG	do_CurrentY;
    DrawerData	*do_DrawerData;
    char *	do_ToolWindow;
    LONG	do_StackSize;
};

#define WB_DISKMAGIC    0xe310
#define WB_DISKVERSION  1

struct FreeList {
    WORD	fl_NumFree;
    List	fl_MemList;
};

enum WorkBenchEventType {
	MTYPE_PSTD	= 1,
	MTYPE_TOOLEXIT	= 2,
	MTYPE_DISKCHANGE = 3,
	MTYPE_TIMER	= 4,
	MTYPE_CLOSEDOWN	= 5,
	MTYPE_IOPROC	= 6
};

#define GADGBACKFILL    0x0001
#define NO_ICON_POSITION (0x80000000)

#endif !WORKBEHCN_WORKBENCH_H
