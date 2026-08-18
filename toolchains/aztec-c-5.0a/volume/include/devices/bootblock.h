#ifndef	DEVICES_BOOTBLOCK_H
#define	DEVICES_BOOTBLOCK_H
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif
struct	BootBlock	{
UBYTE	bb_id[4];
LONG	bb_chksum;
LONG	bb_dosblock;
};
#define	BOOTSECTS	2L
#define	BBID_DOS	{	'D',	'O',	'S',	'\0'	}
#define	BBID_KICK	{	'K',	'I',	'C',	'K'	}
#define	BBNAME_DOS	(('D'<<24)|('O'<<16)|('S'<<8))
#define	BBNAME_KICK	(('K'<<24)|('I'<<16)|('C'<<8)|('K'))
#endif
