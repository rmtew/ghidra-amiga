#ifndef	EXEC_PORTS_H
#define	EXEC_PORTS_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif	/* !EXEC_LISTS_H */
#ifndef	EXEC_TASKS_H
#include	"exec/tasks.h"
#endif	/* !EXEC_TASKS_H */
struct	MsgPort	{
struct	Node	mp_Node;
UBYTE	mp_Flags;
UBYTE	mp_SigBit;
struct	Task	*mp_SigTask;
struct	List	mp_MsgList;
};
#define	mp_SoftInt	mp_SigTask
#define	PF_ACTION	3L
#define	PA_SIGNAL	0L
#define	PA_SOFTINT	1L
#define	PA_IGNORE	2L
struct	Message	{
struct	Node	mn_Node;
struct	MsgPort	*mn_ReplyPort;
UWORD	mn_Length;
};
#endif
