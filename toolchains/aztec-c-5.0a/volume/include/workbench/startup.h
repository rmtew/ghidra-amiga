#ifndef	WORKBENCH_STARTUP_H
#define	WORKBENCH_STARTUP_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif	/* !EXEC_TYPES_H */
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif	/* !EXEC_PORTS_H */
#ifndef	LIBRARIES_DOS_H
#include	"libraries/dos.h"
#endif	/* !LIBRARIES_DOS_H */
struct	WBStartup	{
struct	Message	sm_Message;
struct	MsgPort	*	sm_Process;
BPTR	sm_Segment;
LONG	sm_NumArgs;
char	*	sm_ToolWindow;
struct	WBArg	*	sm_ArgList;
};
struct	WBArg	{
BPTR	wa_Lock;
BYTE	*	wa_Name;
};
#endif
