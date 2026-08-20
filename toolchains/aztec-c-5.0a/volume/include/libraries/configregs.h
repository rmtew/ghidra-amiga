#ifndef	LIBRARIES_CONFIGREGS_H
#define	LIBRARIES_CONFIGREGS_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif	/* !EXEC_TYPES_H */
struct	ExpansionRom	{
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
struct	ExpansionControl	{
UBYTE	ec_Interrupt;
UBYTE	ec_Reserved11;
UBYTE	ec_BaseAddress;
UBYTE	ec_Shutup;
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
#define	E_SLOTSIZE	0x10000
#define	E_SLOTMASK	0xffff
#define	E_SLOTSHIFT	16
#define	E_EXPANSIONBASE	0xe80000
#define	E_EXPANSIONSIZE	0x080000
#define	E_EXPANSIONSLOTS	8
#define	E_MEMORYBASE	0x200000
#define	E_MEMORYSIZE	0x800000
#define	E_MEMORYSLOTS	128
#define	ERT_TYPEMASK	0xc0
#define	ERT_TYPEBIT	6
#define	ERT_TYPESIZE	2
#define	ERT_NEWBOARD	0xc0
#define	ERT_MEMMASK	0x07
#define	ERT_MEMBIT	0
#define	ERT_MEMSIZE	3
#define	ERTB_CHAINEDCONFIG	3
#define	ERTB_DIAGVALID	4
#define	ERTB_MEMLIST	5
#define	ERTF_CHAINEDCONFIG	(1<<3)
#define	ERTF_DIAGVALID	(1<<4)
#define	ERTF_MEMLIST	(1<<5)
#define	ERFB_MEMSPACE	7
#define	ERFB_NOSHUTUP	6
#define	ERFF_MEMSPACE	(1<<7)
#define	ERFF_NOSHUTUP	(1<<6)
#define	ERT_MEMNEEDED(t)	\
(((t)&ERT_MEMMASK)?	0x10000	<<	(((t)&ERT_MEMMASK)	-1)	:	0x800000	)
#define	ERT_SLOTSNEEDED(t)	\
(((t)&ERT_MEMMASK)?	1	<<	(((t)&ERT_MEMMASK)-1)	:	0x80	)
#define	ECIB_INTENA	1
#define	ECIB_RESET	3
#define	ECIB_INT2PEND	4
#define	ECIB_INT6PEND	5
#define	ECIB_INT7PEND	6
#define	ECIB_INTERRUPTING	7
#define	ECIF_INTENA	(1<<1)
#define	ECIF_RESET	(1<<3)
#define	ECIF_INT2PEND	(1<<4)
#define	ECIF_INT6PEND	(1<<5)
#define	ECIF_INT7PEND	(1<<6)
#define	ECIF_INTERRUPTING	(1<<7)
#define	EC_MEMADDR(slot)	((slot)	<<	(E_SLOTSHIFT)	)
#define	EROFFSET(er)	((int)&((struct	ExpansionRom	*)0)->er)
#define	ECOFFSET(ec)	\
(sizeof(struct	ExpansionRom)+((int)&((struct	ExpansionControl	*)0)->ec))
struct	DiagArea	{
UBYTE	da_Config;
UBYTE	da_Flags;
UWORD	da_Size;
UWORD	da_DiagPoint;
UWORD	da_BootPoint;
UWORD	da_Name;
UWORD	da_Reserved01;
UWORD	da_Reserved02;
};
#define	DAC_BUSWIDTH	0xC0
#define	DAC_NIBBLEWIDE	0x00
#define	DAC_BYTEWIDE	0x40
#define	DAC_WORDWIDE	0x80
#define	DAC_BOOTTIME	0x30
#define	DAC_NEVER	0x00
#define	DAC_CONFIGTIME	0x10
#define	DAC_BINDTIME	0x20
#endif
