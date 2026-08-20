/***
*
*          Copyright © 1992  SAS Institute, Inc.
*
* name             _CXBRK  -  default signal handler for SIGINT
*
* synopsis         _CXBRK(sig)
*                  sig             signal which caused the abort request
*
*
* description      _CXBRK is the default signal handler used to handle
*                  the SIGINT signal.  It puts up a requester asking
*                  the user if he really wants to abort program execution.
*                  If yes, this routine aborts the program, otherwise the
*                  routine returns to normal program execution.
*
***/

#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <exec/types.h>
#include <intuition/intuition.h>
#include <libraries/dosextens.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>


void _CXBRK(int);

static struct IntuiText Text2 = {
           (unsigned char)-1, 
           (unsigned char)-1,                /* pen numbers */
           0,                                /* draw mode */
           14, 14,                           /* starting offsets */
           NULL,                             /* text attribute pointer */
           "** User Abort Requested **",
           NULL
};

static struct IntuiText BodyText = {
           (unsigned char)-1, 
           (unsigned char)-1,                /* pen numbers */
           0,                                /* draw mode */
           4, 4,                             /* starting offsets */
           NULL,                             /* text attribute pointer */
           NULL,
           &Text2
};

static struct IntuiText ContinueText = {
           (unsigned char)-1, 
           (unsigned char)-1,                /* pen numbers */
           0,                                /* draw mode */
           4, 4,                             /* starting offsets */
           NULL,                             /* text attribute pointer */
           "CONTINUE",
           NULL
};

static struct IntuiText AbortText = {
           (unsigned char)-1, 
           (unsigned char)-1,                /* pen numbers */
           0,                                /* draw mode */
           4, 4,                             /* starting offsets */
           NULL,                             /* text attribute pointer */
           "ABORT",
           NULL
};

extern char *_ProgramName;


void _CXBRK(sig)
    int  sig;
{
    char   temp[81];
    int    len;
    struct Process *us;
    BPTR   fh;
    struct CommandLineInterface  *cli;
    struct IntuitionBase         *IntuitionBase;


    /*   Build the program name into temp   */

    len = _ProgramName[-1];
    if (len > 79)
        len = 79;
    memcpy(temp, _ProgramName, len);
    temp[len] = '\0';


    /* Now see if we were invoked from CLI or from WorkBench */

    us = (struct Process *) FindTask(0L);
    if (us->pr_CLI != NULL) {
        cli = (struct CommandLineInterface *) (((long) (us->pr_CLI)) << 2);
        fh = cli->cli_StandardOutput;
        if (fh == NULL)
            fh = us->pr_COS;
        if (fh != NULL) {
            Write(fh, "*** Break: ", 11L);
            temp[len++] = '\n';
            Write(fh, temp, (long) len);
            __sigfunc[SIGINT] = SIG_IGN;
            __exit(20);
	}
    }

    IntuitionBase = (struct IntuitionBase *)
                           OpenLibrary("intuition.library",0);
    if (IntuitionBase == NULL) {
        __sigfunc[SIGINT] = SIG_IGN;
        __exit(20);
    }

    BodyText.IText = temp;

    if (AutoRequest(NULL, &BodyText, &ContinueText, &AbortText,
                                  0L, 0L, 250L, 60L) != TRUE) {
        __sigfunc[SIGINT] = SIG_IGN;
        __exit(20);
    }

    __sigfunc[sig] = _CXBRK;

    return;
}
