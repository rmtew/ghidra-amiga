#ifndef	LIBRARIES_EXPANSION_H
#define	LIBRARIES_EXPANSION_H
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif /* !EXEC_TYPES_H */
#define	EXPANSIONNAME	"expansion.library"
#define	ADNB_STARTPROC	0L
#define	ADNF_STARTPROC	(1L<<0)
struct	ConfigDev	*AllocConfigDev();
CPTR	AllocExpansionMem();
struct	ConfigDev	*FindConfigDev();
struct	DeviceNode	*MakeDosNode();
#endif
