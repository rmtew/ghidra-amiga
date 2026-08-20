/***
*
*          Copyright © 1992 SAS Institute, Inc.
*
* name             __main - process command line, open files, and call main()
*
* synopsis         __main(line);
*                  char *line;     ptr to command line that caused execution
*
* description      This function performs the standard pre-processing for
*                  the main module of a C program.  It accepts a command
*                  line of the form
*
*                       pgmname arg1 arg2 ...
*
*                  and builds a list of pointers to each argument.  The first
*                  pointer is to the program name.  For some environments, the
*                  standard I/O files are also opened, using file names that
*                  were set up by the OS interface module XCMAIN.
*
***/

#include <stdio.h>
#include <fcntl.h>
#include <ios1.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <workbench/startup.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/execbase.h>

extern struct ExecBase *SysBase;

#define MAXARG 32
#define QUOTE       '"'
#define MAXWINDOW   40
#define ESCAPE '*'
#define ESC '\027'
#define NL '\n'

#define isspace(c)      ((c == ' ')||(c == '\t') || (c == '\n'))


#ifndef TINY
extern int __fmode;
#endif

extern char __stdiowin[];
extern char __stdiov37[];

extern struct WBStartup *_WBenchMsg;
int main(int, void *);

static int argc;                            /* arg count */
static char **targv, *argv[MAXARG+1];       /* arg pointers */




void __stdargs __main(line)
    char *line;
{
    char **pargv;
    char *argbuf;
    int ret;

/***
*     Build argument pointer list
***/
   while (argc < MAXARG)
   {
        while (isspace(*line))  line++;
        if (*line == '\0')      break;
        pargv = &argv[argc++];
        if (*line == QUOTE)
        {
            argbuf = *pargv = ++line;  /* ptr inside quoted string */
            while (*line != QUOTE && *line != 0)
            {
               if (*line == ESCAPE)
               {
                  line++;
                  switch (*line)
                  {
                     case '\0':
                        *argbuf = 0;
                        goto linedone;
                     case 'E':
                        *argbuf++ = ESC;
                        break;
                     case 'N':
                        *argbuf++ = NL;
                        break;
                     default:
                        *argbuf++ = *line;
                  }
                  line++;
               }
               else
               {
                 *argbuf++ = *line++;
               }
            }
            line++;
            *argbuf++ = '\0'; /* terminate arg */
        }
        else            /* non-quoted arg */
        {       
            *pargv = line;
            while ((*line != '\0') && (!isspace(*line))) line++;
            if (*line == '\0')  break;
            else                *line++ = '\0';  /* terminate arg */
        }
   }  /* while */

linedone:

    targv = (argc == 0) ? (char **) _WBenchMsg : (char **) &argv[0];


/***
*     Call user's main program
***/

    ret = main(argc, targv);                /* call main function */

    exit(ret);
}
