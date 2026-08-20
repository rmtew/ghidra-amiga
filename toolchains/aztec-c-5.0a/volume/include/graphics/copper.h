#ifndef	GRAPHICS_COPPER_H
#define	GRAPHICS_COPPER_H
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif
#define	COPPER_MOVE	0
#define	COPPER_WAIT	1
#define	CPRNXTBUF	2
#define	CPR_NT_LOF	0x8000
#define	CPR_NT_SHT	0x4000
struct	CopIns
{
short	OpCode;
union
{
struct	CopList	*nxtlist;
struct
{
union
{
SHORT	VWaitPos;
SHORT	DestAddr;
}	u1;
union
{
SHORT	HWaitPos;
SHORT	DestData;
}	u2;
}	u4;
}	u3;
};
#define	NXTLIST	u3.nxtlist
#define	VWAITPOS	u3.u4.u1.VWaitPos
#define	DESTADDR	u3.u4.u1.DestAddr
#define	HWAITPOS	u3.u4.u2.HWaitPos
#define	DESTDATA	u3.u4.u2.DestData
struct	cprlist
{
struct	cprlist	*Next;
UWORD	*start;
SHORT	MaxCount;
};
struct	CopList
{
struct	CopList	*Next;
struct	CopList	*_CopList;
struct	ViewPort	*_ViewPort;
struct	CopIns	*CopIns;
struct	CopIns	*CopPtr;
UWORD	*CopLStart;
UWORD	*CopSStart;
SHORT	Count;
SHORT	MaxCount;
SHORT	DyOffset;
};
struct	UCopList
{
struct	UCopList	*Next;
struct	CopList	*FirstCopList;
struct	CopList	*CopList;
};
struct	copinit
{
UWORD	diagstrt[4];
UWORD	sprstrtup[(2*8*2)+2+(2*2)+2];
UWORD	sprstop[2];
};
#endif
