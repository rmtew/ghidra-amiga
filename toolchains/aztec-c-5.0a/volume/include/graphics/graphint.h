#ifndef	GRAPHICS_GRAPHINT_H
#define	GRAPHICS_GRAPHINT_H
#ifndef	EXEC_NODES_H
#include	<exec/nodes.h>
#endif
struct	Isrvstr
{
struct	Node	is_Node;
struct	Isrvstr	*Iptr;
int	(*code)();
int	(*ccode)();
int	Carg;
};
#endif
