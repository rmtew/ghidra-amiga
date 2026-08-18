#ifndef RESOURCES_MISC_I
#define RESOURCES_MISC_I







#ifndef EXEC_TYPES_H
#include "exec/types.h" 
#endif   !EXEC_TYPES_H

#ifndef EXEC_LIBRARIES_H
#include "exec/libraries.h" 
#endif   !EXEC_LIBRARIES_H




#define  MR_SERIALPORT     0
#define  MR_SERIALBITS     1
#define  MR_PARALLELPORT   2
#define  MR_PARALLELBITS   3

#define NUMMRTYPES         4

struct MiscResource {
    struct Library mr_Library;
    ULONG mr_AllocArray[NUMMRTYPES];
};

#define MR_ALLOCMISCRESOURCE  (LIB_BASE)
#define MR_FREEMISCRESOURCE   (LIB_BASE + LIB_VECSIZE)


#define MISCNAME "misc.resource"

#endif   !RESOURCES_MISC_H
