#ifndef DEVICES_BOOTBLOCK_H
#define DEVICES_BOOTBLOCK_H
/*
**	$Filename: devices/bootblock.h $
**	$Release: 1.3 $
**
**	BootBlock definition: 
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

struct BootBlock {
	UBYTE	bb_id[4];		/* 4 character identifier */
	LONG	bb_chksum;	/* boot block checksum (balance) */
	LONG	bb_dosblock;	/* reserved for DOS patch */
};

#define		BOOTSECTS	2	/* 1K bootstrap */

#define BBID_DOS	{ 'D', 'O', 'S', '\0' }
#define BBID_KICK	{ 'K', 'I', 'C', 'K' }

#define BBNAME_DOS	(((long)'D'<<24)|((long)'O'<<16)|('S'<<8))
#define BBNAME_KICK	(((long)'K'<<24)|((long)'I'<<16)|('C'<<8)|('K'))

#endif	/* DEVICES_BOOTBLOCK_H */
