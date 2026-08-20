#ifndef EXEC_EXECBASE_H
#define EXEC_EXECBASE_H
/*
**	$Filename: exec/execbase.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_LISTS_H
#include "exec/lists.h"
#endif !EXEC_LISTS_H

#ifndef EXEC_INTERRUPTS_H
#include "exec/interrupts.h"
#endif !EXEC_INTERRUPTS_H

#ifndef EXEC_LIBRARIES_H
#include "exec/libraries.h"
#endif !EXEC_LIBRARIES_H

#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif !EXEC_TASKS_H


struct ExecBase {
	struct Library	LibNode;

	UWORD	SoftVer;	/* kickstart release number	 */
	WORD	LowMemChkSum;
	ULONG	ChkBase;	/* system base pointer complement */
	APTR	ColdCapture;	/* coldstart soft vector	 */
	APTR	CoolCapture;
	APTR	WarmCapture;
	APTR	SysStkUpper;	/* system stack base (upper bound) */
	APTR	SysStkLower;	/* top of system stack (lower bound) */
	ULONG	MaxLocMem;
	APTR	DebugEntry;
	APTR	DebugData;
	APTR	AlertData;
	APTR	MaxExtMem;	/* top of extended mem, or null if none */

	UWORD	ChkSum;

/****** Interrupt Related ***************************************/

	struct	IntVector IntVects[16];

/****** System Variables ****************************************/

	struct	Task *ThisTask; /* pointer to current task */
	ULONG	IdleCount;	/* idle counter */
	ULONG	DispCount;	/* dispatch counter */
	UWORD	Quantum;	/* time slice quantum */
	UWORD	Elapsed;	/* current quantum ticks */
	UWORD	SysFlags;	/* misc system flags */
	BYTE	IDNestCnt;	/* interrupt disable nesting count */
	BYTE	TDNestCnt;	/* task disable nesting count */

	UWORD	AttnFlags;	/* special attention flags */
	UWORD	AttnResched;	/* rescheduling attention */
	APTR	ResModules;	/* resident module array pointer */

	APTR	TaskTrapCode;
	APTR	TaskExceptCode;
	APTR	TaskExitCode;
	ULONG	TaskSigAlloc;
	UWORD	TaskTrapAlloc;


/****** System Lists ********************************************/

	struct	List MemList;
	struct	List ResourceList;
	struct	List DeviceList;
	struct	List IntrList;
	struct	List LibList;
	struct	List PortList;
	struct	List TaskReady;
	struct	List TaskWait;

	struct	SoftIntList SoftInts[5];

/****** Other Globals *******************************************/

	LONG	LastAlert[4];



	/* these next two variables are provided to allow
	** system developers to have a rough idea of the
	** period of two externally controlled signals --
	** the time between vertical blank interrupts and the
	** external line rate (which is counted by CIA A's
	** "time of day" clock).  In general these values
	** will be 50 or 60, and may or may not track each
	** other.  These values replace the obsolete AFB_PAL
	** and AFB_50HZ flags.
	*/
	UBYTE	VBlankFrequency;
	UBYTE	PowerSupplyFrequency;

	struct	List SemaphoreList;

	/* these next two are to be able to kickstart into user ram.
	** KickMemPtr holds a singly linked list of MemLists which
	** will be removed from the memory list via AllocAbs.  If
	** all the AllocAbs's succeeded, then the KickTagPtr will
	** be added to the rom tag list.
	*/
	APTR	KickMemPtr;	/* ptr to queue of mem lists */
	APTR	KickTagPtr;	/* ptr to rom tag queue */
	APTR	KickCheckSum;	/* checksum for mem and tags */

	UBYTE	ExecBaseReserved[10];
	UBYTE	ExecBaseNewReserved[20];
};

#define SYSBASESIZE  sizeof(struct ExecBase)

/******* AttnFlags */
/*  Processors and Co-processors: */
#define AFB_68010	0	/* also set for 68020 */
#define AFB_68020	1
#define AFB_68881	4

#define AFF_68010	(1<<0)
#define AFF_68020	(1<<1)
#define AFF_68881	(1<<4)

/* These two bits used to be AFB_PAL and AFB_50HZ.  After some soul
** searching we realized that they were misnomers, and the information
** is now kept in VBlankFrequency and PowerSupplyFrequency above.
** To find out what sort of video conversion is done, look in the
** graphics subsytem.
*/
#define AFB_RESERVED8	8
#define AFB_RESERVED9	9

#endif	/* EXEC_EXECBASE_H */
