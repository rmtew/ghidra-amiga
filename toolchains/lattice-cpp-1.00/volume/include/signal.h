/**
*
* This header file contains definitions needed by the signal function.
*
**/

/**
*
* NSIG supposedly defines the number of signals recognized.  However,
* since not all signals are actually implemented under AmigaDOS, it actually
* is the highest legal signal number plus one.
*
*/
#define NSIG 9		
			
/**
*
* The following symbols are the defined signals.
*
*/
#define SIGINT 2	/* console interrupt */
#define SIGFPE 8	/* floating point exception */

/**
* The following symbols are the special forms for the function pointer
* argument.  They specify certain standard actions that can be performed
* when the signal occurs.
*
*/
#define SIG_DFL (int (*)())0	/* default action */
#define SIG_IGN (int (*)())1	/* ignore the signal */

#define SIG_ERR (int (*)())(-1)	/* error return */
/**
*
* Function declarations
*
*/
//int (*signal)(const int snum, ... );

/**
*
* Define NULL if it's not already defined
*
*/
#ifndef NULL
#define NULL 0			/* null pointer value */
#endif
