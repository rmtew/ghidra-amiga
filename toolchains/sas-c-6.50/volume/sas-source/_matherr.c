/***
*
* name             __matherr -- math error handler
*
* synopsis         action = __matherr(x);
*                  int action;             non-zero if new value supplied
*                  struct __exception *x;
*
* description      This function is called by functions in the math library
*                  when an error occurs.  The exception vector contains
*                  information about the function that encountered the
*                  error, including the error type, function name, first
*                  two arguments, and proposed default value.
*
*                  Normally, matherr translates the error type into a code
*                  that is placed into "errno".  Then, matherr signals
*                  the caller to simply use the proposed default.  Other
*                  actions are possible if the user replaces or enhances
*                  this function with application-specific code.
***/

#include <errno.h>
#include <math.h>



__matherr(x)
    struct __exception *x;
{
    switch(x->type) {
        case DOMAIN:
        case SING:
            errno = EDOM;
            break;

        default:
            errno = ERANGE;
    }

    return(0);
}
