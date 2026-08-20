ÄàDOS_DOSEXTENS_HÄDOS_DOSEXTENS_HàEXEC_TASKS_Hå"exec/tasks.h"áà∏å"exec/ports.h"áàEXEC_LIBRARIES_Hå"exec/libraries.h"áàEXEC_SEMAPHORES_Hå"exec/semaphores.h"áàDEVICES_TIMER_Hå"devices/timer.h"áàDOS_DOS_Hå"dos/dos.h"á
ÉProcess{
ÉTask pr_Task;
É©pr_MsgPort;
òpr_Pad;
°pr_SegList;
ípr_StackSize;
îpr_GlobVec;
ípr_TaskNum;
°pr_StackBase;
ípr_Result2;
°pr_CurrentDir;
°pr_CIS;
°pr_COS;
îpr_ConsoleTask;
îpr_FileSystemTask;
°pr_CLI;
îpr_ReturnAddr;
îpr_PktWait;
îpr_WindowPtr;
°pr_HomeDir;
ípr_Flags;
ã(*pr_ExitCode)();
ípr_ExitData;
ä*pr_Arguments;
ÉMinList pr_LocalVars;
ópr_ShellPrivate;
°pr_CES;
};ÄPRB_FREESEGLIST 0ÄPRF_FREESEGLIST 1ÄPRB_FREECURRDIR 1ÄPRF_FREECURRDIR 2ÄPRB_FREECLI 2ÄPRF_FREECLI 4ÄPRB_CLOSEINPUT 3ÄPRF_CLOSEINPUT 8ÄPRB_CLOSEOUTPUT 4ÄPRF_CLOSEOUTPUT 16ÄPRB_FREEARGS 5ÄPRF_FREEARGS 32
ÉFileHandle{
ÉØ*fh_Link;
É©*fh_Port;
É©*fh_Type;
ífh_Buf;
ífh_Pos;
ífh_End;
ífh_Funcs;Äfh_Func1 fh_Funcs
ífh_Func2;
ífh_Func3;
ífh_Args;Äfh_Arg1 fh_Args
ífh_Arg2;
};
ÉDosPacket{
ÉØ*dp_Link;
É©*dp_Port;
ídp_Type;
ídp_Res1;
ídp_Res2;Ädp_Action dp_TypeÄdp_Status dp_Res1Ädp_Status2 dp_Res2Ädp_BufAddr dp_Arg1
ídp_Arg1;
ídp_Arg2;
ídp_Arg3;
ídp_Arg4;
ídp_Arg5;
ídp_Arg6;
ídp_Arg7;
};
ÉStandardPacket{
ÉØsp_Msg;
ÉDosPacket sp_Pkt;
};ÄACTION_NIL 0ÄACTION_STARTUP 0ÄACTION_GET_BLOCK 2ÄACTION_SET_MAP 4ÄACTION_DIE 5ÄACTION_EVENT 6ÄACTION_CURRENT_VOLUME 7ÄACTION_LOCATE_OBJECT 8ÄACTION_RENAME_DISK 9ÄACTION_WRITE 'W'ÄACTION_READ 'R'ÄACTION_FREE_LOCK 15ÄACTION_DELETE_OBJECT 16ÄACTION_RENAME_OBJECT 17ÄACTION_MORE_CACHE 18ÄACTION_COPY_DIR 19ÄACTION_WAIT_CHAR 20ÄACTION_SET_PROTECT 21ÄACTION_CREATE_DIR 22ÄACTION_EXAMINE_OBJECT 23ÄACTION_EXAMINE_NEXT 24ÄACTION_DISK_INFO 25ÄACTION_INFO 26ÄACTION_FLUSH 27ÄACTION_SET_COMMENT 28ÄACTION_PARENT 29ÄACTION_TIMER 30ÄACTION_INHIBIT 31ÄACTION_DISK_TYPE 32ÄACTION_DISK_CHANGE 33ÄACTION_SET_DATE 34ÄACTION_SCREEN_MODE 994ÄACTION_READ_RETURN 1001ÄACTION_WRITE_RETURN∂ÄACTION_SEEK 1008ÄACTION_FINDUPDATE 1004ÄACTION_FINDINPUT 1005ÄACTION_FINDOUTPUT 1006ÄACTION_END 1007ÄACTION_SET_FILE_SIZE 1022ÄACTION_WRITE_PROTECT 1023ÄACTION_SAME_LOCK 40ÄACTION_CHANGE_SIGNAL 995ÄACTION_FORMAT 1020ÄACTION_MAKE_LINK 1021ÄACTION_READ_LINK 1024ÄACTION_FH_FROM_LOCK 1026ÄACTION_IS_FILESYSTEM 1027ÄACTION_CHANGE_MODE 1028ÄACTION_COPY_DIR_FH 1030ÄACTION_PARENT_FH 1031ÄACTION_EXAMINE_ALL 1033ÄACTION_EXAMINE_FH 1034ÄACTION_LOCK_RECORD 2008ÄACTION_FREE_RECORD 2009ÄACTION_ADD_NOTIFY 4097ÄACTION_REMOVE_NOTIFY 4098ÄACTION_EXAMINE_ALL_END 1035ÄACTION_SET_OWNER 1036ÄACTION_SERIALIZE_DISK 4200
ÉErrorString{
í*estr_Nums;
ä*estr_Strings;
};
ÉDosLibrary{
ÉLibrary dl_lib;
ÉRootNode*dl_Root;
îdl_GV;
ídl_A2;
ídl_A5;
ídl_A6;
ÉErrorString*dl_Errors;
Étimerequest*dl_TimeReq;
ÉLibrary*dl_UtilityBase;
ÉLibrary*dl_IntuitionBase;
};
ÉRootNode{
°rn_TaskArray;
°rn_ConsoleSegment;
ÉDateStamp rn_Time;
írn_RestartSeg;
°rn_Info;
°rn_FileHandlerSegment;
ÉMinList rn_CliList;
É©*rn_BootProc;
°rn_ShellSegment;
írn_Flags;
};ÄRNB_WILDSTAR 24ÄRNF_WILDSTAR (1L<<24)ÄRNB_PRIVATE1 1ÄRNF_PRIVATE1 2
ÉCliProcList{
ÉMinNode cpl_Node;
ícpl_First;
É©**cpl_Array;
};
ÉDosInfo{
°di_McName;Ädi_ResList di_McName
°di_DevInfo;
°di_Devices;
°di_Handlers;
îdi_NetHand;
ÉSignalSemaphore di_DevLock;
ÉSignalSemaphore di_EntryLock;
ÉSignalSemaphore di_DeleteLock;
};
ÉSegment{
°seg_Next;
íseg_UC;
°seg_Seg;
äseg_Name[4];
};ÄCMD_SYSTEM -1ÄCMD_INTERNAL -2ÄCMD_DISABLED -999
ÉCommandLineInterface{
ícli_Result2;
BSTR cli_SetName;
°cli_CommandDir;
ícli_ReturnCode;
BSTR cli_CommandName;
ícli_FailLevel;
BSTR cli_Prompt;
°cli_StandardInput;
°cli_CurrentInput;
BSTR cli_CommandFile;
ícli_Interactive;
ícli_Background;
°cli_CurrentOutput;
ícli_DefaultStack;
°cli_StandardOutput;
°cli_Module;
};
ÉDeviceList{
°dl_Next;
ídl_Type;
É©*dl_Task;
°dl_Lock;
ÉDateStamp dl_VolumeDate;
°dl_LockList;
ídl_DiskType;
ídl_unused;
BSTR dl_Name;
};
ÉDevInfo{
°dvi_Next;
ídvi_Type;
îdvi_Task;
°dvi_Lock;
BSTR dvi_Handler;
ídvi_StackSize;
ídvi_Priority;
ídvi_Startup;
°dvi_SegList;
°dvi_GlobVec;
BSTR dvi_Name;
};
ÉDosList{
°dol_Next;
ídol_Type;
É©*dol_Task;
°dol_Lock;
´{
É{
BSTR dol_Handler;
ídol_StackSize;
ídol_Priority;
ódol_Startup;
°dol_SegList;
°dol_GlobVec;
}dol_handler;
É{
ÉDateStamp dol_VolumeDate;
°dol_LockList;
ídol_DiskType;
}dol_volume;
É{
ä*dol_AssignName;
ÉAssignList*dol_List;
}dol_assign;
}dol_misc;
BSTR dol_Name;
};
ÉAssignList{
ÉAssignList*al_Next;
°al_Lock;
};ÄDLT_DEVICE 0ÄDLT_DIRECTORY 1ÄDLT_VOLUME 2ÄDLT_LATE 3ÄDLT_NONBINDING 4ÄDLT_PRIVATE -1
ÉDevProc{
É©*dvp_Port;
°dvp_Lock;
ódvp_Flags;
ÉDosList*dvp_DevNode;
};ÄDVPB_UNLOCK 0ÄDVPF_UNLOCK (1L<<DVPB_UNLOCK)ÄDVPB_ASSIGN 1ÄDVPF_ASSIGN (1L<<DVPB_ASSIGN)ÄLDB_DEVICES 2ÄLDF_DEVICES (1L<<LDB_DEVICES)ÄLDB_VOLUMES 3ÄLDF_VOLUMES (1L<<LDB_VOLUMES)ÄLDB_ASSIGNS 4ÄLDF_ASSIGNS (1L<<LDB_ASSIGNS)ÄLDB_ENTRY 5ÄLDF_ENTRY (1L<<LDB_ENTRY)ÄLDB_DELETE 6ÄLDF_DELETE (1L<<LDB_DELETE)ÄLDB_READ 0ÄLDF_READ (1L<<LDB_READ)ÄLDB_WRITE 1ÄLDF_WRITE (1L<<LDB_WRITE)ÄLDF_ALL (LDF_DEVICES|LDF_VOLUMES|LDF_ASSIGNS)
ÉFileLock{
°fl_Link;
ífl_Key;
ífl_Access;
É©*fl_Task;
°fl_Volume;
};ÄREPORT_STREAM 0ÄREPORT_TASK 1ÄREPORT_LOCK 2ÄREPORT_VOLUME 3ÄREPORT_INSERT 4ÄABORT_DISK_ERROR 296ÄABORT_BUSY 288ÄRUN_EXECUTE -1ÄRUN_SYSTEM -2ÄRUN_SYSTEM_ASYNCH -3ÄST_ROOT 1ÄST_USERDIR 2ÄST_SOFTLINK 3ÄST_LINKDIR 4ÄST_FILE -3ÄST_LINKFILE -4ÄST_PIPEFILE -5á