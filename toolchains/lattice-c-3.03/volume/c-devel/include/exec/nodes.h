#ifndef  EXEC_NODES_H
#define  EXEC_NODES_H



struct Node { 
    struct  Node *ln_Succ;
    struct  Node *ln_Pred;
    UBYTE   ln_Type;
    BYTE    ln_Pri; 
    char    *ln_Name; 
};


#define NT_UNKNOWN   0 
#define NT_TASK      1 
#define NT_INTERRUPT 2 
#define NT_DEVICE    3 
#define NT_MSGPORT   4 
#define NT_MESSAGE   5 
#define NT_FREEMSG   6 
#define NT_REPLYMSG  7 
#define NT_RESOURCE  8
#define NT_LIBRARY   9
#define NT_MEMORY    10
#define NT_SOFTINT   11
#define NT_FONT      12
#define NT_PROCESS   13
#define NT_SEMAPHORE 14

#endif
