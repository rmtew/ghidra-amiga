#ifndef  EXEC_EXECBASE_H
#define  EXEC_EXECBASE_H


#ifndef EXEC_LISTS_H
#include "exec/libraries.h"
#endif !EXEC_LISTS_H

#ifndef EXEC_INTERRUPTS_H
#include "exec/interrupts.h"
#endif !EXEC_INTERRUPTS_H

#ifndef EXEC_LIBRARIES_H
#include "exec/libraries.h"
#endif !EXEC_LIBRARIES_H

#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif !EXEC_TASKS_H


struct ExecBase {
   struct Library  LibNode;

   UWORD SoftVer;       
   WORD  LowMemChkSum;
   ULONG ChkBase;       
   APTR  ColdCapture;   
   APTR  CoolCapture;
   APTR  WarmCapture;
   APTR  SysStkUpper;   
   APTR  SysStkLower;   
   ULONG MaxLocMem;
   APTR  DebugEntry;
   APTR  DebugData;
   APTR  AlertData;
   APTR  RsvdExt;

   UWORD ChkSum;



   struct   IntVector IntVects[16];



   struct   Task *ThisTask;   
   ULONG IdleCount;           
   ULONG DispCount;           
   UWORD Quantum;             
   UWORD Elapsed;             
   UWORD SysFlags;            
   BYTE  IDNestCnt;           
   BYTE  TDNestCnt;           

   UWORD AttnFlags;           
   UWORD AttnResched;         
   APTR  ResModules;          

   APTR  TaskTrapCode;
   APTR  TaskExceptCode;
   APTR  TaskExitCode;
   ULONG TaskSigAlloc;
   UWORD TaskTrapAlloc;




   struct   List MemList;
   struct   List ResourceList;
   struct   List DeviceList;
   struct   List IntrList;
   struct   List LibList;
   struct   List PortList;
   struct   List TaskReady;
   struct   List TaskWait;

   struct   SoftIntList SoftInts[5];



   LONG  LastAlert[4];

   LONG  ExecBaseReserved[8];
};

#define SYSBASESIZE  sizeof(  struct   ExecBase)



#define  AFB_68010   0  
#define  AFB_68020   1
#define  AFB_68881   4
#define  AFB_PAL     8   PAL/NTSC
#define  AFB_50HZ    9   Clock Rate
#endif
