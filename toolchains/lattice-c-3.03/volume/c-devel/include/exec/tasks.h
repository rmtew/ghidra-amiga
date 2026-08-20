#ifndef  EXEC_TASKS_H
#define  EXEC_TASKS_H


#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H

#ifndef EXEC_LISTS_H
#include "exec/lists.h"
#endif !EXEC_LISTS_H


extern struct Task {
    struct  Node tc_Node; 
    UBYTE   tc_Flags; 
    UBYTE   tc_State; 
    BYTE    tc_IDNestCnt;           
    BYTE    tc_TDNestCnt;           
    ULONG   tc_SigAlloc;            
    ULONG   tc_SigWait;             
    ULONG   tc_SigRecvd;            
    ULONG   tc_SigExcept;           
    UWORD   tc_TrapAlloc;           
    UWORD   tc_TrapAble;            
    APTR    tc_ExceptData;          
    APTR    tc_ExceptCode;          
    APTR    tc_TrapData;            
    APTR    tc_TrapCode;            
    APTR    tc_SPReg;               
    APTR    tc_SPLower;             
    APTR    tc_SPUpper;             
    VOID    (*tc_Switch)();         
    VOID    (*tc_Launch)();         
    struct  List tc_MemEntry;       
    APTR    tc_UserData;            
};


#define TB_PROCTIME  0 
#define TB_STACKCHK  4
#define TB_EXCEPT    5 
#define TB_SWITCH    6 
#define TB_LAUNCH    7 


#define TS_INVALID   0 
#define TS_ADDED     1 
#define TS_RUN       2 
#define TS_READY     3 
#define TS_WAIT      4 
#define TS_EXCEPT    5 
#define TS_REMOVED   6 



#define SIGF_ABORT   (1<<0)
#define SIGF_CHILD   (1<<1)
#define SIGF_BLIT    (1<<4)
#define SIGF_DOS     (1<<8)

#endif

