// Wokbench/startup.h
//
// (C) 1987, MYS Associates
//
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif !EXEC_TYPES_H
#ifndef EXEC_PORTS_H
#include "exec/ports.h"
#endif !EXEC_PORTS_H
#ifndef LIBRARIES_DOS_H
#include "libraries/dos.h"
#endif !LIBRARIES_DOS_H

struct WBArg {
	BPTR	wa_Lock;
	BYTE	*wa_Name;
};

struct WBStartup : public Message {
	MsgPort	*sm_Process;
	BPTR	sm_Segment;
	LONG	sm_NumArgs;
	char	*sm_ToolWindow;
	WBArg	*sm_ArgList;
};
