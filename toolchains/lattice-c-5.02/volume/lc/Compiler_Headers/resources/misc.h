#ifndef RESOURCES_MISC_H
#define RESOURCES_MISC_H
/*
**	$Filename: resources/misc.h $
**	$Release: 1.3 $
**
**	external declarations for misc system resources 
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include "exec/types.h" 
#endif	!EXEC_TYPES_H

#ifndef EXEC_LIBRARIES_H
#include "exec/libraries.h" 
#endif	!EXEC_LIBRARIES_H


/********************************************************************
*
* Resource structures
*
********************************************************************/

#define MR_SERIALPORT	0
#define MR_SERIALBITS	1
#define MR_PARALLELPORT 2
#define MR_PARALLELBITS 3

#define NUMMRTYPES	4

struct MiscResource {
    struct Library mr_Library;
    ULONG mr_AllocArray[NUMMRTYPES];
};

#define MR_ALLOCMISCRESOURCE	(LIB_BASE)
#define MR_FREEMISCRESOURCE	(LIB_BASE + LIB_VECSIZE)


#define MISCNAME "misc.resource"

#endif	/* RESOURCES_MISC_H */
