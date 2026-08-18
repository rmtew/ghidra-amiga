#ifndef	WORKBENCH_WORKBENCH_H
#define	WORKBENCH_WORKBENCH_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif	/* !EXEC_TYPES_H */
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif	/* !EXEC_LISTS_H */
#ifndef	EXEC_TASKS_H
#include	"exec/tasks.h"
#endif	/* !EXEC_TASKS_H */
#ifndef	INTUITION_INTUITION_H
#include	"intuition/intuition.h"
#endif	/* !INTUITION_INTUITION_H */
#define	WBDISK	1L
#define	WBDRAWER	2L
#define	WBTOOL	3L
#define	WBPROJECT	4L
#define	WBGARBAGE	5L
#define	WBDEVICE	6L
#define	WBKICK	7L
struct	DrawerData	{
struct	NewWindow	dd_NewWindow;
LONG	dd_CurrentX;
LONG	dd_CurrentY;
};
#define	DRAWERDATAFILESIZE	(sizeof(	struct	DrawerData	))
struct	DiskObject	{
UWORD	do_Magic;
UWORD	do_Version;
struct	Gadget	do_Gadget;
UBYTE	do_Type;
char	*	do_DefaultTool;
char	**	do_ToolTypes;
LONG	do_CurrentX;
LONG	do_CurrentY;
struct	DrawerData	*	do_DrawerData;
char	*	do_ToolWindow;
LONG	do_StackSize;
};
#define	WB_DISKMAGIC	0xe310L
#define	WB_DISKVERSION	1L
struct	FreeList	{
WORD	fl_NumFree;
struct	List	fl_MemList;
};
#define	MTYPE_PSTD	1L
#define	MTYPE_TOOLEXIT	2L
#define	MTYPE_DISKCHANGE	3L
#define	MTYPE_TIMER	4L
#define	MTYPE_CLOSEDOWN	5L
#define	MTYPE_IOPROC	6L
#define	GADGBACKFILL	0x0001L
#define	NO_ICON_POSITION	(0x80000000L)
#endif
