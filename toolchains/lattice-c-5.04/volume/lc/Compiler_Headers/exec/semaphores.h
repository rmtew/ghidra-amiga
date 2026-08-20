#ifndef EXEC_SEMAPHORES_H
#define EXEC_SEMAPHORES_H
/*
**	$Filename: exec/semaphores.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H

#ifndef EXEC_LISTS_H
#include "exec/lists.h"
#endif !EXEC_LISTS_H

#ifndef EXEC_PORTS_H
#include "exec/ports.h"
#endif !EXEC_PORTS_H

#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif !EXEC_TASKS_H


/****** Semaphore ***************************************************/

struct Semaphore {
    struct MsgPort sm_MsgPort;
    WORD   sm_Bids;
};

#define sm_LockMsg	mp_SigTask


/****** SignalSemaphore *********************************************/

/* this is the structure used to request a signal semaphore */
struct SemaphoreRequest {
    struct  MinNode sr_Link;
    struct  Task *sr_Waiter;
};

/* this is the actual semaphore itself */
struct SignalSemaphore {
    struct  Node ss_Link;
    SHORT   ss_NestCount;
    struct  MinList ss_WaitQueue;
    struct  SemaphoreRequest ss_MultipleLink;
    struct  Task *ss_Owner;
    SHORT   ss_QueueCount;
};

#endif	/* EXEC_SEMAPHORES_H */
