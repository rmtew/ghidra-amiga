#ifdef M881
#include <dos.h>
#endif
#include <stdio.h>
#include <proto/exec.h>

#ifdef FFP
extern struct Library *MathBase;
extern struct Library *MathTransBase;
#else
#ifdef IEEE
extern struct Library *MathIeeeDoubBasBase;
extern struct Library *MathIeeeDoubTransBase;
#endif
#endif

extern int (* far __ctors[])(void);
extern void (* far __dtors[])(void);


int __stdargs __fpinit(void)
{
   void * volatile p = &__ctors;  /* so GO won't eliminate tst */   
   /* call all the autoinit routines */
   if (p)
   {
       if ((*__ctors[-1])()) /* this function calls all the others */
          return 1;
   }

#ifdef M881
    __emit(0xf23c);        /* put out the instruction FMOVE.L #$00,FPCR */
    __emit(0x9000);        /* This is Round toward nearest and use      */
    __emit(0x0000);        /* Extended precision.                       */
    __emit(0x0000);
#endif
   return 0;
}




void __stdargs __fpterm(void)
{
   void * volatile p = &__dtors;  /* so GO won't eliminate tst */   
   /* call all the auto close routines */
   if (p)
   {
       (*__ctors[-1])(); /* this function calls all the others */
   }

}
