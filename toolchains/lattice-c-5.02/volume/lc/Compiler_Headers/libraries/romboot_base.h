#ifndef LIBRARIES_ROMBOOT_BASE_H
#define LIBRARIES_ROMBOOT_BASE_H
/*
**	$Filename: libraries/romboot_base.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif
#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif
#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif
#ifndef EXEC_EXECBASE_H
#include <exec/execbase.h>
#endif
#ifndef EXEC_EXECNAME_H
#include <exec/execname.h>
#endif

struct RomBootBase
{
    struct Library  LibNode;
	struct ExecBase *ExecBase;
	struct List		BootList;
	ULONG			Reserved[4];	/* for future expansion */
};

struct BootNode
{
	struct Node bn_Node;
	UWORD	bn_Flags;
	CPTR	bn_DeviceNode;
};

#define ROMBOOT_NAME "romboot.library"

#endif	/* LIBRARIES_ROMBOOT_BASE_H */
