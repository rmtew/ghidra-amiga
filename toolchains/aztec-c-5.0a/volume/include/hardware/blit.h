#ifndef	HARDWARE_BLIT_H
#define	HARDWARE_BLIT_H
#define	HSIZEBITS	6
#define	VSIZEBITS	16-HSIZEBITS
#define	HSIZEMASK	0x3f
#define	VSIZEMASK	0x3FF
#define	MAXBYTESPERROW	128
#define	ABC	0x80
#define	ABNC	0x40
#define	ANBC	0x20
#define	ANBNC	0x10
#define	NABC	0x8
#define	NABNC	0x4
#define	NANBC	0x2
#define	NANBNC	0x1
#define	A_OR_B	ABC|ANBC|NABC	|	ABNC|ANBNC|NABNC
#define	A_OR_C	ABC|NABC|ABNC	|	ANBC|NANBC|ANBNC
#define	A_XOR_C	NABC|ABNC	|	NANBC|ANBNC
#define	A_TO_D	ABC|ANBC|ABNC|ANBNC
#define	BC0B_DEST	8
#define	BC0B_SRCC	9
#define	BC0B_SRCB	10
#define	BC0B_SRCA	11
#define	BC0F_DEST	0x100
#define	BC0F_SRCC	0x200
#define	BC0F_SRCB	0x400
#define	BC0F_SRCA	0x800
#define	BC1F_DESC	2
#define	DEST	0x100
#define	SRCC	0x200
#define	SRCB	0x400
#define	SRCA	0x800
#define	ASHIFTSHIFT	12
#define	BSHIFTSHIFT	12
#define	LINEMODE	0x1
#define	FILL_OR	0x8
#define	FILL_XOR	0x10
#define	FILL_CARRYIN	0x4
#define	ONEDOT	0x2
#define	OVFLAG	0x20
#define	SIGNFLAG	0x40
#define	BLITREVERSE	0x2
#define	SUD	0x10
#define	SUL	0x8
#define	AUL	0x4
#define	OCTANT8	24
#define	OCTANT7	4
#define	OCTANT6	12
#define	OCTANT5	28
#define	OCTANT4	20
#define	OCTANT3	8
#define	OCTANT2	0
#define	OCTANT1	16
struct	bltnode
{
struct	bltnode	*n;
int	(*function)();
char	stat;
short	blitsize;
short	beamsync;
int	(*cleanup)();
};
#define	CLEANUP	0x40
#define	CLEANME	CLEANUP
#endif
