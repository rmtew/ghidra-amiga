#ifndef EXEC_MEMORY_H
#define EXEC_MEMORY_H
/*
**	$Filename: exec/memory.h $
**	$Release: 1.3 $
**
**	definitions for use with the memory allocator
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H


/****** MemChunk ****************************************************/

struct	MemChunk {
    struct  MemChunk *mc_Next;	/* pointer to next chunk */
    ULONG   mc_Bytes;		/* chunk byte size	*/
};


/****** MemHeader ***************************************************/

struct	MemHeader {
    struct  Node mh_Node;
    UWORD   mh_Attributes;	/* characteristics of this region */
    struct  MemChunk *mh_First; /* first free region		*/
    APTR    mh_Lower;		/* lower memory bound		*/
    APTR    mh_Upper;		/* upper memory bound+1		*/
    ULONG   mh_Free;		/* total number of free bytes	*/ 
};


/****** MemEntry ****************************************************/

struct	MemEntry {
union {
    ULONG   meu_Reqs;		/* the AllocMem requirements */
    APTR    meu_Addr;		/* the address of this memory region */
    } me_Un;
    ULONG   me_Length;		/* the length of this memory region */
};

#define me_un	    me_Un	/* compatability */
#define me_Reqs	    me_Un.meu_Reqs
#define me_Addr	    me_Un.meu_Addr


/****** MemList *****************************************************/

struct	MemList {
    struct  Node ml_Node;
    UWORD   ml_NumEntries;	/* number of entries in this struct */
    struct  MemEntry ml_ME[1];	/* the first entry	*/
};

#define ml_me	ml_ME		/* compatability */


/*----- Memory Requirement Types ---------------------------*/

#define MEMF_PUBLIC (1<<0)
#define MEMF_CHIP   (1<<1)
#define MEMF_FAST   (1<<2)

#define MEMF_CLEAR  (1L<<16)
#define MEMF_LARGEST (1L<<17)

#define MEM_BLOCKSIZE	8
#define MEM_BLOCKMASK	7

#endif	/* EXEC_MEMORY_H */
