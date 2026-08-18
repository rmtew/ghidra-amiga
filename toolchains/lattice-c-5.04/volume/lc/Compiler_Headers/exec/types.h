#ifndef EXEC_TYPES_H
#define EXEC_TYPES_H
/*
**	$Filename: exec/types.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#define GLOBAL	extern	    /* the declaratory use of an external */
#define IMPORT	extern	    /* reference to an external */
#define STATIC	static	    /* a local static variable */
#define REGISTER register   /* a (hopefully) register variable */

#define VOID		void	/* typedef does not seem to work here */

typedef long		LONG;	    /* signed 32-bit quantity */
typedef unsigned long	ULONG;	    /* unsigned 32-bit quantity */
typedef unsigned long	LONGBITS;   /* 32 bits manipulated individually */
typedef short		WORD;	    /* signed 16-bit quantity */
typedef unsigned short	UWORD;	    /* unsigned 16-bit quantity */
typedef unsigned short	WORDBITS;   /* 16 bits manipulated individually */
typedef char		BYTE;	    /* signed 8-bit quantity */
typedef unsigned char	UBYTE;	    /* unsigned 8-bit quantity */
typedef unsigned char	BYTEBITS;   /* 8 bits manipulated individually */
typedef unsigned char	*STRPTR;    /* string pointer */
typedef STRPTR		*APTR;	    /* absolute memory pointer */

/* sigh.  APTR was misdefined, but compatibility rules.	 Heres what it
 * should have been
 */
typedef ULONG		CPTR;	    /* absolute memory pointer */

/* For compatability only: (don't use in new code) */
typedef short		SHORT;	    /* signed 16-bit quantity (WORD) */
typedef unsigned short	USHORT;	    /* unsigned 16-bit quantity (UWORD) */


/*	Types with specific semantics */
typedef float		FLOAT;
typedef double		DOUBLE;
typedef short		COUNT;
typedef unsigned short	UCOUNT;
typedef short		BOOL;
typedef unsigned char	TEXT;

#define TRUE		1
#define FALSE		0
#define NULL		0L

#define BYTEMASK	0xFF

#define LIBRARY_VERSION 34

#endif	/* EXEC_TYPES_H */
