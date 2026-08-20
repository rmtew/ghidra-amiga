#ifndef EXEC_PORTS_H
#define EXEC_PORTS_H
/*
**	$Filename: exec/ports.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H

#ifndef EXEC_LISTS_H
#include "exec/lists.h"
#endif !EXEC_LISTS_H

#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif !EXEC_TASKS_H


/****** MsgPort *****************************************************/

struct MsgPort {
    struct  Node mp_Node; 
    UBYTE   mp_Flags; 
    UBYTE   mp_SigBit;		    /* signal bit number    */
    struct  Task *mp_SigTask;	    /* task to be signalled */
    struct  List mp_MsgList;	    /* message linked list  */
};

#define mp_SoftInt mp_SigTask

#define PF_ACTION	3

#define PA_SIGNAL	0
#define PA_SOFTINT	1
#define PA_IGNORE	2


/****** Message *****************************************************/

struct Message {
    struct  Node mn_Node; 
    struct  MsgPort *mn_ReplyPort;  /* message reply port */
    UWORD   mn_Length;		    /* message len in bytes */
};

#endif	/* EXEC_PORTS_H */
