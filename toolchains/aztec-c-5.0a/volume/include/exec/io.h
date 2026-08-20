#ifndef	EXEC_IO_H
#define	EXEC_IO_H
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif	/* !EXEC_PORTS_H */
struct	IORequest	{
struct	Message	io_Message;
struct	Device	*io_Device;
struct	Unit	*io_Unit;
UWORD	io_Command;
UBYTE	io_Flags;
BYTE	io_Error;
};
struct	IOStdReq	{
struct	Message	io_Message;
struct	Device	*io_Device;
struct	Unit	*io_Unit;
UWORD	io_Command;
UBYTE	io_Flags;
BYTE	io_Error;
ULONG	io_Actual;
ULONG	io_Length;
APTR	io_Data;
ULONG	io_Offset;
};
#define	DEV_BEGINIO	(-30L)
#define	DEV_ABORTIO	(-36L)
#define	IOB_QUICK	0L
#define	IOF_QUICK	(1L<<0)
#define	CMD_INVALID	0L
#define	CMD_RESET	1L
#define	CMD_READ	2L
#define	CMD_WRITE	3L
#define	CMD_UPDATE	4L
#define	CMD_CLEAR	5L
#define	CMD_STOP	6L
#define	CMD_START	7L
#define	CMD_FLUSH	8L
#define	CMD_NONSTD	9L
#endif
