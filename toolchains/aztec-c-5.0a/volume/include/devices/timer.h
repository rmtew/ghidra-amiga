#ifndef	DEVICES_TIMER_H
#define	DEVICES_TIMER_H
#ifndef	EXEC_IO_H
#include	"exec/io.h"
#endif	/* EXEC_IO_H */
#define	UNIT_MICROHZ	0L
#define	UNIT_VBLANK	1L
#define	TIMERNAME	"timer.device"
struct	timeval	{
ULONG	tv_secs;
ULONG	tv_micro;
};
struct	timerequest	{
struct	IORequest	tr_node;
struct	timeval	tr_time;
};
#define	TR_ADDREQUEST	CMD_NONSTD
#define	TR_GETSYSTIME	(CMD_NONSTD+1)
#define	TR_SETSYSTIME	(CMD_NONSTD+2)
#endif
