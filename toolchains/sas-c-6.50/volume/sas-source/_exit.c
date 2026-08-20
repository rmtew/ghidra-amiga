/***
*
*          Copyright © 1992 SAS Institute, Inc.
*
* name             __exit -- standard exit function from C program
*
* synopsis         __exit(errcode);
*                  int errcode;          exit error code
*
* description      This function provides a standard exit point from a
*                  C program.  Control is returned to the operating
*                  system under which the program is being executed.
*                  The errcode parameter is sent to the system and has
*                  the following meanings assigned:
*
*                       0 = Normal termination
*                       5 = Warning
*                      10 = Error
*                      20 = Fatal Error
*
***/

#include <stdlib.h>
#include <dos.h>

void __exit(errcode)
    int errcode;
{
#ifdef NOBASER
/* this allows DATA=FARONLY functions to call exit, or __exit, */
/* but not XCEXIT. */
     __builtin_geta4();
#endif

    _XCEXIT((long) errcode);           /*  Call exit in startup code.  */
}
