#ifndef	EXEC_NODES_H
#define	EXEC_NODES_H
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif /* !EXEC_TYPES_H */
struct	Node	{
struct	Node	*ln_Succ;
struct	Node	*ln_Pred;
UBYTE	ln_Type;
BYTE	ln_Pri;
char	*ln_Name;
};
struct	MinNode	{
struct	MinNode	*mln_Succ;
struct	MinNode	*mln_Pred;
};
#define	NT_UNKNOWN	0L
#define	NT_TASK	1L
#define	NT_INTERRUPT	2L
#define	NT_DEVICE	3L
#define	NT_MSGPORT	4L
#define	NT_MESSAGE	5L
#define	NT_FREEMSG	6L
#define	NT_REPLYMSG	7L
#define	NT_RESOURCE	8L
#define	NT_LIBRARY	9L
#define	NT_MEMORY	10L
#define	NT_SOFTINT	11L
#define	NT_FONT	12L
#define	NT_PROCESS	13L
#define	NT_SEMAPHORE	14L
#define	NT_SIGNALSEM	15L
#define	NT_BOOTNODE	16L
#endif
