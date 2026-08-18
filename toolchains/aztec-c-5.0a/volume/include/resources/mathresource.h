#ifndef	RESOURCES_MATHRESOURCE_H
#define	RESOURCES_MATHRESOURCE_H
#ifndef	EXEC_TYPES_H
#include	<exec/types.h>
#endif
#ifndef	EXEC_NODES_H
#include	<exec/nodes.h>
#endif
struct	MathIEEEResource
{
struct	Node	MathIEEEResource_Node;
unsigned	short	MathIEEEResource_Flags;
unsigned	short	*MathIEEEResource_BaseAddr;
void	(*MathIEEEResource_DblBasInit)();
void	(*MathIEEEResource_DblTransInit)();
void	(*MathIEEEResource_SglBasInit)();
void	(*MathIEEEResource_SglTransInit)();
void	(*MathIEEEResource_ExtBasInit)();
void	(*MathIEEEResource_ExtTransInit)();
};
#define	MATHIEEERESOURCEF_DBLBAS	(1<<0)
#define	MATHIEEERESOURCEF_DBLTRANS	(1<<1)
#define	MATHIEEERESOURCEF_SGLBAS	(1<<2)
#define	MATHIEEERESOURCEF_SGLTRANS	(1<<3)
#define	MATHIEEERESOURCEF_EXTBAS	(1<<4)
#define	MATHIEEERESOURCEF_EXTTRANS	(1<<5)
#endif
