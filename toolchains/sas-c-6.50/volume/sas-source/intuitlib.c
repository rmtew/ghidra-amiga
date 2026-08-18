void __regargs __autoopenfail(char *);

#include <constructor.h>
#include <proto/exec.h> 
#include <proto/intuition.h>
struct IntuitionBase *IntuitionBase ;
static void *libbase;
extern long __oslibversion;

CBMLIB_CONSTRUCTOR(openintuit)
{
   IntuitionBase = libbase = 
       (void *)OpenLibrary("intuition.library", __oslibversion);
   if (IntuitionBase == NULL)
   {
     __autoopenfail("intuition.library");
     return 1;
   }
   return 0;
}

CBMLIB_DESTRUCTOR(closeintuit)
{
   if (libbase)
   {
      CloseLibrary((struct Library *)libbase);
      libbase = IntuitionBase = NULL;
   }
}
