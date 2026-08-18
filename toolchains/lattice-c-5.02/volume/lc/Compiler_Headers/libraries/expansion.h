#ifndef LIBRARIES_EXPANSION_H
#define LIBRARIES_EXPANSION_H
/*
**	$Filename: libraries/expansion.h $
**	$Release: 1.3 $
**
**	external definitions for expansion.library 
**
**	(C) Copyright 1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#define EXPANSIONNAME	"expansion.library"

/* flags for the AddDosNode() call */
#define ADNB_STARTPROC	0

#define ADNF_STARTPROC	(1<<0)


/* correct types for C programs */

struct ConfigDev *AllocConfigDev();
CPTR AllocExpansionMem();
struct ConfigDev *FindConfigDev();
struct DeviceNode *MakeDosNode();

#endif	/* LIBRARIES_EXPANSION_H */
