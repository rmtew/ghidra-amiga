#ifndef EXEC_LISTS_H
#define EXEC_LISTS_H
/*
**	$Filename: exec/lists.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H


/* normal, full featured list */
struct List { 
    struct  Node *lh_Head;
    struct  Node *lh_Tail;
    struct  Node *lh_TailPred;
    UBYTE   lh_Type;
    UBYTE   l_pad;
};

/* minimum list -- no type checking possible */
struct MinList {
    struct MinNode *mlh_Head;
    struct MinNode *mlh_Tail;
    struct MinNode *mlh_TailPred;
};

#endif	/* EXEC_LISTS_H */
