#ifndef WORKBENCH_ICON_H
#define WORKBENCH_ICON_H
/*
**	$Filename: workbench/icon.h $
**	$Release: 1.3 $
**
**	external declarations for workbench support library 
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

/********************************************************************
*
* library structures
*
********************************************************************/


#define ICONNAME	"icon.library"

/********************************************************************
*
* function types
*
********************************************************************/

struct WBObject *GetWBObject(), *AllocWBObject();
struct DiskObject *GetDiskObject();
LONG PutWBObject(), PutIcon(), GetIcon(), MatchToolValue();
VOID FreeFreeList(), FreeWBObject(), AddFreeList();
char *FindToolType();

#endif	/* WORKBENCH_ICON_H */
