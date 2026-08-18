#ifndef	EXEC_TASKS_H
#define	EXEC_TASKS_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif	/* !EXEC_LISTS_H */
struct	Task	{
struct	Node	tc_Node;
UBYTE	tc_Flags;
UBYTE	tc_State;
BYTE	tc_IDNestCnt;
BYTE	tc_TDNestCnt;
ULONG	tc_SigAlloc;
ULONG	tc_SigWait;
ULONG	tc_SigRecvd;
ULONG	tc_SigExcept;
UWORD	tc_TrapAlloc;
UWORD	tc_TrapAble;
APTR	tc_ExceptData;
APTR	tc_ExceptCode;
APTR	tc_TrapData;
APTR	tc_TrapCode;
APTR	tc_SPReg;
APTR	tc_SPLower;
APTR	tc_SPUpper;
VOID	(*tc_Switch)();
VOID	(*tc_Launch)();
struct	List	tc_MemEntry;
APTR	tc_UserData;
};
#define	TB_PROCTIME	0L
#define	TB_STACKCHK	4L
#define	TB_EXCEPT	5L
#define	TB_SWITCH	6L
#define	TB_LAUNCH	7L
#define	TF_PROCTIME	(1L<<0)
#define	TF_STACKCHK	(1L<<4)
#define	TF_EXCEPT	(1L<<5)
#define	TF_SWITCH	(1L<<6)
#define	TF_LAUNCH	(1L<<7)
#define	TS_INVALID	0L
#define	TS_ADDED	1L
#define	TS_RUN	2L
#define	TS_READY	3L
#define	TS_WAIT	4L
#define	TS_EXCEPT	5L
#define	TS_REMOVED	6L
#define	SIGB_ABORT	0L
#define	SIGB_CHILD	1L
#define	SIGB_BLIT	4L
#define	SIGB_SINGLE	4L
#define	SIGB_DOS	8L
#define	SIGF_ABORT	(1L<<0)
#define	SIGF_CHILD	(1L<<1)
#define	SIGF_BLIT	(1L<<4)
#define	SIGF_SINGLE	(1L<<4)
#define	SIGF_DOS	(1L<<8)
#endif
