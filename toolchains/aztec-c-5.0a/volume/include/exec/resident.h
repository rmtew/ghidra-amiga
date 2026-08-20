#ifndef	EXEC_RESIDENT_H
#define	EXEC_RESIDENT_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
struct	Resident	{
UWORD	rt_MatchWord;
struct	Resident	*rt_MatchTag;
APTR	rt_EndSkip;
UBYTE	rt_Flags;
UBYTE	rt_Version;
UBYTE	rt_Type;
BYTE	rt_Pri;
char	*rt_Name;
char	*rt_IdString;
APTR	rt_Init;
};
#define	RTC_MATCHWORD	0x4AFCL
#define	RTF_AUTOINIT	(1L<<7)
#define	RTF_COLDSTART	(1L<<0)
#define	RTM_WHEN	3L
#define	RTW_NEVER	0L
#define	RTW_COLDSTART	1L
#endif
