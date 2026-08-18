#ifndef EXEC_NODES_H
#define EXEC_NODES_H
/*
**	$Filename: exec/nodes.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

/* normal node */
struct Node { 
    struct  Node *ln_Succ;
    struct  Node *ln_Pred;
    UBYTE   ln_Type;
    BYTE    ln_Pri; 
    char    *ln_Name; 
};

/* stripped node -- no type checking is possible */
struct MinNode {
    struct MinNode *mln_Succ;
    struct MinNode *mln_Pred;
};


/*----- Node Types --------*/
#define NT_UNKNOWN	0 
#define NT_TASK		1 
#define NT_INTERRUPT	2	/* also for software interrupt node */
#define NT_DEVICE	3 
#define NT_MSGPORT	4 
#define NT_MESSAGE	5 
#define NT_FREEMSG	6 
#define NT_REPLYMSG	7 
#define NT_RESOURCE	8
#define NT_LIBRARY	9
#define NT_MEMORY	10
#define NT_SOFTINT	11	/* exec private */
#define NT_FONT		12
#define NT_PROCESS	13
#define NT_SEMAPHORE	14
#define NT_SIGNALSEM	15	/* signal semaphores */
#define NT_BOOTNODE	16

#endif	/* EXEC_NODES_H */
