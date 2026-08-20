// devices/timer.h
//
// (C) 1987, MTS Associates
//
#ifndef DEVICES_TIMER_H
#define DEVICES_TIMER_H
#ifndef EXEC_IO_H
#include "exec/io.h"
#endif EXEC_IO_H
#define UNIT_MICROHZ    0
#define UNIT_VBLANK     1
#define TIMERNAME "timer.device"

struct timeval {
    ULONG tv_secs;
    ULONG tv_micro;

	timeval(){}
	timeval(ULONG secs, ULONG micros=0)
	{tv_secs=secs; tv_micro=micros;}
};

struct timerequest : public IORequest {
    timeval tr_time;
};

#define TR_ADDREQUEST   CMD_NONSTD
#define TR_GETSYSTIME   (CMD_NONSTD+1)
#define TR_SETSYSTIME   (CMD_NONSTD+2)
#endif DEVICES_TIMER_H
