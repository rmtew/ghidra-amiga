#ifndef	EXEC_SEMAPHORES_H
#define	EXEC_SEMAPHORES_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif	/* !EXEC_LISTS_H */
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif	/* !EXEC_PORTS_H */
#ifndef	EXEC_TASKS_H
#include	"exec/tasks.h"
#endif	/* !EXEC_TASKS_H */
struct	Semaphore	{
struct	MsgPort	sm_MsgPort;
WORD	sm_Bids;
};
#define	sm_LockMsg	mp_SigTask
struct	SemaphoreRequest	{
struct	MinNode	sr_Link;
struct	Task	*sr_Waiter;
};
struct	SignalSemaphore	{
struct	Node	ss_Link;
SHORT	ss_NestCount;
struct	MinList	ss_WaitQueue;
struct	SemaphoreRequest	ss_MultipleLink;
struct	Task	*ss_Owner;
SHORT	ss_QueueCount;
};
#endif
