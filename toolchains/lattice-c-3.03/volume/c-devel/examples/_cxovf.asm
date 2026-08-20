	xdef	__xcovf
	xref	_cxovf
	xref	_exit
	xref	__StackPtr

__xcovf	move.l	__StackPtr,A7		; reset stack pointer
	jsr	_cxovf			; display requestor
	move.l	#20,-(A7)
	jsr	_exit			; call exit
	end
