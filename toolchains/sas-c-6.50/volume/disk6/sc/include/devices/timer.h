€ˆDEVICES_TIMER_H€DEVICES_TIMER_H 1Œ<exec/types.h>Œ<exec/io.h>€UNIT_MICROHZ 0€UNIT_VBLANK 1€UNIT_ECLOCK 2€UNIT_WAITUNTIL 3€UNIT_WAITECLOCK 4€TIMERNAME "timer.device"
ƒtimeval{
—tv_secs;
—tv_micro;
};
ƒEClockVal{
—ev_hi;
—ev_lo;
};
ƒtimerequest{
ƒIORequest tr_node;
ƒtimeval tr_time;
};€TR_ADDREQUEST CMD_NONSTD€TR_GETSYSTIME (CMD_NONSTD+1)€TR_SETSYSTIME (CMD_NONSTD+2)‡