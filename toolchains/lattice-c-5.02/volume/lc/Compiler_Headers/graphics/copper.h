#ifndef GRAPHICS_COPPER_H
#define GRAPHICS_COPPER_H
/*
**	$Filename: graphics/copper.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#define COPPER_MOVE 0	    /* pseude opcode for move #XXXX,dir */
#define COPPER_WAIT 1	    /* pseudo opcode for wait y,x */
#define CPRNXTBUF   2	    /* continue processing with next buffer */
#define CPR_NT_LOF  0x8000  /* copper instruction only for short frames */
#define CPR_NT_SHT  0x4000  /* copper instruction only for long frames */
struct CopIns
{
    short   OpCode; /* 0 = move, 1 = wait */
    union
    {
    struct CopList *nxtlist;
    struct 
    {
   union
   {
   SHORT   VWaitPos;	    /* vertical beam wait */
   SHORT   DestAddr;	    /* destination address of copper move */
   } u1;
   union
   {
   SHORT   HWaitPos;	    /* horizontal beam wait position */
   SHORT   DestData;	    /* destination immediate data to send */
   } u2;
    } u4;
    } u3;
};
/* shorthand for above */
#define NXTLIST	    u3.nxtlist
#define VWAITPOS    u3.u4.u1.VWaitPos
#define DESTADDR    u3.u4.u1.DestAddr
#define HWAITPOS    u3.u4.u2.HWaitPos
#define DESTDATA    u3.u4.u2.DestData


/* structure of cprlist that points to list that hardware actually executes */
struct cprlist
{
    struct cprlist *Next;
    UWORD   *start;	    /* start of copper list */
    SHORT   MaxCount;	    /* number of long instructions */
};

struct CopList
{
    struct  CopList *Next;  /* next block for this copper list */
    struct  CopList *_CopList;	/* system use */
    struct  ViewPort *_ViewPort;    /* system use */
    struct  CopIns *CopIns; /* start of this block */
    struct  CopIns *CopPtr; /* intermediate ptr */
    UWORD   *CopLStart;	    /* mrgcop fills this in for Long Frame*/
    UWORD   *CopSStart;	    /* mrgcop fills this in for Short Frame*/
    SHORT   Count;	    /* intermediate counter */
    SHORT   MaxCount;	    /* max # of copins for this block */
    SHORT   DyOffset;	    /* offset this copper list vertical waits */
};

struct UCopList
{
    struct UCopList *Next;
    struct CopList  *FirstCopList; /* head node of this copper list */
    struct CopList  *CopList;	   /* node in use */
};

struct copinit
{
    UWORD diagstrt[4];	    /* copper list for first bitplane */
    UWORD sprstrtup[(2*8*2)+2+(2*2)+2];
    UWORD   sprstop[2];
};

#endif	/* GRAPHICS_COPPER_H */
