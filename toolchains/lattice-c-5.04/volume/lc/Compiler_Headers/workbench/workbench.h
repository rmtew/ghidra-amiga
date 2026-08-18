#ifndef WORKBENCH_WORKBENCH_H
#define WORKBENCH_WORKBENCH_H
/*
**	$Filename: workbench/workbench.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

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

#define WBDISK		1
#define WBDRAWER	2
#define WBTOOL		3
#define WBPROJECT	4
#define WBGARBAGE	5
#define WBDEVICE	6
#define WBKICK		7

struct DrawerData {
    struct NewWindow	dd_NewWindow;	/* args to open window */
    LONG		dd_CurrentX;	/* current x coordinate of origin */
    LONG		dd_CurrentY;	/* current y coordinate of origin */
};

/* the amount of DrawerData actually written to disk */
#define DRAWERDATAFILESIZE	(sizeof( struct DrawerData ))


struct DiskObject {
    UWORD		do_Magic; /* a magic number at the start of the file */
    UWORD		do_Version; /* a version number, so we can change it */
    struct Gadget	do_Gadget;	/* a copy of in core gadget */
    UBYTE		do_Type;
    char *		do_DefaultTool;
    char **		do_ToolTypes;
    LONG		do_CurrentX;
    LONG		do_CurrentY;
    struct DrawerData * do_DrawerData;
    char *		do_ToolWindow;	/* only applies to tools */
    LONG		do_StackSize;	/* only applies to tools */

};

#define WB_DISKMAGIC	0xe310	/* a magic number, not easily impersonated */
#define WB_DISKVERSION	1	/* our current version number */

struct FreeList {
    WORD		fl_NumFree;
    struct List		fl_MemList;
};

/* each message that comes into the WorkBenchPort must have a type field
 * in the preceeding short.  These are the defines for this type
 */

#define MTYPE_PSTD		1	/* a "standard Potion" message */
#define MTYPE_TOOLEXIT		2	/* exit message from our tools */
#define MTYPE_DISKCHANGE	3	/* dos telling us of a disk change */
#define MTYPE_TIMER		4	/* we got a timer tick */
#define MTYPE_CLOSEDOWN		5	/* <unimplemented> */
#define MTYPE_IOPROC		6	/* <unimplemented> */

/* workbench does different complement modes for its gadgets.
 * It supports separate images, complement mode, and backfill mode.
 * The first two are identical to intuitions GADGIMAGE and GADGHCOMP.
 * backfill is similar to GADGHCOMP, but the region outside of the
 * image (which normally would be color three when complemented)
 * is flood-filled to color zero.
 */
#define GADGBACKFILL	0x0001

/* if an icon does not really live anywhere, set its current position
 * to here
 */
#define NO_ICON_POSITION	(0x80000000)

#endif	/* WORKBENCH_WORKBENCH_H */
