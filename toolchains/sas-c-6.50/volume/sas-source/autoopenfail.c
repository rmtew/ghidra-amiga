#include <proto/dos.h>
#include <proto/exec.h>
#include <string.h>

extern struct WBStartup *_WBenchMsg;
extern char __stdiowin[];
extern long __oslibversion;

void __regargs __autoopenfail(char *lib)
{
   struct DOSBase *DOSBase;
   long fh;
   char numbers[4];
   unsigned short temp;  /* declared short so it doesn't use a cx routine to do the divide */
   
   DOSBase = (struct DOSBase *)OpenLibrary("dos.library",0);
   fh = Output();
   if (fh == NULL)
      fh = Open(__stdiowin, MODE_NEWFILE);
   
   Write(fh, "Can't open version ", 20);
   temp = __oslibversion;
   numbers[0] = temp >= 100 ? '0' + temp/100 : ' ';
   temp = temp % 100;
   numbers[1] = __oslibversion >= 10 ?  '0' + temp/10  : ' ';
   temp = temp % 10;
   numbers[2] = '0' + temp;

   Write(fh, numbers, 3);
   Write(fh, " of \"", 5);
   Write(fh, lib, strlen(lib));
   Write(fh, "\"\n",2);
   
   if (_WBenchMsg)
   {
      Delay(200);
      Close(fh);
   }   
   
   CloseLibrary((struct Library *)DOSBase);
}
