#ifndef	RESOURCES_DISK_H
#define	RESOURCES_DISK_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif	/* !EXEC_TYPES_H */
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif	/* !EXEC_LISTS_H */
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif	/* !EXEC_PORTS_H */
#ifndef	EXEC_INTERRUPTS_H
#include	"exec/interrupts.h"
#endif	/* !EXEC_INTERRUPTS_H */
#ifndef	EXEC_LIBRARIES_H
#include	"exec/libraries.h"
#endif	/* !EXEC_LIBRARIES_H */
struct	DiscResourceUnit	{
struct	Message	dru_Message;
struct	Interrupt	dru_DiscBlock;
struct	Interrupt	dru_DiscSync;
struct	Interrupt	dru_Index;
};
struct	DiscResource	{
struct	Library	dr_Library;
struct	DiscResourceUnit	*dr_Current;
UBYTE	dr_Flags;
UBYTE	dr_pad;
struct	Library	*dr_SysLib;
struct	Library	*dr_CiaResource;
ULONG	dr_UnitID[4];
struct	List	dr_Waiting;
struct	Interrupt	dr_DiscBlock;
struct	Interrupt	dr_DiscSync;
struct	Interrupt	dr_Index;
};
#define	DRB_ALLOC0	0L
#define	DRB_ALLOC1	1L
#define	DRB_ALLOC2	2L
#define	DRB_ALLOC3	3L
#define	DRB_ACTIVE	7L
#define	DRF_ALLOC0	(1L<<0)
#define	DRF_ALLOC1	(1L<<1)
#define	DRF_ALLOC2	(1L<<2)
#define	DRF_ALLOC3	(1L<<3)
#define	DRF_ACTIVE	(1L<<7)
#define	DSKDMAOFF	0x4000L
#define	DISKNAME	"disk.resource"
#define	DR_ALLOCUNIT	(LIB_BASE	-	0*LIB_VECTSIZE)
#define	DR_FREEUNIT	(LIB_BASE	-	1*LIB_VECTSIZE)
#define	DR_GETUNIT	(LIB_BASE	-	2*LIB_VECTSIZE)
#define	DR_GIVEUNIT	(LIB_BASE	-	3*LIB_VECTSIZE)
#define	DR_GETUNITID	(LIB_BASE	-	4*LIB_VECTSIZE)
#define	DR_LASTCOMM	(DR_GIVEUNIT)
#define	DRT_AMIGA	(0x00000000L)
#define	DRT_37422D2S	(0x55555555L)
#define	DRT_EMPTY	(0xFFFFFFFFL)
#endif
