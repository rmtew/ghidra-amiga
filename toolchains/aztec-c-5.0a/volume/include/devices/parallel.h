#ifndef	DEVICES_PARALLEL_H
#define	DEVICES_PARALLEL_H
#ifndef	EXEC_IO_H
#include	"exec/io.h"
#endif	/* !EXEC_IO_H */
struct	IOPArray	{
ULONG	PTermArray0;
ULONG	PTermArray1;
};
struct	IOExtPar	{
struct	IOStdReq	IOPar;
ULONG	io_PExtFlags;
UBYTE	io_Status;
UBYTE	io_ParFlags;
struct	IOPArray	io_PTermArray;
};
#define	PARB_SHARED	5L
#define	PARF_SHARED	(1L<<5)
#define	PARB_RAD_BOOGIE	3L
#define	PARF_RAD_BOOGIE	(1L<<3)
#define	PARB_EOFMODE	1L
#define	PARF_EOFMODE	(1L<<1)
#define	IOPARB_QUEUED	6L
#define	IOPARF_QUEUED	(1L<<6)
#define	IOPARB_ABORT	5L
#define	IOPARF_ABORT	(1L<<5)
#define	IOPARB_ACTIVE	4L
#define	IOPARF_ACTIVE	(1L<<4)
#define	IOPTB_RWDIR	3L
#define	IOPTF_RWDIR	(1L<<3)
#define	IOPTB_PARSEL	2L
#define	IOPTF_PARSEL	(1L<<2)
#define	IOPTB_PAPEROUT	1L
#define	IOPTF_PAPEROUT	(1L<<1)
#define	IOPTB_PARBUSY	0L
#define	IOPTF_PARBUSY	(1L<<0)
#define	PARALLELNAME	"parallel.device"
#define	PDCMD_QUERY	(CMD_NONSTD)
#define	PDCMD_SETPARAMS	(CMD_NONSTD+1)
#define	ParErr_DevBusy	1L
#define	ParErr_BufTooBig	2L
#define	ParErr_InvParam	3L
#define	ParErr_LineErr	4L
#define	ParErr_NotOpen	5L
#define	ParErr_PortReset	6L
#define	ParErr_InitErr	7L
#endif
