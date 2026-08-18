#ifndef LIBRARIES_EXPANSIONBASE_H
#define LIBRARIES_EXPANSIONBASE_H
/*
**	$Filename: libraries/expansionbase.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif	!EXEC_TYPES_H

#ifndef EXEC_LIBRARIES_H
#include "exec/libraries.h"
#endif	!EXEC_LIBRARIES_H

#ifndef EXEC_INTERRUPTS_H
#include "exec/interrupts.h"
#endif	!EXEC_INTERRUPTS_H

#ifndef EXEC_SEMAPHORES_H
#include "exec/semaphores.h"
#endif	!EXEC_SEMAPHORES_H

#ifndef LIBRARIES_CONFIGVARS_H
#include "libraries/configvars.h"
#endif	!LIBRARIES_CONFIGVARS_H

#define TOTALSLOTS 256

struct ExpansionInt
{
	UWORD	IntMask;
	UWORD	ArrayMax;
	UWORD	ArraySize;
};


struct	ExpansionBase
{
	struct Library	LibNode;
	UBYTE	Flags;
	UBYTE	pad;
	APTR	ExecBase;
	APTR	SegList;
	struct	CurrentBinding	CurrentBinding;
	struct	List	BoardList;
	struct	List	MountList;
	UBYTE	AllocTable[TOTALSLOTS];
	struct	SignalSemaphore BindSemaphore;
	struct	Interrupt	Int2List;
	struct	Interrupt	Int6List;
	struct	Interrupt	Int7List;
};

#endif	/* LIBRARIES_EXPANSIONBASE_H */
