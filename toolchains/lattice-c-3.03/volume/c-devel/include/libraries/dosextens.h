
#ifndef LIBRARIES_DOSEXTENS_H
#define LIBRARIES_DOSEXTENS_H 1






#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif
#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif
#ifndef EXEC_PORTS_H
#include "exec/ports.h"
#endif
#ifndef EXEC_LIBRARIES_H
#include "exec/libraries.h"
#endif

#ifndef LIBRARIES_DOS_H
#include "libraries/dos.h"
#endif





struct Process {
    struct  Task    pr_Task;         
    struct  MsgPort pr_MsgPort; 
    WORD    pr_Pad;             
    BPTR    pr_SegList;         
    LONG    pr_StackSize;       
    APTR    pr_GlobVec;         
    LONG    pr_TaskNum;         
    BPTR    pr_StackBase;       
    LONG    pr_Result2;         
    BPTR    pr_CurrentDir;      
    BPTR    pr_CIS;             
    BPTR    pr_COS;             
    APTR    pr_ConsoleTask;     
    APTR    pr_FileSystemTask;  
    BPTR    pr_CLI;             
    APTR    pr_ReturnAddr;      
    APTR    pr_PktWait;         
    APTR    pr_WindowPtr;       
};  



struct FileHandle {
   struct Message *fh_Link;         
   struct MsgPort *fh_Port;      
   struct MsgPort *fh_Type;      
   LONG fh_Buf;
   LONG fh_Pos;
   LONG fh_End;
   LONG fh_Funcs;
#define fh_Func1 fh_Funcs
   LONG fh_Func2;
   LONG fh_Func3;
   LONG fh_Args;
#define fh_Arg1 fh_Args
   LONG fh_Arg2;
}; 



struct DosPacket {
   struct Message *dp_Link;      
   struct MsgPort *dp_Port;      
                                 
   LONG dp_Type;                 
   LONG dp_Res1;                 
   LONG dp_Res2;                 

#define dp_Action  dp_Type
#define dp_Status  dp_Res1
#define dp_Status2 dp_Res2
#define dp_BufAddr dp_Arg1
   LONG dp_Arg1;                    
   LONG dp_Arg2;
   LONG dp_Arg3;
   LONG dp_Arg4;
   LONG dp_Arg5;
   LONG dp_Arg6;
   LONG dp_Arg7;
}; 



struct StandardPacket {
   struct Message   sp_Msg;
   struct DosPacket sp_Pkt;
}; 


#define ACTION_NIL              0
#define ACTION_GET_BLOCK        2
#define ACTION_SET_MAP          4
#define ACTION_DIE              5
#define ACTION_EVENT            6
#define ACTION_CURRENT_VOLUME   7
#define ACTION_LOCATE_OBJECT    8
#define ACTION_RENAME_DISK      9
#define ACTION_WRITE            'W'
#define ACTION_READ             'R'
#define ACTION_FREE_LOCK        15
#define ACTION_DELETE_OBJECT    16
#define ACTION_RENAME_OBJECT    17

#define ACTION_COPY_DIR         19
#define ACTION_WAIT_CHAR        20
#define ACTION_SET_PROTECT      21
#define ACTION_CREATE_DIR       22
#define ACTION_EXAMINE_OBJECT   23
#define ACTION_EXAMINE_NEXT     24
#define ACTION_DISK_INFO        25
#define ACTION_INFO             26

#define ACTION_SET_COMMENT      28  
#define ACTION_PARENT           29
#define ACTION_TIMER            30
#define ACTION_INHIBIT          31
#define ACTION_DISK_TYPE        32
#define ACTION_DISK_CHANGE      33



struct DosLibrary {
    struct  Library dl_lib;
    APTR    dl_Root;          
    APTR    dl_GV;            
    LONG    dl_A2;            
    LONG    dl_A5;
    LONG    dl_A6;
};  



struct RootNode {
    BPTR    rn_TaskArray;            
    BPTR    rn_ConsoleSegment;  
    struct  DateStamp rn_Time; 
    LONG    rn_RestartSeg;     
    BPTR    rn_Info;           
};  

struct DosInfo {
    BPTR    di_McName;         
    BPTR    di_DevInfo;        
    BPTR    di_Devices;        
    BPTR    di_Handlers;       
    APTR    di_NetHand;        
};  



struct CommandLineInterface {
    LONG   cli_Result2;          
    BSTR   cli_SetName;        
    BPTR   cli_CommandDir;     
    LONG   cli_ReturnCode;     
    BSTR   cli_CommandName;    
    LONG   cli_FailLevel;      
    BSTR   cli_Prompt;         
    BPTR   cli_StandardInput;  
    BPTR   cli_CurrentInput;   
    BSTR   cli_CommandFile;    
    LONG   cli_Interactive;    
    LONG   cli_Background;     
    BPTR   cli_CurrentOutput;  
    LONG   cli_DefaultStack;   
    BPTR   cli_StandardOutput; 
    BPTR   cli_Module;         
};  


struct DeviceList {
    BPTR    dl_Next;                
    LONG    dl_Type;                
    struct MsgPort * dl_Task;       
    BPTR    dl_Lock;                
    struct DateStamp dl_VolumeDate; 
    BPTR    dl_LockList;            
    LONG    dl_DiskType;            
    LONG    dl_unused;
    BSTR *  dl_Name;                
};


#define DLT_DEVICE   0
#define DLT_DIRECTORY   1
#define DLT_VOLUME   2



struct FileLock {
    BPTR    fl_Link;                 
    LONG    fl_Key;                  
    LONG    fl_Access;               
    struct MsgPort * fl_Task;        
    BPTR    fl_Volume;               
};

#endif LIBRARIES_DOSEXTENS_H
