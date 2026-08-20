/* Copyright Manx Software Systems, Inc. 1988.  All rights reserved */

#ifndef __STAT_H 
#define __STAT_H 

struct stat {
	char st_attr;
	long st_mtime;
	long st_size;
};

/* settings of the st_attr field */
#define	ST_DELETE	0x01
#define	ST_EXECUTE	0x02
#define	ST_WRITE	0x04
#define	ST_READ		0x08

/* the format of the st_mtime field is:
	seconds since Jan 1, 1978
*/

int stat(char *name, struct stat *buf);

#endif

