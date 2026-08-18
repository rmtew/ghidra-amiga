/***
*
*          Copyright © 1989  Lattice, Inc.
*
* name             _CXFERR -- low-level floating point error trap 
*
* synopsis         _CXFERR(code);
*                  int code;       error code (see math.h)
*
* description      This function is called when an error is detected by
*                  one of the low-level floating point routines, such as
*                  the arithmetic operations.  Higher-level routines such
*                  as the transcendental functions, use the more sophisti-
*                  cated "matherr" trap.
*
***/

#include <signal.h>

void __stdargs _CXFERR(int);


extern int _FPERR;



void __stdargs _CXFERR(code)
    int code;
{
    _FPERR = code;

    raise(SIGFPE);
}
