#ifndef DEVICES_TIMER_H
#include <devices/timer.h>
#endif
extern struct Library *TimerBase;
void AddTime(struct timerequest *, struct timerequest *);
void SubTime(struct timerequest *, struct timerequest *);
long CmpTime(struct timerequest *, struct timerequest *);
#ifndef  NO_PRAGMAS
#pragma libcall TimerBase AddTime 2a 9802
#pragma libcall TimerBase SubTime 30 9802
#pragma libcall TimerBase CmpTime 36 9802
#endif
