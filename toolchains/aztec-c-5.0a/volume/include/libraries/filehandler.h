#ifndef	LIBRARIES_FILEHANDLER_H
#define	LIBRARIES_FILEHANDLER_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif	/* !EXEC_TYPES_H */
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif	/* !EXEC_PORTS_H */
#ifndef	LIBRARIES_DOS_H
#include	"libraries/dos.h"
#endif	/* !LIBRARIES_DOS_H */
struct	DosEnvec	{
ULONG	de_TableSize;
ULONG	de_SizeBlock;
ULONG	de_SecOrg;
ULONG	de_Surfaces;
ULONG	de_SectorPerBlock;
ULONG	de_BlocksPerTrack;
ULONG	de_Reserved;
ULONG	de_PreAlloc;
ULONG	de_Interleave;
ULONG	de_LowCyl;
ULONG	de_HighCyl;
ULONG	de_NumBuffers;
ULONG	de_BufMemType;
ULONG	de_MaxTransfer;
ULONG	de_Mask;
LONG	de_BootPri;
ULONG	de_DosType;
};
#define	DE_TABLESIZE	0L
#define	DE_SIZEBLOCK	1L
#define	DE_SECORG	2L
#define	DE_NUMHEADS	3L
#define	DE_SECSPERBLK	4L
#define	DE_BLKSPERTRACK	5L
#define	DE_RESERVEDBLKS	6L
#define	DE_PREFAC	7L
#define	DE_INTERLEAVE	8L
#define	DE_LOWCYL	9L
#define	DE_UPPERCYL	10L
#define	DE_NUMBUFFERS	11L
#define	DE_MEMBUFTYPE	12L
#define	DE_BUFMEMTYPE	12L
#define	DE_MAXTRANSFER	13L
#define	DE_MASK	14L
#define	DE_BOOTPRI	15L
#define	DE_DOSTYPE	16L
struct	FileSysStartupMsg	{
ULONG	fssm_Unit;
BSTR	fssm_Device;
BPTR	fssm_Environ;
ULONG	fssm_Flags;
};
struct	DeviceNode	{
BPTR	dn_Next;
ULONG	dn_Type;
struct	MsgPort	*dn_Task;
BPTR	dn_Lock;
BSTR	dn_Handler;
ULONG	dn_StackSize;
LONG	dn_Priority;
BPTR	dn_Startup;
BPTR	dn_SegList;
BPTR	dn_GlobalVec;
BSTR	dn_Name;
};
#endif
