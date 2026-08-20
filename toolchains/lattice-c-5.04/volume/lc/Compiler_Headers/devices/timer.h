#ifndef DEVICES_TIMER_H
#define DEVICES_TIMER_H
/*
**	$Filename: devices/timer.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_IO_H
#include "exec/io.h"
#endif EXEC_IO_H

/* unit defintions */
#define UNIT_MICROHZ	0
#define UNIT_VBLANK	1

#define TIMERNAME	"timer.device"

struct timeval {
    ULONG tv_secs;
    ULONG tv_micro;
};

struct timerequest {
    struct IORequest tr_node;
    struct timeval tr_time;
};

/* IO_COMMAND to use for adding a timer */
#define TR_ADDREQUEST	CMD_NONSTD
#define TR_GETSYSTIME	(CMD_NONSTD+1)
#define TR_SETSYSTIME	(CMD_NONSTD+2)

#endif	/* DEVICES_TIMER_H */
