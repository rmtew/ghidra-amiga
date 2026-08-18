#ifndef	LIBRARIES_DOSEXTENS_H
#define	LIBRARIES_DOSEXTENS_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif
#ifndef	EXEC_TASKS_H
#include	"exec/tasks.h"
#endif
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif
#ifndef	EXEC_LIBRARIES_H
#include	"exec/libraries.h"
#endif
#ifndef	LIBRARIES_DOS_H
#include	"libraries/dos.h"
#endif
struct	Process	{
struct	Task	pr_Task;
struct	MsgPort	pr_MsgPort;
WORD	pr_Pad;
BPTR	pr_SegList;
LONG	pr_StackSize;
APTR	pr_GlobVec;
LONG	pr_TaskNum;
BPTR	pr_StackBase;
LONG	pr_Result2;
BPTR	pr_CurrentDir;
BPTR	pr_CIS;
BPTR	pr_COS;
APTR	pr_ConsoleTask;
APTR	pr_FileSystemTask;
BPTR	pr_CLI;
APTR	pr_ReturnAddr;
APTR	pr_PktWait;
APTR	pr_WindowPtr;
};
struct	FileHandle	{
struct	Message	*fh_Link;
struct	MsgPort	*fh_Port;
struct	MsgPort	*fh_Type;
LONG	fh_Buf;
LONG	fh_Pos;
LONG	fh_End;
LONG	fh_Funcs;
#define	fh_Func1	fh_Funcs
LONG	fh_Func2;
LONG	fh_Func3;
LONG	fh_Args;
#define	fh_Arg1	fh_Args
LONG	fh_Arg2;
};
struct	DosPacket	{
struct	Message	*dp_Link;
struct	MsgPort	*dp_Port;
LONG	dp_Type;
LONG	dp_Res1;
LONG	dp_Res2;
#define	dp_Action	dp_Type
#define	dp_Status	dp_Res1
#define	dp_Status2	dp_Res2
#define	dp_BufAddr	dp_Arg1
LONG	dp_Arg1;
LONG	dp_Arg2;
LONG	dp_Arg3;
LONG	dp_Arg4;
LONG	dp_Arg5;
LONG	dp_Arg6;
LONG	dp_Arg7;
};
struct	StandardPacket	{
struct	Message	sp_Msg;
struct	DosPacket	sp_Pkt;
};
#define	ACTION_NIL	0L
#define	ACTION_GET_BLOCK	2L
#define	ACTION_SET_MAP	4L
#define	ACTION_DIE	5L
#define	ACTION_EVENT	6L
#define	ACTION_CURRENT_VOLUME	7L
#define	ACTION_LOCATE_OBJECT	8L
#define	ACTION_RENAME_DISK	9L
#define	ACTION_WRITE	((long)'W')
#define	ACTION_READ	((long)'R')
#define	ACTION_FREE_LOCK	15L
#define	ACTION_DELETE_OBJECT	16L
#define	ACTION_RENAME_OBJECT	17L
#define	ACTION_MORE_CACHE	18L
#define	ACTION_COPY_DIR	19L
#define	ACTION_WAIT_CHAR	20L
#define	ACTION_SET_PROTECT	21L
#define	ACTION_CREATE_DIR	22L
#define	ACTION_EXAMINE_OBJECT	23L
#define	ACTION_EXAMINE_NEXT	24L
#define	ACTION_DISK_INFO	25L
#define	ACTION_INFO	26L
#define	ACTION_FLUSH	27L
#define	ACTION_SET_COMMENT	28L
#define	ACTION_PARENT	29L
#define	ACTION_TIMER	30L
#define	ACTION_INHIBIT	31L
#define	ACTION_DISK_TYPE	32L
#define	ACTION_DISK_CHANGE	33L
#define	ACTION_SET_DATE	34L
#define	ACTION_SCREEN_MODE	994L
#define	ACTION_READ_RETURN	1001L
#define	ACTION_WRITE_RETURN	1002L
#define	ACTION_SEEK	1008L
#define	ACTION_FINDUPDATE	1004L
#define	ACTION_FINDINPUT	1005L
#define	ACTION_FINDOUTPUT	1006L
#define	ACTION_END	1007L
#define	ACTION_TRUNCATE	1022L
#define	ACTION_WRITE_PROTECT	1023L
struct	DosLibrary	{
struct	Library	dl_lib;
APTR	dl_Root;
APTR	dl_GV;
LONG	dl_A2;
LONG	dl_A5;
LONG	dl_A6;
};
struct	RootNode	{
BPTR	rn_TaskArray;
BPTR	rn_ConsoleSegment;
struct	DateStamp	rn_Time;
LONG	rn_RestartSeg;
BPTR	rn_Info;
BPTR	rn_FileHandlerSegment;
};
struct	DosInfo	{
BPTR	di_McName;
BPTR	di_DevInfo;
BPTR	di_Devices;
BPTR	di_Handlers;
APTR	di_NetHand;
};
struct	CommandLineInterface	{
LONG	cli_Result2;
BSTR	cli_SetName;
BPTR	cli_CommandDir;
LONG	cli_ReturnCode;
BSTR	cli_CommandName;
LONG	cli_FailLevel;
BSTR	cli_Prompt;
BPTR	cli_StandardInput;
BPTR	cli_CurrentInput;
BSTR	cli_CommandFile;
LONG	cli_Interactive;
LONG	cli_Background;
BPTR	cli_CurrentOutput;
LONG	cli_DefaultStack;
BPTR	cli_StandardOutput;
BPTR	cli_Module;
};
struct	DeviceList	{
BPTR	dl_Next;
LONG	dl_Type;
struct	MsgPort	*	dl_Task;
BPTR	dl_Lock;
struct	DateStamp	dl_VolumeDate;
BPTR	dl_LockList;
LONG	dl_DiskType;
LONG	dl_unused;
BSTR	*	dl_Name;
};
struct	DevInfo	{
BPTR	dvi_Next;
LONG	dvi_Type;
APTR	dvi_Task;
BPTR	dvi_Lock;
BSTR	dvi_Handler;
LONG	dvi_StackSize;
LONG	dvi_Priority;
LONG	dvi_Startup;
BPTR	dvi_SegList;
BPTR	dvi_GlobVec;
BSTR	dvi_Name;
};
struct	DosList	{
BPTR	dol_Next;
LONG	dol_Type;
struct	MsgPort	*dol_Task;
BPTR	dol_Lock;
union	{
struct	{
BSTR	dol_Handler;
LONG	dol_StackSize;
LONG	dol_Priority;
ULONG	dol_Startup;
BPTR	dol_SegList;
BPTR	dol_GlobVec;
}	dol_handler;
struct	{
struct	DateStamp	dol_VolumeDate;
BPTR	dol_LockList;
LONG	dol_DiskType;
}	dol_volume;
}	dol_misc;
BSTR	dol_Name;
};
#define	DLT_DEVICE	0L
#define	DLT_DIRECTORY	1L
#define	DLT_VOLUME	2L
struct	FileLock	{
BPTR	fl_Link;
LONG	fl_Key;
LONG	fl_Access;
struct	MsgPort	*	fl_Task;
BPTR	fl_Volume;
};
#endif
