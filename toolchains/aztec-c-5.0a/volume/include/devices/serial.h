#ifndef	DEVICES_SERIAL_H
#define	DEVICES_SERIAL_H
#ifndef	EXEC_IO_H
#include	"exec/io.h"
#endif
struct	IOTArray	{
ULONG	TermArray0;
ULONG	TermArray1;
};
#define	SER_DEFAULT_CTLCHAR	0x11130000L
struct	IOExtSer	{
struct	IOStdReq	IOSer;
ULONG	io_CtlChar;
ULONG	io_RBufLen;
ULONG	io_ExtFlags;
ULONG	io_Baud;
ULONG	io_BrkTime;
struct	IOTArray	io_TermArray;
UBYTE	io_ReadLen;
UBYTE	io_WriteLen;
UBYTE	io_StopBits;
UBYTE	io_SerFlags;
UWORD	io_Status;
};
#define	SDCMD_QUERY	CMD_NONSTD
#define	SDCMD_BREAK	(CMD_NONSTD+1)
#define	SDCMD_SETPARAMS	(CMD_NONSTD+2)
#define	SERB_XDISABLED	7L
#define	SERF_XDISABLED	(1L<<7)
#define	SERB_EOFMODE	6L
#define	SERF_EOFMODE	(1L<<6)
#define	SERB_SHARED	5L
#define	SERF_SHARED	(1L<<5)
#define	SERB_RAD_BOOGIE	4L
#define	SERF_RAD_BOOGIE	(1L<<4)
#define	SERB_QUEUEDBRK	3L
#define	SERF_QUEUEDBRK	(1L<<3)
#define	SERB_7WIRE	2L
#define	SERF_7WIRE	(1L<<2)
#define	SERB_PARTY_ODD	1L
#define	SERF_PARTY_ODD	(1L<<1)
#define	SERB_PARTY_ON	0L
#define	SERF_PARTY_ON	(1L<<0)
#define	IO_STATB_XOFFREAD	12L
#define	IO_STATF_XOFFREAD	(1L<<12)
#define	IO_STATB_XOFFWRITE	11L
#define	IO_STATF_XOFFWRITE	(1L<<11)
#define	IO_STATB_READBREAK	10L
#define	IO_STATF_READBREAK	(1L<<10)
#define	IO_STATB_WROTEBREAK	9L
#define	IO_STATF_WROTEBREAK	(1L<<9)
#define	IO_STATB_OVERRUN	8L
#define	IO_STATF_OVERRUN	(1L<<8)
#define	SEXTB_MSPON	1L
#define	SEXTF_MSPON	(1L<<1)
#define	SEXTB_MARK	0L
#define	SEXTF_MARK	(1<<0)
#define	SerErr_DevBusy	1L
#define	SerErr_BufErr	4L
#define	SerErr_InvParam	5L
#define	SerErr_LineErr	6L
#define	SerErr_ParityErr	9L
#define	SerErr_TimerErr	11L
#define	SerErr_BufOverflow	12L
#define	SerErr_NoDSR	13L
#define	SerErr_DetectedBreak	15L
#ifdef	DEVICES_SERIAL_H_OBSOLETE
#define	SerErr_BaudMismatch	2L
#define	SerErr_InvBaud	3L
#define	SerErr_NotOpen	7L
#define	SerErr_PortReset	8L
#define	SerErr_InitErr	10L
#define	SerErr_NoCTS	14L
#define	IOSTB_XOFFREAD	4L
#define	IOSTF_XOFFREAD	(1L<<4)
#define	IOSTB_XOFFWRITE	3L
#define	IOSTF_XOFFWRITE	(1L<<3)
#define	IOSTB_READBREAK	2L
#define	IOSTF_READBREAK	(1L<<2)
#define	IOSTB_WROTEBREAK	1L
#define	IOSTF_WROTEBREAK	(1L<<1)
#define	IOSTB_OVERRUN	0L
#define	IOSTF_OVERRUN	(1L<<0)
#define	IOSERB_BUFRREAD	7L
#define	IOSERF_BUFRREAD	(1L<<7)
#define	IOSERB_QUEUED	6L
#define	IOSERF_QUEUED	(1L<<6)
#define	IOSERB_ABORT	5L
#define	IOSERF_ABORT	(1L<<5)
#define	IOSERB_ACTIVE	4L
#define	IOSERF_ACTIVE	(1L<<4)
#endif
#define	SERIALNAME	"serial.device"
#endif
