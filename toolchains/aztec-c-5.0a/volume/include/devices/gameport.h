#ifndef	DEVICES_GAMEPORT_H
#define	DEVICES_GAMEPORT_H
#ifndef EXEC_IO_H
#include "exec/io.h"
#endif
#define	GPD_READEVENT	(CMD_NONSTD+0)
#define	GPD_ASKCTYPE	(CMD_NONSTD+1)
#define	GPD_SETCTYPE	(CMD_NONSTD+2)
#define	GPD_ASKTRIGGER	(CMD_NONSTD+3)
#define	GPD_SETTRIGGER	(CMD_NONSTD+4)
#define	GPTB_DOWNKEYS	0L
#define	GPTF_DOWNKEYS	(1L<<0)
#define	GPTB_UPKEYS	1L
#define	GPTF_UPKEYS	(1L<<1)
struct	GamePortTrigger	{
UWORD	gpt_Keys;
UWORD	gpt_Timeout;
UWORD	gpt_XDelta;
UWORD	gpt_YDelta;
};
#define	GPCT_ALLOCATED	-1L
#define	GPCT_NOCONTROLLER	0L
#define	GPCT_MOUSE	1L
#define	GPCT_RELJOYSTICK	2L
#define	GPCT_ABSJOYSTICK	3L
#define	GPDERR_SETCTYPE	1L
#endif
