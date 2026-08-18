#ifndef	EXEC_INTERRUPTS_H
#define	EXEC_INTERRUPTS_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif	/* !EXEC_LISTS_H */
struct	Interrupt	{
struct	Node	is_Node;
APTR	is_Data;
VOID	(*is_Code)();
};
struct	IntVector	{
APTR	iv_Data;
VOID	(*iv_Code)();
struct	Node	*iv_Node;
};
struct	SoftIntList	{
struct	List	sh_List;
UWORD	sh_Pad;
};
#define	SIH_PRIMASK	(0xf0L)
#define	INTB_NMI	15L
#define	INTF_NMI	(1L<<15)
#endif
