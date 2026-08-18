#ifndef  EXEC_PORTS_H
#define  EXEC_PORTS_H


#ifndef EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H

#ifndef EXEC_LISTS_H
#include "exec/lists.h"
#endif !EXEC_LISTS_H

#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif !EXEC_TASKS_H




struct MsgPort {
    struct  Node mp_Node; 
    UBYTE   mp_Flags; 
    UBYTE   mp_SigBit;            
    struct  Task *mp_SigTask;     
    struct  List mp_MsgList;      
};

#define mp_SoftInt mp_SigTask

#define PF_ACTION    3

#define PA_SIGNAL    0
#define PA_SOFTINT   1
#define PA_IGNORE    2




struct Message {
    struct  Node mn_Node; 
    struct  MsgPort *mn_ReplyPort; 
    UWORD   mn_Length;            
};



struct Semaphore {
    struct MsgPort sm_MsgPort;
    WORD   sm_Bids;
};

#define  sm_LockMsg  mp_SigTask
#endif
