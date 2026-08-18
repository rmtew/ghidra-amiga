// Dos.h - (Amiga) DOS definitions
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif

#ifndef EXEC_PORTS_H
#include "exec/ports.h"
#endif

#ifndef EXEC_LISTS_H
#include "exec/lists.h"
#endif

#ifndef LIBRARIES_DOS_H
#include "libraries/dos.h"
#endif

/**
*
* This header file supplies information needed to interface with the
* particular operating system and C compiler being used.
*
**/

/**
*
* The following definitions specify the particular C compiler being used.
*
*	LATTICE		Lattice C compiler
*
*/
#if !defined(LATTICE)
  #define LATTICE 1
#endif
#define LATTICE_V4 1
/**
*
* The following type definitions take care of the particularly nasty
* machine dependency caused by the unspecified handling of sign extension
* in the C language.  When converting "char" to "int" some compilers
* will extend the sign, while others will not.  Both are correct, and
* the unsuspecting programmer is the loser.  For situations where it
* matters, the new type "byte" is equivalent to "unsigned char".
*
*/
typedef unsigned char byte;


/**
*
* Miscellaneous definitions
*
*/
#define SECSIZ 512		/* disk sector size */

/**
*
* The following symbols define the sizes of file names and node names.
*
*/
#define FNSIZE 32	/* maximum file node size */
#define FMSIZE 64	/* maximum file name size */
#define FESIZE 32	/* maximum file extension size */

/**
*
* This structure contains disk size information returned by the getdfs
* function.
*/
#define DISKINFO InfoData

/**
*
* The following structure is used by the dfind and dnext functions to
* hold file information.
*
*/
#define FILEINFO FileInfoBlock

/**
*
* The following structure appears at the beginning (low address) of
* each free memory block.
*
*/
struct MELT
	{
	struct MELT *fwd;	/* points to next free block */
	long size;		/* number of MELTs in this block */
	};
#define MELTSIZE sizeof(struct MELT)

/**
*
* The following structure is used to keep track of currently allocated
* system memory
*
*/
struct MELT2
	{
	struct MELT2 *fwd;	/* points to next block */
	struct MELT2 *bwd;	/* points to previous block */
	unsigned size;		/* size of this block */
	};
#define MELT2SIZE sizeof(struct MELT2)

/**
*
* The following structures are used with the AmigaDOS fork() and wait()
* functions
*
*/

struct ProcID {				/* packet returned from fork()  */
	struct ProcID *nextID;		/* link to next packet		*/
	struct Process *process;	/* process ID of child		*/
	int UserPortFlag;
	struct MsgPort *parent;		/* termination msg destination	*/
	struct MsgPort *child;		/* child process' task msg port	*/
	BPTR seglist;			/* child process' segment list	*/
	};

struct FORKENV {
	long priority;			/* new process priority		*/
	long stack;			/* stack size for new process	*/
	BPTR std_in;			/* stdin for new process	*/
	BPTR std_out;			/* stdout for new process	*/
	BPTR console;			/* console window for new process */
	struct MsgPort *msgport;	/* msg port to receive termination */
	};				/* message from child		*/

struct TermMsg : public Message {	/* termination message from child */
	long msgclass;			/* class == 0			*/
	short type;			/* message type == 0		*/
	struct Process *process;	/* process ID of sending task	*/
	long ret;			/* return value			*/
	};

#ifndef __ARGS
#ifdef NARGS
#define __ARGS(a) ()
#else
#define __ARGS(a) a
#endif
#endif

extern int forkl __ARGS((char *, char *, ...));
extern int forkv __ARGS((char *, char **, struct FORKENV *, struct ProcID *));
extern int wait __ARGS((struct ProcID *));
extern struct ProcID *waitm __ARGS((struct ProcID **));

/**
*
* Level 0 I/O services
*
**/
extern int _dclose __ARGS((long));
extern long _dcreat __ARGS((const char *, int));
extern long _dcreatx __ARGS((const char *, int));
extern int dfind __ARGS((struct FILEINFO *, const char *, int));
extern int dnext __ARGS((struct FILEINFO *));
extern long _dopen __ARGS((const char *, int));
extern unsigned _dread __ARGS((long, char *, unsigned));
extern long _dseek __ARGS((long, long, int));
extern unsigned dwrite __ARGS((long, const char *, unsigned));
extern int getcd __ARGS((int,char *));
extern int getdfs __ARGS((const char *, struct DISKINFO *));
extern int getfa __ARGS((const char *));
extern long getft __ARGS((const char *));
/**
*
* Miscellaneous external definitions
*
*/
typedef void *_PV;
typedef void (*_PF)(_PV);

extern int chgclk __ARGS((unsigned char *));
extern int getclk __ARGS((unsigned char *));
extern int onbreak (_PF);
extern void onerror __ARGS((int));
extern int poserr __ARGS((char *));
