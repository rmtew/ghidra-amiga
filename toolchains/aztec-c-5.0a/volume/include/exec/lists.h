#ifndef	EXEC_LISTS_H
#define	EXEC_LISTS_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
struct	List	{
struct	Node	*lh_Head;
struct	Node	*lh_Tail;
struct	Node	*lh_TailPred;
UBYTE	lh_Type;
UBYTE	l_pad;
};
struct	MinList	{
struct	MinNode	*mlh_Head;
struct	MinNode	*mlh_Tail;
struct	MinNode	*mlh_TailPred;
};
#endif
