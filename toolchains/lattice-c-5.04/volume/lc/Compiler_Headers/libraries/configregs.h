#ifndef LIBRARIES_CONFIGREGS_H
#define LIBRARIES_CONFIGREGS_H
/*
**	$Filename: libraries/configregs.h $
**	$Release: 1.3 $
**
**	register and bit definitions for expansion boards 
**
**	(C) Copyright 1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif	!EXEC_TYPES_H

/*
** Expansion boards are actually organized such that only one nibble per
** word (16 bits) are valid information.  This table is structured
** as LOGICAL information.  This means that it never corresponds
** exactly with a physical implementation.
**
** The expansion space is logically split into two regions:
** a rom portion and a control portion.	 The rom portion is
** actually stored in one's complement form (except for the
** er_type field).
*/


struct ExpansionRom {
    UBYTE	er_Type;
    UBYTE	er_Product;
    UBYTE	er_Flags;
    UBYTE	er_Reserved03;
    UWORD	er_Manufacturer;
    ULONG	er_SerialNumber;
    UWORD	er_InitDiagVec;
    UBYTE	er_Reserved0c;
    UBYTE	er_Reserved0d;
    UBYTE	er_Reserved0e;
    UBYTE	er_Reserved0f;
};

struct ExpansionControl {
    UBYTE	ec_Interrupt;		/* interrupt control register */
    UBYTE	ec_Reserved11;
    UBYTE	ec_BaseAddress;		/* set new config address */
    UBYTE	ec_Shutup;		/* don't respond, pass config out */
    UBYTE	ec_Reserved14;
    UBYTE	ec_Reserved15;
    UBYTE	ec_Reserved16;
    UBYTE	ec_Reserved17;
    UBYTE	ec_Reserved18;
    UBYTE	ec_Reserved19;
    UBYTE	ec_Reserved1a;
    UBYTE	ec_Reserved1b;
    UBYTE	ec_Reserved1c;
    UBYTE	ec_Reserved1d;
    UBYTE	ec_Reserved1e;
    UBYTE	ec_Reserved1f;
};

/*
** many of the constants below consist of a triplet of equivalent
** definitions: xxMASK is a bit mask of those bits that matter.
** xxBIT is the starting bit number of the field.  xxSIZE is the
** number of bits that make up the definition.	This method is
** used when the field is larger than one bit.
**
** If the field is only one bit wide then the xxB_xx and xxF_xx convention
** is used (xxB_xx is the bit number, and xxF_xx is mask of the bit).
*/

/* manifest constants */
#define E_SLOTSIZE		0x10000
#define E_SLOTMASK		0xffff
#define E_SLOTSHIFT		16

/* these define the two free regions of Zorro memory space.
** THESE MAY WELL CHANGE FOR FUTURE PRODUCTS!
*/
#define E_EXPANSIONBASE		0xe80000
#define E_EXPANSIONSIZE		0x080000
#define E_EXPANSIONSLOTS	8

#define E_MEMORYBASE		0x200000
#define E_MEMORYSIZE		0x800000
#define E_MEMORYSLOTS		128



/****** ec_Type definitions */

/* board type -- ignore "old style" boards */
#define ERT_TYPEMASK		0xc0
#define ERT_TYPEBIT		6
#define ERT_TYPESIZE		2
#define ERT_NEWBOARD		0xc0


/* type field memory size */
#define ERT_MEMMASK		0x07
#define ERT_MEMBIT		0
#define ERT_MEMSIZE		3


/* other bits defined in type field */
#define ERTB_CHAINEDCONFIG	3
#define ERTB_DIAGVALID		4
#define ERTB_MEMLIST		5

#define ERTF_CHAINEDCONFIG	(1<<3)
#define ERTF_DIAGVALID		(1<<4)
#define ERTF_MEMLIST		(1<<5)


/* er_Flags byte -- for those things that didn't fit into the type byte */
#define ERFB_MEMSPACE		7	/* wants to be in 8 meg space.	Also
					** implies that board is moveable
					*/
#define ERFB_NOSHUTUP		6	/* board can't be shut up.  Must not
					** be a board.	Must be a box that
					** does not pass on the bus.
					*/

#define ERFF_MEMSPACE		(1<<7)
#define ERFF_NOSHUTUP		(1<<6)


/* figure out amount of memory needed by this box/board */
#define ERT_MEMNEEDED(t)	\
	(((t)&ERT_MEMMASK)? 0x10000L << (((t)&ERT_MEMMASK) -1) : 0x800000 )

/* same as ERT_MEMNEEDED, but return number of slots */
#define ERT_SLOTSNEEDED(t)	\
	(((t)&ERT_MEMMASK)? 1L << (((t)&ERT_MEMMASK)-1) : 0x80 )

/* interrupt control register */
#define ECIB_INTENA		1
#define ECIB_RESET		3
#define ECIB_INT2PEND		4
#define ECIB_INT6PEND		5
#define ECIB_INT7PEND		6
#define ECIB_INTERRUPTING	7

#define ECIF_INTENA		(1<<1)
#define ECIF_RESET		(1<<3)
#define ECIF_INT2PEND		(1<<4)
#define ECIF_INT6PEND		(1<<5)
#define ECIF_INT7PEND		(1<<6)
#define ECIF_INTERRUPTING	(1<<7)


/* convert a expantion slot number into a memory address */
#define EC_MEMADDR(slot)		(((long)slot) << (E_SLOTSHIFT) )

/* a kludge to get the byte offset of a structure */
#define EROFFSET(er)	((int)&((struct ExpansionRom *)0)->er)
#define ECOFFSET(ec)	\
 (sizeof(struct ExpansionRom)+((int)&((struct ExpansionControl *)0)->ec))


/***************************************************************************
**
** these are the specifications for the diagnostic area.  If the Diagnostic
** Address Valid bit is set in the Board Type byte (the first byte in
** expansion space) then the Diag Init vector contains a valid offset.
**
** The Diag Init vector is actually a word offset from the base of the
** board.  The resulting address points to the base of the DiagArea
** structure.  The structure may be physically implemented either four,
** eight, or sixteen bits wide.	 The code will be copied out into
** ram first before being called.
**
** The da_Size field, and both code offsets (da_DiagPoint and da_BootPoint)
** are offsets from the diag area AFTER it has been copied into ram, and
** "de-nibbleized" (if needed).	 Inotherwords, the size is the size of
** the actual information, not how much address space is required to
** store it.
**
** All bits are encoded with uninverted logic (e.g. 5 volts on the bus
** is a logic one).
**
** If your board is to make use of the boot facility then it must leave
** its config area available even after it has been configured.	 Your
** boot vector will be called AFTER your board's final address has been
** set.
**
****************************************************************************/

struct DiagArea {
    UBYTE	da_Config;	/* see below for definitions */
    UBYTE	da_Flags;	/* see below for definitions */
    UWORD	da_Size;	/* the size (in bytes) of the total diag area */
    UWORD	da_DiagPoint;	/* where to start for diagnostics, or zero */
    UWORD	da_BootPoint;	/* where to start for booting */
    UWORD	da_Name;	/* offset in diag area where a string */
				/*   identifier can be found (or zero if no */
				/*   identifier is present). */

    UWORD	da_Reserved01;	/* two words of reserved data.	must be zero. */
    UWORD	da_Reserved02;
};

/* da_Config definitions */
#define DAC_BUSWIDTH	0xC0	/* two bits for bus width */
#define DAC_NIBBLEWIDE	0x00
#define DAC_BYTEWIDE	0x40
#define DAC_WORDWIDE	0x80

#define DAC_BOOTTIME	0x30	/* two bits for when to boot */
#define DAC_NEVER	0x00	/* obvious */
#define DAC_CONFIGTIME	0x10	/* call da_BootPoint when first configing the */
				/*   the device */
#define DAC_BINDTIME	0x20	/* run when binding drivers to boards */

/*
** These are the calling conventions for Diag or Boot area
**
** A7 -- points to at least 2K of stack
** A6 -- ExecBase
** A5 -- ExpansionBase
** A3 -- your board's ConfigDev structure
** A2 -- Base of diag/init area that was copied
** A0 -- Base of your board
**
** Your board should return a value in D0.  If this value is NULL, then
** the diag/init area that was copied in will be returned to the free
** memory pool.
*/

#endif	/* LIBRARIES_CONFIGREGS_H */
