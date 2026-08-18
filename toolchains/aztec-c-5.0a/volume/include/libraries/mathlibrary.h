#ifndef	LIBRARIES_MATHLIBRARY_H
#define	LIBRARIES_MATHLIBRARY_H
#ifndef	EXEC_TYPES_H
#include	<exec/types.h>
#endif
#ifndef	EXEC_LIBRARIES_H
#include	<exec/libraries.h>
#endif
struct	MathIEEEBase
{
struct	Library	MathIEEEBase_LibNode;
unsigned	char	MathIEEEBase_Flags;
unsigned	char	MathIEEEBase_reserved1;
unsigned	short	*MathIEEEBase_68881;
APTR	MathIEEEBase_SysLib;
APTR	MathIEEEBase_SegList;
struct	MathIEEEResource	*MathIEEEBase_Resource;
int	(*MathIEEEBase_TaskOpenLib)();
int	(*MathIEEEBase_TaskCloseLib)();
};
#endif
