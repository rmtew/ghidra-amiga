/* dos.h - AmigaDOS library function prototypes */

/*
**	Copyright (C) 1987, by MTS Associates, Jacksonville FL
**	All rights reserved.  Duplication of this data by other than
**	licensed users is a violation of copyright law.
**
**	Function prototypes for the exec.library of Amiga EXEC
**
**	Updated: 02/26/87, TFH
*/

#ifndef LIBRARIES_DOS_H

#define LIBRARIES_DOS_H

#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif

#ifndef EXEC_PORTS_H
#include "exec/ports.h"
#endif

const char DOSNAME[] =  "dos.library";

/* Predefined Amiga DOS global constants */

// All BCPL data must be long word aligned.  BCPL pointers are the long word
// address (i.e byte address divided by 4 (>>2))
// Use of the conversion operators is recommended whereever possible

typedef long  BPTR;                 /* Long word pointer */
typedef long  BSTR;                 /* Long word pointer to BCPL string  */

 // Convert BPTR to typical C pointer */
inline APTR BADDR( BPTR bptr ) {return (APTR (bptr << 2));}


/* BCPL strings have a length in the first byte and then the characters.
 * For example:  s[0]=3 s[1]=S s[2]=Y s[3]=S                             */

// A lock structure, as returned by Lock() or DupLock():

struct FileInfoBlock;
struct DeviceList;

typedef BPTR FLock;

struct FileLock {
    FLock		fl_Link;	/* bcpl pointer to next lock */
    FileLock *next() { return (FileLock *) BADDR(fl_Link); }
    LONG		fl_Key;		/* disk block number */
    LONG		fl_Access;	/* exclusive or shared */
    class MsgPort 	*fl_Task;	/* handler task's port */
    BPTR		fl_Volume;	/* bptr to a DeviceList */
    operator DeviceList *() { return (DeviceList *) BADDR(fl_Volume); }

   friend FLock CurrentDir(FLock);
   friend FLock CreateDir(const char * dirname);
   friend FLock DupLock(FLock);

   friend BOOL Examine(FLock, FileInfoBlock *fb);
   friend BOOL ExNext(FLock, FileInfoBlock *fb);

   friend FLock Lock(const char *name, int accessmode);
   friend FLock ParentDir(FLock);
   friend void UnLock(FLock);
   friend void Unlock(FLock fl) { UnLock (fl); } // #$%#%$!!!!
};

typedef BPTR FileHandle;

FileHandle Open (const char *filename, const int mode);
void Close (FileHandle);

extern FileHandle Input();
extern FileHandle Output();
extern BOOL IsInteractive(FileHandle file);
extern int Read(FileHandle f, const void * buf, const unsigned len);
extern int Write(FileHandle f, const void * buf, const unsigned len);
extern BOOL Rename(const char *oldname, const char* newname);
extern ULONG Seek(FileHandle fh, ULONG p, const int m);
extern BOOL SetComment (const char * filename, const char *comment);
extern BOOL SetProtection (const char *filename, int mask);
extern BOOL WaitForChar (FileHandle fh, int microseconds);
extern BOOL Execute (const char *s,
	const FileHandle in, const FileHandle out);

/* Mode parameter to Open() */
#define MODE_READWRITE	     1004   /* Old file with exclusive lock	*/
#define MODE_OLDFILE         1005   /* Old file with shared lock	*/
				    /* positioned at begining of file. */
#define MODE_READONLY	     MODE_OLDFILE
#define MODE_NEWFILE         1006   /* Open freshly created file (delete */ 
				    /* old file) read/write */

/* Relative position to Seek() */
#define OFFSET_BEGINING     -1      /* relative to Begining Of File */
#define OFFSET_CURRENT       0      /* relative to Current file position */
#define OFFSET_END           1      /* relative to End Of File    */

#define BITSPERBYTE          8
#define BYTESPERLONG         4
#define BITSPERLONG          32
#define MAXINT               0x7FFFFFFF
#define MININT               0x80000000

/* Passed as type to FileLock() */
#define SHARED_LOCK          -2     /* File is readable by others */
#define ACCESS_READ          -2     /* Synonym */
#define EXCLUSIVE_LOCK       -1     /* No other access allowed    */
#define ACCESS_WRITE         -1     /* Synonym */

struct DateStamp {
   LONG  ds_Days;             /* Number of days since Jan. 1, 1978 */
   LONG  ds_Minutes;          /* Number of minutes past midnight */
   LONG  ds_Ticks;            /* Number of ticks past minute */

   friend DateStamp(DateStamp *when);
}; /* DateStamp */
#define TICKS_PER_SECOND      50   /* Number of ticks in one second */

extern void Delay(int ticks);

/* Returned by Examine() and ExInfo(), must be on a 4 byte boundary */
struct FileInfoBlock {
   LONG   fib_DiskKey;
   LONG   fib_DirEntryType;
   BOOL   is_file() { return fib_DirEntryType < 0; }
   BOOL   is_directory() { return fib_DirEntryType > 0; }

   char   fib_FileName[108]; /* Null terminated. Max 30 chars used for now */
   LONG   fib_Protection;    /* bit mask of protection, rwxd are 3-0.      */
   LONG   fib_EntryType;
   LONG   fib_Size;          /* Number of bytes in file */
   LONG   fib_NumBlocks;     /* Number of Blocks in file */
   struct DateStamp fib_Date;/* Date file last changed */
   char   fib_Comment[116];  /* Null terminated. 
                              * Comment associated with file */


/* FIB stands for FileInfoBlock */
/* FIBB are bit definitions, FIBF are field definitions */
#define FIBB_READ      3
#define FIBB_WRITE     2
#define FIBB_EXECUTE   1
#define FIBB_DELETE    0
#define FIBB_MODIFIED  4	/* Archive bit (added, MTS)*/
#define FIBF_READ      (1<<FIBB_READ)
#define FIBF_WRITE     (1<<FIBB_WRITE)
#define FIBF_EXECUTE   (1<<FIBB_EXECUTE)
#define FIBF_DELETE    (1<<FIBB_DELETE)
#define FIBF_MODIFIED  (1<<FIBB_MODIFIED)

   friend BOOL Examine(FLock, FileInfoBlock *l);
   friend BOOL ExNext(FLock, FileInfoBlock *l);
   friend BOOL can_execute (FileInfoBlock &fi)
	{return (fi.fib_Protection & FIBF_EXECUTE);}
   friend BOOL can_read (FileInfoBlock &fi)
	{return (fi.fib_Protection & FIBF_READ);}
   friend BOOL can_write (FileInfoBlock &fi)
   	{return (fi.fib_Protection & FIBF_WRITE);}
   friend BOOL can_delete(FileInfoBlock &fi)
	 {return (fi.fib_Protection & FIBF_DELETE);}
}; /* FileInfoBlock */


/* returned by Info(), must be on a 4 byte boundary */
struct InfoData { 
   LONG   id_NumSoftErrors;	// number of soft errors on disk
   LONG   id_UnitNumber;	// Which unit disk is (was) mounted on
   LONG   id_DiskState;		// See defines below
   LONG   id_NumBlocks;		// Number of blocks on disk
   LONG   id_NumBlocksUsed;	// Number of block in use
   LONG   id_BytesPerBLock;   
   LONG   id_DiskType;		// Disk Type code
   BSTR   id_VolumeNode;	// BSTR volume name
   LONG   id_InUse;		// Flag, zero if not in use

   friend BOOL Info(FLock, InfoData *d);
}; /* InfoData */

/* ID stands for InfoData */
        /* Disk states */
#define ID_WRITE_PROTECTED 80    /* Disk is write protected */
#define ID_VALIDATING      81    /* Disk is currently being validated */
#define ID_VALIDATED       82    /* Disk is consistent and writeable */

        /* Disk types */
#define ID_NO_DISK_PRESENT	(-1)
#define ID_UNREADABLE_DISK	(('B'<<24) | ('A'<<16) | ('D'<<8))
#define ID_DOS_DISK		(('D'<<24) | ('O'<<16) | ('S'<<8))
#define ID_NOT_REALLY_DOS	(('N'<<24) | ('D'<<16) | ('O'<<8) | ('S))
#define ID_KICKSTART_DISK	(('K'<<24) | ('I'<<16) | ('C'<<8) | ('K'))

/* Errors from IoErr(), etc. */
#define ERROR_NO_FREE_STORE               103
#define ERROR_NO_DEFAULT_DIR              201
#define ERROR_OBJECT_IN_USE               202
#define ERROR_OBJECT_EXISTS               203
#define ERROR_DIR_NOT_FOUND               204
#define ERROR_OBJECT_NOT_FOUND            205
#define ERROR_BAD_STREAM_NAME             206
#define ERROR_OBJECT_TOO_LARGE            207
#define ERROR_ACTION_NOT_KNOWN            209
#define ERROR_INVALID_COMPONENT_NAME      210
#define ERROR_INVALID_LOCK                211
#define ERROR_OBJECT_WRONG_TYPE           212
#define ERROR_DISK_NOT_VALIDATED          213
#define ERROR_DISK_WRITE_PROTECTED        214
#define ERROR_RENAME_ACROSS_DEVICES       215
#define ERROR_DIRECTORY_NOT_EMPTY         216
#define ERROR_TOO_MANY_LEVELS             217
#define ERROR_DEVICE_NOT_MOUNTED          218
#define ERROR_SEEK_ERROR                  219
#define ERROR_COMMENT_TOO_BIG             220   
#define ERROR_DISK_FULL                   221
#define ERROR_DELETE_PROTECTED            222
#define ERROR_WRITE_PROTECTED             223 
#define ERROR_READ_PROTECTED              224
#define ERROR_NOT_A_DOS_DISK              225
#define ERROR_NO_DISK                     226
#define ERROR_NO_MORE_ENTRIES             232

/* These are the return codes used by convention by AmigaDOS commands */
/* See FAILAT and IF for relvance to EXECUTE files                    */
#define RETURN_OK                           0  /* No problems, success */
#define RETURN_WARN                         5  /* A warning only */
#define RETURN_ERROR                       10  /* Something wrong */
#define RETURN_FAIL                        20  /* Complete or severe failure*/

/* Bit numbers that signal you that a user has issued a break */
#define SIGBREAKB_CTRL_C   12
#define SIGBREAKB_CTRL_D   13
#define SIGBREAKB_CTRL_E   14
#define SIGBREAKB_CTRL_F   15

/* Bit fields that signal you that a user has issued a break */
/* for example:  if (SetSignal(0,0) & BREAK_CTRL_CF) cleanup_and_exit(); */
#define SIGBREAKF_CTRL_C  (1<<SIGBREAKB_CTRL_C)
#define SIGBREAKF_CTRL_D  (1<<SIGBREAKB_CTRL_D)
#define SIGBREAKF_CTRL_E  (1<<SIGBREAKB_CTRL_E)
#define SIGBREAKF_CTRL_F  (1<<SIGBREAKB_CTRL_F)

extern int IoErr(void);
extern BOOL DeleteFile(const char *n);

#endif LIBRARIES_DOS_H
