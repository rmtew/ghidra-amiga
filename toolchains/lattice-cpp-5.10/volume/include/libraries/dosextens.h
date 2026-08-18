/* dosextens.h - AmigaDOS library function prototypes */

/*
**	Copyright (C) 1987, by MTS Associates, Jacksonville FL
**
**	Function prototypes for the exec.library of Amiga EXEC
**
**	Updated: 05/05/87, TFH
*/

/* DOS structures not needed for the casual DOS user */

// Caveat: examination of actual Amiga memory indicates that not all
// of these data areas are contain what these and similar data definitions
// state they do.  The data area definitions in this file are quite
// volatile, and Commodore-Amiga may alter their definitions radically
// at any time.  Use them with extreme caution!

#ifndef LIBRARIES_DOSEXTENS_H
#define LIBRARIES_DOSEXTENS_H 1

#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif
#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif
#ifndef EXEC_MEMORY_H
#include "exec/memory.h"
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

// Segment List.  All modules in memory are held together by chained SegLists

struct SegList {
	// length of segment is at offset -4 from SegList.
	BPTR sl_NextSeg;
	SegList *next() { return (SegList *) BADDR(sl_NextSeg); }
	// Code immediately follows
};

/* This is the extension to EXEC Messages used by DOS */

struct DosPacket {
   struct Message *dp_Link;	/* EXEC message              */
   struct MsgPort *dp_Port;     /* Reply port for the packet */
                                 /* Must be filled in each send. */
   LONG dp_Type;                 /* See ACTION_... below and 
                                  * 'R' means Read, 'W' means Write to the
                                  * file system */
   LONG dp_Res1;                 /* For file system calls this is the result
                                  * that would have been returned by the
                                  * function, e.g. Write ('W') returns actual
                                  * length written */
   LONG dp_Res2;                 /* For file system calls this is what would
                                  * have been returned by IoErr() */
/*  Device packets common equivalents */
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

	void send (MsgPort *mp, MsgPort *answer)
	{
		dp_Link->mn_ReplyPort = dp_Port = answer;
		dp_Link->send(mp);
	}

	void sendio(MsgPort *mp, MsgPort *answer, LONG opcode,
		LONG arg1=0, LONG arg2=0, LONG arg3=0)
	{
		dp_Action = opcode;
		dp_Arg1 = arg1; dp_Arg2 = arg2; dp_Arg3 = arg3;
		send (mp, answer);
	}
	void reply(LONG code, LONG stat=0)
	{ dp_Res1 = code; dp_Res2 = stat; PutMsg(dp_Port, dp_Link); }

	DosPacket * get(MsgPort *mp)
		{ return (DosPacket *) GetMsg(mp); }

	friend DosPacket *PacketAddress (Message *m)
	{ return (DosPacket *)m->ln_Name; }
}; /* DosPacket */

struct StandardPacket : public Message {
   struct DosPacket sp_Pkt;

StandardPacket() {
		ln_Name = (char *) &sp_Pkt;
		ln_Type = NT_MESSAGE;
		ln_Pri = 0;
		mn_ReplyPort = sp_Pkt.dp_Port = NULL;
		mn_Length = sizeof (struct DosPacket); 
		sp_Pkt.dp_Link = this;
	}

	void send (MsgPort *mp, MsgPort *answer)
	{sp_Pkt.send(mp, answer);}

	void sendio(MsgPort *mp, MsgPort *answer, LONG opcode,
		LONG arg1=0, LONG arg2=0, LONG arg3=0)
	{sp_Pkt.sendio(mp, answer, opcode, arg1, arg2, arg3);}
}; /* StandardPacket */


/* Packet types */
#define ACTION_NIL              0
#define ACTION_GET_BLOCK        2
#define ACTION_SET_MAP          4
#define ACTION_DIE              5
#define ACTION_EVENT		6
#define ACTION_CURRENT_VOLUME   7
#define ACTION_LOCATE_OBJECT    8
#define ACTION_RENAME_DISK      9
#define ACTION_WRITE            'W'
#define ACTION_READ             'R'
#define ACTION_FREE_LOCK        15
#define ACTION_DELETE_OBJECT    16
#define ACTION_RENAME_OBJECT    17
#define ACTION_MORE_CACHE	18
#define ACTION_COPY_DIR         19
#define ACTION_WAIT_CHAR        20
#define ACTION_SET_PROTECT      21
#define ACTION_CREATE_DIR       22
#define ACTION_EXAMINE_OBJECT   23
#define ACTION_EXAMINE_NEXT     24
#define ACTION_DISK_INFO        25
#define ACTION_INFO       	26
#define ACTION_FLUSH		27

#define ACTION_SET_COMMENT      28  
#define ACTION_PARENT           29
#define ACTION_TIMER            30
#define ACTION_INHIBIT          31
#define ACTION_DISK_TYPE        32
#define ACTION_DISK_CHANGE      33
#define ACTION_SET_DATE		34
#define ACTION_SCREEN_MODE	994
#define ACTION_END		1007 /* close file */

/* The long word address (BPTR) of this structure is returned by
 * Open() and other routines that return a file.  You need only worry
 * about this struct to do async io's via PutMsg() instead of
 * standard file system calls */

struct FileHandleInfo {
   struct Msg     *fh_Link;	/* EXEC message			*/
   struct MsgPort *fh_Port;	/* Reply port for the packet	*/
   struct MsgPort *fh_Type;	/* Port to do PutMsg() to
				 * Address is negative if a plain file */
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

	FileHandleInfo(struct MsgPort * iop)
	{ this = (FileHandleInfo *)
		AllocMem (sizeof (FileHandleInfo), MEMF_PUBLIC | MEMF_CLEAR);
	  if (this != NULL) { fh_Type = iop; fh_Pos = fh_End = -1L;}
	}
	~FileHandleInfo()
	{ FreeMem ((APTR) this, sizeof (FileHandleInfo)); this = NULL; }
	void send(Message *pkt) { pkt->send(fh_Type); }
	void send(StandardPacket *pkt, MsgPort *rport)
	 { pkt->send(fh_Type, rport); }
}; /* FileHandle */

//class LockHandle {
//	BPTR lock;
//	operator FLock() { return (FLock) BADDR (lock); }
//};


/* All DOS processes have this structure */

struct Process : public Task {
    struct  MsgPort pr_MsgPort; // Addresss of this is process ID -
				// The documentation LIES!
    WORD    pr_Pad;		/* Remaining variables on 4 byte boundaries */
    BPTR    pr_SegList;		/* Array of seg lists used by this process  */
    SegList *first_segment() { return (SegList *) BADDR(pr_SegList); }
    LONG    pr_StackSize;	/* Size of process stack in bytes           */
    APTR    pr_GlobVec;		/* Global vector for this process (BCPL)    */
    LONG    pr_TaskNum;		/* CLI task number of zero if not a CLI     */
    BPTR    pr_StackBase;	/* Ptr to high memory end of process stack  */
    APTR StackBase() { return BADDR(pr_StackBase); }
    LONG    pr_Result2;		/* Value of secondary result from last call */
    FLock	pr_CurrentDir;	/* Lock associated with current directory   */
    FileHandle	pr_CIS;		/* Current CLI Input Stream	*/
    FileHandle stdin_fh() { return pr_CIS; }
    FileHandle	pr_COS;		/* Current CLI Output Stream	*/
    FileHandle stdout_fh() { return pr_COS; }
    APTR    pr_ConsoleTask;	/* Console handler process for current window*/
    APTR    pr_FileSystemTask;	/* File handler process for current drive   */
    BPTR    pr_CLI;		/* pointer to ConsoleLineInterpreter        */
    APTR    CLI() { return BADDR(pr_CLI); }
    APTR    pr_ReturnAddr;	/* pointer to previous stack frame          */
    APTR    pr_PktWait;		/* Function to be called when awaiting msg  */
    APTR    pr_WindowPtr;	/* Window for error printing */

    friend MsgPort *DeviceProc(char *deviceID);

    friend Process *ThisProcess()
	{ return (Process *)(((Task *) DeviceProc(NULL))-1);}

    MsgPort *process_id() { return &pr_MsgPort; }

    DosPacket *getpacket()
	{ Message *dp;
	  dp = GetMsg(&pr_MsgPort);
	  return (dp != NULL) ? (DosPacket *) (dp->ln_Name) : NULL;
	}
};  /* Process */

struct CLIArray {
	LONG	CLI_Max;	// Maximum number of CLI's
	Process *CLI[1];	// ..2, ..3, etc
};

struct DosInfo;

struct RootNode {
    BPTR    rn_TaskArray;            /* [0] is max number of CLI's
                                      * [1] is APTR to process id of CLI 1
                                      * [n] is APTR to process id of CLI n */
    CLIArray *CLIs() { return (CLIArray *) BADDR(rn_TaskArray); }
    BPTR    rn_ConsoleSegment; /* SegList for the CLI                      */ 
    SegList *ConsoleSegment() { return (SegList *) BADDR(rn_ConsoleSegment); }
    struct  DateStamp rn_Time; /* Current time                             */
    LONG    rn_RestartSeg;     /* SegList for the disk validator process   */
    BPTR    rn_Info;           /* Pointer ot the Info structure            */
    DosInfo  *DosInfo() {return (struct DosInfo *) BADDR(rn_Info); }
};  /* RootNode */

/* DOS library node structure.
 * This is the data at positive offsets from the library node.
 * Negative offsets from the node is the jump table to DOS functions  
 * node = (struct DosLibrary *) OpenLibrary( "dos.library" .. )      */

struct DosLibrary :  Library {
    RootNode *dl_Root;		/* Pointer to RootNode, described below */
    APTR    dl_GV;		/* Pointer to BCPL global vector        */
    LONG    dl_A2;		/* Private register dump of DOS         */
    LONG    dl_A5;
    LONG    dl_A6;
};  /*  DosLibrary */

class DeviceList;

struct DosInfo {
    BPTR    di_McName;         /* Network name of this machine; currently 0 */
    BPTR    di_DevInfo;        /* Device List                               */
    DeviceList *DevInfo() { return (DeviceList *) BADDR(di_DevInfo); }
    BPTR    di_Devices;        /* Currently zero                            */
    BPTR    di_Handlers;       /* Currently zero                            */
    APTR    di_NetHand;        /* Network handler processid; currently zero */
};  /* DosInfo */

/* DOS Processes started from the CLI via RUN or NEWCLI have this additional
 * set to data associated with them */
// NOTE - a little kludgy here, because otherwise names expand to > 31 chars.

#define CommandLineInterface xyz__CLI
struct CommandLineInterface {
    LONG   cli_Result2;        /* Value of IoErr from last command        */  
    BSTR   cli_SetName;        /* Name of current directory               */
    FLock cli_CommandDir;     /* Lock associated with command directory  */
    LONG   cli_ReturnCode;     /* Return code from last command           */
    BSTR   cli_CommandName;    /* Name of current command                 */
    LONG   cli_FailLevel;      /* Fail level (set by FAILAT)              */
    BSTR   cli_Prompt;         /* Current prompt (set by PROMPT)          */
    FLock  cli_StandardInput;  /* Default (terminal) CLI input            */
    FLock  cli_CurrentInput;   /* Current CLI input                       */
    BSTR   cli_CommandFile;    /* Name of EXECUTE command file            */
    LONG   cli_Interactive;    /* Boolean; True if prompts required       */
    LONG   cli_Background;     /* Boolean; True if CLI created by RUN     */
    FLock cli_CurrentOutput;  /* Current CLI output                      */
    LONG   cli_DefaultStack;   /* Stack size to be obtained in long words */
    FLock cli_StandardOutput; /* Default (terminal) CLI output           */
    BPTR   cli_Module;         /* SegList of currently loaded command     */
    SegList *Module() { return (SegList *) BADDR(cli_Module); }
};  /* CommandLineInterface */

// Devices, directories, and ASSIGN'ed names are defined here:

/* definitions for dl_Type */

enum dlt_Type {
	DLT_DEVICE =	0,
	DLT_DIRECTORY =	1,
	DLT_VOLUME =	2
};

struct DeviceDescriptor {
	BSTR Handler;	// file name of handler or NULL
	LONG StackSize;	// handler's stack size
	LONG HandlerPriority;
	LONG HandlerStartup; // parameter
	BPTR SegList;	// Handler's SegList or NULL
	BPTR GlobVect;	// for handler
};

struct DeviceDir {
	DateStamp	dl_VolumeDate;	/* creation date */
	FLock	dl_LockList;	/* outstanding locks */
	LONG		dl_DiskType;	/* 'DOS', etc */
	LONG		dl_unused;
};

struct DeviceList {
    BPTR		dl_Next;	/* bptr to next device list */
    DeviceList *next() { return (DeviceList *) BADDR(dl_Next); }
    LONG		dl_Type;	/* see DLT below */
    MsgPort *		dl_Task;	/* ptr to handler task */
    FLock		dl_Lock;	/* not for volumes */
    union {
	DeviceDescriptor dd;
#define dl_Handler dd.Handler
#define dl_StackSize dd.StackSize
#define dl_HandlerPriority dd.HandlerPriority
#define dl_HandlerStartup dd.HandlerStartup
#define dl_SegList dd.SegList
#define dl_GlobVect  dd.GlobVect
	struct DeviceDir dvd;
    };
    BSTR 	dl_Name;	// bptr to bcpl (device or "assign"ed)
};

#endif
