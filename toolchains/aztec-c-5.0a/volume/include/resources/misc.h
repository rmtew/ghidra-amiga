#ifndef	RESOURCES_MISC_H
#define	RESOURCES_MISC_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif	/* !EXEC_TYPES_H */
#ifndef	EXEC_LIBRARIES_H
#include	"exec/libraries.h"
#endif	/* !EXEC_LIBRARIES_H */
#define	MR_SERIALPORT	0L
#define	MR_SERIALBITS	1L
#define	MR_PARALLELPORT	2L
#define	MR_PARALLELBITS	3L
#define	NUMMRTYPES	4L
struct	MiscResource	{
struct	Library	mr_Library;
ULONG	mr_AllocArray[NUMMRTYPES];
};
#define	MR_ALLOCMISCRESOURCE	(LIB_BASE)
#define	MR_FREEMISCRESOURCE	(LIB_BASE	+	LIB_VECSIZE)
#define	MISCNAME	"misc.resource"
#endif
