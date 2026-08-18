
************************************************************************
*
*	C Program Startup/Exit (Combo Version: CLI and WorkBench)
*
************************************************************************
*
*   Source Control:
*
*       $Header$
*
*       $Locker$
*
*       $Log$
*
************************************************************************


******* Included Files *************************************************

	INCLUDE	"exec/types.i"
	INCLUDE "exec/alerts.i"
	INCLUDE "exec/nodes.i"
	INCLUDE "exec/lists.i"
	INCLUDE "exec/ports.i"
	INCLUDE "exec/libraries.i"
	INCLUDE "exec/tasks.i"
	INCLUDE "libraries/dos.i"
	INCLUDE "libraries/dosextens.i"
	INCLUDE "workbench/startup.i"


******* Imported *******************************************************

xlib	macro
	xref	_LVO\1
	endm

	xref	_AbsExecBase
	xref	_Input
	xref	_Output

	xref	_main			; C code entry point

	xlib	Alert
	xlib	FindTask
	xlib	Forbid
	xlib	GetMsg
	xlib	OpenLibrary
	xlib	CloseLibrary
	xlib	ReplyMsg
	xlib	Wait
	xlib	WaitPort

	xlib	CurrentDir
	xlib	Open


******* Exported *******************************************************

	xdef	_SysBase
	xdef	_DOSBase

	xdef	_errno
	xdef	_stdin
	xdef	_stdout
	xdef	_stderr

	xdef	_exit			; standard C exit function


callsys	macro
	CALLLIB	_LVO\1
	endm


************************************************************************
*
*	Standard Program Entry Point
*
************************************************************************
*
*	main (argc, argv)
*	   int  argc;
*	   char *argv[]; 
*
************************************************************************

startup:				; reference for Wack users
		move.l	sp,initialSP	; initial task stack pointer
		move.l	d0,dosCmdLen
		move.l	a0,dosCmdBuf
		clr.l	returnMsg

	;------ get Exec's library base pointer:
		move.l	_AbsExecBase,a6
		move.l	a6,_SysBase

	;------ get the address of our task
		suba.l	a1,a1
		callsys	FindTask
		move.l	d0,a4

	;------ are we running as a son of Workbench?
		tst.l	pr_CLI(A4)
		beq	fromWorkbench

;=======================================================================
;====== CLI Startup Code ===============================================
;=======================================================================
fromCLI:
	;------	attempt to open DOS library:
		bsr	openDOS

	;------ find command name:
		move.l	pr_CLI(a4),a0
		add.l   a0,a0		; bcpl pointer conversion
		add.l   a0,a0
		move.l	cli_CommandName(a0),a0
		add.l   a0,a0		; bcpl pointer conversion
		add.l   a0,a0

	;------ create buffer and array:
*		link	a6,#-(100+16*4+2*4)
		movem.l	d2/a2/a3,-(sp)
		lea	argvBuffer,a2
		lea	argvArray,a3
*		move.l	a3,16(sp)	; save 
		moveq.l	#1,d2		; param counter

	;------ fetch command name:
		moveq.l	#0,d0
		move.b	(a0)+,d0	; size of command name
		move.l	a2,(a3)+	; ptr to command name
		bra.s	1$
2$:		move.b	(a0)+,(a2)+
1$:		dbf	d0,2$
		clr.b	(a2)+

	;------	collect parameters:
		move.l	dosCmdLen,d0
		move.l	dosCmdBuf,a0

	;------ skip control characters and space:
3$:		move.b	(a0)+,d1
		subq.l	#1,d0
		ble.s	parmExit
		cmp.b	#' ',d1
		ble.s	3$

	;------ copy parameter:
		addq.l	#1,d2
		move.l	a2,(a3)+
		bra.s	5$
4$:		move.b	(a0)+,d1
		subq.l	#1,d0
		cmp.b	#' ',d1
		ble.s	6$
5$:		move.b	d1,(a2)+
		bra.s	4$
6$:
		clr.b	(a2)+
		bra.s	3$
parmExit:	clr.b	(a2)+
		clr.l	(a3)+

		move.l	d2,d0
		movem.l	(sp)+,d2/a2/a3
		pea	argvArray
		move.l	d0,-(sp)


*
*  The above code relies on the end of line containing a control
*  character of any type, i.e. a valid character must not be the
*  last.  This fact is ensured by DOS.
*
		

	;------ get standard input handle:
		jsr	_Input
		move.l	d0,_stdin

	;------ get standard output handle:
		jsr	_Output
		move.l	d0,_stdout
		move.l	d0,_stderr

	;------ call C main entry point
		jsr	_main

	;------ return success code:
		moveq.l	#0,D0
		move.l	initialSP,sp	; restore stack ptr
		rts

;=======================================================================
;====== Workbench Startup Code =========================================
;=======================================================================
fromWorkbench:
	;------ open the DOS library:
		bsr	openDOS

	;------ we are now set up.  wait for a message from our starter
		bsr	waitmsg

	;------ save the message so we can return it later
		move.l	d0,returnMsg	NOTE: no GetMsg performed

	;------ push the message on the stack for wbmain
		clr.l	-(SP)		indicate: run from Workbench
		move.l	d0,-(SP)

	;------ get the first argument
		move.l	d0,a2
		move.l	sm_ArgList(a2),d0
		beq.s	docons

	;------ and set the current directory to the same directory
		move.l	_DOSBase,a6
		move.l	d0,a0
		move.l	wa_Lock(a0),d1
		callsys	CurrentDir

docons:
	;------ get the toolwindow argument
		move.l	sm_ToolWindow(A2),d1
		beq.s	domain

	;------ open up the file
		move.l	#MODE_OLDFILE,d2
		callsys	Open

	;------ set the C input and output descriptors
		move.l	d0,_stdin
		move.l	d0,_stdout
		move.l	d0,_stderr
		beq.s	domain

	;------ set the console task (so Open( "*", mode ) will work
	;	waitmsg has left the task pointer in A4 for us
		lsl.l	#2,d0
		move.l	d0,a0
		move.l	fh_Type(a0),pr_ConsoleTask(A4)

domain:
		jsr	_main
		moveq.l	#0,d0		Successful return code
		bra.s	exit2


************************************************************************
*
*	C Program Exit Function
*
************************************************************************
*
*  Warning: this function really needs to do more than this.
*
************************************************************************

_exit:
		move.l	4(SP),d0	; extract return code
exit2:
		move.l  initialSP,SP	; restore stack pointer
		move.l	d0,-(SP)	; save return code

	;------ close DOS library:
		move.l	_AbsExecBase,A6
		move.l	_DOSBase,d0
		beq.s	1$
		move.l	d0,a1
1$:		callsys	CloseLibrary

	;------ if we ran from CLI, skip workbench cleanup:
		tst.l	returnMsg
		beq.s	exitToDOS

	;------ return the startup message to our parent
	;	we forbid so workbench can't UnLoadSeg() us
	;	before we are done:
		callsys Forbid
		move.l	returnMsg,a1
		callsys	ReplyMsg

	;------ this rts sends us back to DOS:
exitToDOS:
		move.l	(SP)+,d0
		rts


;-----------------------------------------------------------------------
noDOS:
		ALERT	(AG_OpenLib!AO_DOSLib)
		moveq.l	#100,d0
		bra.s	exit2


;-----------------------------------------------------------------------
; This routine gets the message that workbench will send to us
; called with task id in A4

waitmsg:
		lea	pr_MsgPort(A4),a0  	* our process base
		callsys	WaitPort
		lea	pr_MsgPort(A4),a0  	* our process base
		callsys GetMsg
		rts

;-----------------------------------------------------------------------
;  Open the DOS library:

openDOS
		clr.l	_DOSBase
		lea	DOSName,A1
		move.l	#LIBRARY_VERSION,d0
		callsys OpenLibrary
		move.l	D0,_DOSBase
		beq	noDOS
		rts


************************************************************************

	DATA

************************************************************************

VerRev		dc.w	1,0

_SysBase	dc.l	0
_DOSBase	dc.l	0

_errno		dc.l	0
_stdin		dc.l	0
_stdout		dc.l	0
_stderr		dc.l	0

initialSP	dc.l	0
returnMsg	dc.l	0

dosCmdLen	dc.l	0
dosCmdBuf	dc.l	0

argvArray	ds.l	32
argvBuffer	ds.b	256

DOSName		DOSNAME


	END
