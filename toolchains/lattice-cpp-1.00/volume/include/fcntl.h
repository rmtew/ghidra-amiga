/* Lattice fcntl.h
*
* The following symbols are used for the "open" and "creat" functions.
* They are generally UNIX-compatible, except for O_APPEND under MSDOS,
* which has been moved in order to accomodate the file sharing flags
* defined in MSDOS Version 3.
*
* Also, O_TEMP, O_UNIQ, and O_RAW are Lattice extensions.
*
**/
#define O_RDONLY 0	/* Read-only value (right byte of mode word) */
#define O_WRONLY 1	/* Write-only value */
#define O_RDWR 2	/* Read-write value */

#define O_APPEND 8	/* Append mode flag */

#define O_CREAT 0x0100	/* File creation flag */
#define O_TRUNC 0x0200	/* File truncation flag */
#define O_EXCL 0x0400	/* Exclusive access flag */

#define O_RAW 0x8000	/* Raw I/O flag (Lattice feature) */

/**
*
* The following flags are used to establish the protection mode.  Note
* that under MSDOS only write-protection is meaningful.
*
*
*/

#define S_IREAD  8
#define S_IWRITE  4
#define S_IEXECUTE 2
#define S_IDELETE  1

/**
*
* The following symbols are used for the "fcntl" function.
*
*/
#define F_DUPFD 0	/* Duplicate file descriptor */
#define F_GETFD 1	/* Get file descriptor flags */
#define F_SETFD 2	/* Set file descriptor flags */
#define F_GETFL 3	/* Get file flags */
#define F_SETFL 4	/* Set file flags */

/**
*
* External definitions
*
**/

extern int open(const char *name, int mode, int prot=0 );
extern int creat(const char *name, int rights);
extern int unlink(const char *n);
extern int remove(const char *n);
extern int rename(const char *oldn, const char *newn);
extern unsigned read(int f, void *buff,
	 const unsigned nbytes);
extern unsigned write(int f, const void *buff, const unsigned nbytes);
extern long lseek(int f, long locn, int mode);
extern long tell(int f);
extern int close(int f);
extern int iomode(int f, int m);

/**
*
* Define NULL if it's not already defined
*
*/
#ifndef NULL
#define NULL 0
#endif
