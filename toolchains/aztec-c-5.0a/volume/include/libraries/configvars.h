#ifndef	LIBRARIES_CONFIGVARS_H
#define	LIBRARIES_CONFIGVARS_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif	/* !EXEC_TYPES_H */
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
#ifndef	LIBRARIES_CONFIGREGS_H
#include	"libraries/configregs.h"
#endif	/* !LIBRARIES_CONFIGREGS_H */
struct	ConfigDev	{
struct	Node	cd_Node;
UBYTE	cd_Flags;
UBYTE	cd_Pad;
struct	ExpansionRom	cd_Rom;
APTR	cd_BoardAddr;
APTR	cd_BoardSize;
UWORD	cd_SlotAddr;
UWORD	cd_SlotSize;
APTR	cd_Driver;
struct	ConfigDev	*	cd_NextCD;
ULONG	cd_Unused[4];
};
#define	CDB_SHUTUP	0
#define	CDB_CONFIGME	1
#define	CDF_SHUTUP	0x01
#define	CDF_CONFIGME	0x02
struct	CurrentBinding	{
struct	ConfigDev	*	cb_ConfigDev;
UBYTE	*	cb_FileName;
UBYTE	*	cb_ProductString;
UBYTE	**	cb_ToolTypes;
};
struct	ConfigDev	*AllocConfigDev(),	*FindConfigDev();
#endif
