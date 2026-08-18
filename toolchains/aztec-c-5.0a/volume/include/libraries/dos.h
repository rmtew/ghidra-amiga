#ifndef	LIBRARIES_DOS_H
#define	LIBRARIES_DOS_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif
#define	DOSNAME	"dos.library"
#define	DOSTRUE	(-1L)
#define	DOSFALSE	(0L)
#define	MODE_OLDFILE	1005
#define	MODE_NEWFILE	1006
#define	MODE_READWRITE	1004
#define	OFFSET_BEGINNING	-1
#define	OFFSET_CURRENT	0
#define	OFFSET_END	1
#define	OFFSET_BEGINING	OFFSET_BEGINNING
#define	BITSPERBYTE	8
#define	BYTESPERLONG	4
#define	BITSPERLONG	32
#define	MAXINT	0x7FFFFFFF
#define	MININT	0x80000000
#define	SHARED_LOCK	-2
#define	ACCESS_READ	-2
#define	EXCLUSIVE_LOCK	-1
#define	ACCESS_WRITE	-1
struct	DateStamp	{
LONG	ds_Days;
LONG	ds_Minute;
LONG	ds_Tick;
};
#define	TICKS_PER_SECOND	50
struct	FileInfoBlock	{
LONG	fib_DiskKey;
LONG	fib_DirEntryType;
char	fib_FileName[108];
LONG	fib_Protection;
LONG	fib_EntryType;
LONG	fib_Size;
LONG	fib_NumBlocks;
struct	DateStamp	fib_Date;
char	fib_Comment[80];
char	fib_Reserved[36];
};
#define	FIBB_SCRIPT	6
#define	FIBB_PURE	5
#define	FIBB_ARCHIVE	4
#define	FIBB_READ	3
#define	FIBB_WRITE	2
#define	FIBB_EXECUTE	1
#define	FIBB_DELETE	0
#define	FIBF_SCRIPT	(1<<FIBB_SCRIPT)
#define	FIBF_PURE	(1<<FIBB_PURE)
#define	FIBF_ARCHIVE	(1<<FIBB_ARCHIVE)
#define	FIBF_READ	(1<<FIBB_READ)
#define	FIBF_WRITE	(1<<FIBB_WRITE)
#define	FIBF_EXECUTE	(1<<FIBB_EXECUTE)
#define	FIBF_DELETE	(1<<FIBB_DELETE)
typedef	long	BPTR;
typedef	long	BSTR;
#ifdef	OBSOLETE_LIBRARIES_DOS_H
#define	BADDR(	bptr	)	(((ULONG)bptr)	<<	2)
#else
#define	BADDR(x)	((APTR)((ULONG)x	<<	2))
#endif
struct	InfoData	{
LONG	id_NumSoftErrors;
LONG	id_UnitNumber;
LONG	id_DiskState;
LONG	id_NumBlocks;
LONG	id_NumBlocksUsed;
LONG	id_BytesPerBlock;
LONG	id_DiskType;
BPTR	id_VolumeNode;
LONG	id_InUse;
};
#define	ID_WRITE_PROTECTED	80
#define	ID_VALIDATING	81
#define	ID_VALIDATED	82
#define	ID_NO_DISK_PRESENT	(-1)
#define	ID_UNREADABLE_DISK	(('B'<<24)	|	('A'<<16)	|	('D'<<8))
#define	ID_DOS_DISK	(('D'<<24)	|	('O'<<16)	|	('S'<<8))
#define	ID_NOT_REALLY_DOS	(('N'<<24)	|	('D'<<16)	|	('O'<<8)	|	('S'))
#define	ID_KICKSTART_DISK	(('K'<<24)	|	('I'<<16)	|	('C'<<8)	|	('K'))
#define	ERROR_NO_FREE_STORE	103
#define	ERROR_TASK_TABLE_FULL	105
#define	ERROR_LINE_TOO_LONG	120
#define	ERROR_FILE_NOT_OBJECT	121
#define	ERROR_INVALID_RESIDENT_LIBRARY	122
#define	ERROR_NO_DEFAULT_DIR	201
#define	ERROR_OBJECT_IN_USE	202
#define	ERROR_OBJECT_EXISTS	203
#define	ERROR_DIR_NOT_FOUND	204
#define	ERROR_OBJECT_NOT_FOUND	205
#define	ERROR_BAD_STREAM_NAME	206
#define	ERROR_OBJECT_TOO_LARGE	207
#define	ERROR_ACTION_NOT_KNOWN	209
#define	ERROR_INVALID_COMPONENT_NAME	210
#define	ERROR_INVALID_LOCK	211
#define	ERROR_OBJECT_WRONG_TYPE	212
#define	ERROR_DISK_NOT_VALIDATED	213
#define	ERROR_DISK_WRITE_PROTECTED	214
#define	ERROR_RENAME_ACROSS_DEVICES	215
#define	ERROR_DIRECTORY_NOT_EMPTY	216
#define	ERROR_TOO_MANY_LEVELS	217
#define	ERROR_DEVICE_NOT_MOUNTED	218
#define	ERROR_SEEK_ERROR	219
#define	ERROR_COMMENT_TOO_BIG	220
#define	ERROR_DISK_FULL	221
#define	ERROR_DELETE_PROTECTED	222
#define	ERROR_WRITE_PROTECTED	223
#define	ERROR_READ_PROTECTED	224
#define	ERROR_NOT_A_DOS_DISK	225
#define	ERROR_NO_DISK	226
#define	ERROR_NO_MORE_ENTRIES	232
#define	RETURN_OK	0
#define	RETURN_WARN	5
#define	RETURN_ERROR	10
#define	RETURN_FAIL	20
#define	SIGBREAKB_CTRL_C	12
#define	SIGBREAKB_CTRL_D	13
#define	SIGBREAKB_CTRL_E	14
#define	SIGBREAKB_CTRL_F	15
#define	SIGBREAKF_CTRL_C	(1<<SIGBREAKB_CTRL_C)
#define	SIGBREAKF_CTRL_D	(1<<SIGBREAKB_CTRL_D)
#define	SIGBREAKF_CTRL_E	(1<<SIGBREAKB_CTRL_E)
#define	SIGBREAKF_CTRL_F	(1<<SIGBREAKB_CTRL_F)
#endif
