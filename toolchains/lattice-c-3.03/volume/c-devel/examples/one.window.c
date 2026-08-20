/* "SIMPLE" demo showing the use of the Intuition windowing system.

Opens a graphics-window, that is also a console window. When you
click in the window, it echos the characters that you type.
Pressing the mouse buttons causes mouse events to be reported.
Pressing <RETURN> will display both a carriage return and a line
feed.
If you uncomment the line "RAWKEY | Then RAWKEY events will be
reported instead of having characters echo.

The CLOSEWINDOW gadget that causes a program exit.
Note that you would generally have either an IDCMP or a Console
attached to a window, but not necessarily both.  (Or if both,
then that window won't usually ask for RAWKEYS, to allow the
console to translate them for normal reporting.)

No menu stuff is included, but the event handling is included
for menu picks within the HandleEvent() function.

Additionally, shows how to trap the right hand mouse button
events for any given active window.

Author:      Rob Peck, 12/18/85
*/

/* DEFINES ********************************************************** */

#define HOWMUCHTIME 40000

#define SECONDS io_Actual
#define MICROSECONDS io_Length

#define WINDOWGADGETS (WINDOWSIZING|WINDOWDRAG|WINDOWDEPTH|WINDOWCLOSE)

#define INTUITION_MESSAGE (1<<intuitionMsgBit)
#define TYPED_CHARACTER (1<<consoleReadBit)
#define TIME_OUT (1<<timerMsgBit)

#define CloseConsole(x) CloseDevice(x)

/* INCLUDES ********************************************************** */

#include "exec/types.h"
#include "exec/io.h"
#include "exec/memory.h"

#include "graphics/gfx.h"
#include "hardware/dmabits.h"
#include "hardware/custom.h"
#include "hardware/blit.h"
#include "graphics/gfxmacros.h"
#include "graphics/copper.h"
#include "graphics/view.h"
#include "graphics/gels.h"
#include "graphics/regions.h"
#include "graphics/clip.h"
#include "exec/exec.h"
#include "graphics/text.h"
#include "graphics/gfxbase.h"

#include "devices/console.h"
#include "devices/keymap.h"
#include "devices/timer.h"

#include "libraries/dos.h"
#include "graphics/text.h"
#include "libraries/diskfont.h"
#include "intuition/intuition.h"
 
/* EXTERNALS ***************************************************** */

extern struct Window *OpenWindow();
extern struct Screen *OpenScreen();
extern struct MsgPort *CreatePort();
extern struct IOStdReq *CreateStdIO();

/* GLOBALS ****************************************************** */

USHORT class;
USHORT code;
USHORT qualifier;
USHORT mode;
APTR address;           /* address of the gadget which we hit */
SHORT xx=0;
SHORT yy=0;
SHORT xdir = 2;
SHORT ydir = 0;
SHORT xsize,ysize;

long IntuitionBase=0;
long GfxBase=0;

struct NewWindow nwGraphics = {
      20, 20,           /* start position       */
      480, 160,         /* width, height        */
      1, 2,             /* detail pen, block pen      */
      GADGETUP | 
      GADGETDOWN | 
      MOUSEBUTTONS | 
      MENUPICK | 
/*    RAWKEY |      */
/* --------- NOTE ------------------------------------

   If you ask for rawkeys for the IDCMP in a window to
   which a console is attached, you will NOT get any
   translated ordinary console keys from the console
   device.  The IDCMP traps all keystrokes, both down
   and up and reports them as IDCMP RAWKEY events.
   The console device never gets to see any of them.
   Complile this program with this RAWKEY item (above)
   uncommented and you will get RAWKEY events instead
   of ordinary console keys echoed to the window.

   ----------------------------------------------------- */
      CLOSEWINDOW, 
                        /* IDCMP flags                */
      RMBTRAP | GIMMEZEROZERO | WINDOWGADGETS,
                        /* window flags               */
      NULL,             /* pointer to first user gadget */
      NULL,             /* pointer to user checkmark  */ 
      "Graphics Window",      /* window title         */ 
      NULL,             /* pointer to screen    (later) */
      NULL,             /* pointer to superbitmap     */
      50,40,200,100,          /* sizing limits min and max */
      WBENCHSCREEN            /* type of screen in which to open */ 
      };

/* RMBTRAP says that when this window is active, there is no menu to
 be rendered.... so report all right mouse button transitions to 
 the user if MOUSEBUTTONS is specified for the IDCMP (and it is) */

struct IOStdReq *consoleWriteMsg;   /* I/O request block pointer */
struct MsgPort *consoleWritePort;   /* a port at which to receive */
struct IOStdReq *consoleReadMsg;    /* I/O request block pointer */
struct MsgPort *consoleReadPort;    /* a port at which to receive */

char tohex(e) 
      char e;
{
      return( (char)( e < 10 ? (e+'0') : (e-10+'A') ) );
}

/* NULL-terminated string to send to console to reset scrolling
 region for the console to 80 lines from the top, leave some
 room for the graphics rendering */

char topoffset[] = { 0x1b, '[',       '7', '4', 'y',  0 };
                   /* --CSI-- */
char linesonpage[] = { 0x9b,    '8',          't',  0 };
/* Reduce lines per page to 8 so we don't scroll off the bottom */

main()
{
      char letter;            /* one letter at a time from console */

      int problem,error;
      int wakeupmask,timerMsgBit;
 
      struct Window *wG;
      struct RastPort *rpG;
 
      struct IntuiMessage *message; /* the message the IDCMP sends us */
 
      int consoleReadBit, intuitionMsgBit;
/* integers that represent the signal bit assigned for the
 console and for intuition's IDCMP on which one waits 
 for some input during a Wait() function call */

      struct MsgPort *timerport;          /* for timer communications */
      struct IOStdReq *timermsg;          /* for timer */
 
      GfxBase = OpenLibrary("graphics.library", 0);
      if (GfxBase == NULL)
      {
            printf("graphics library open failed\n");
            goto cleanup1;
      }
      IntuitionBase = OpenLibrary("intuition.library", 0);
      if (IntuitionBase == NULL)
      {
            printf("intuition library open failed\n");
            goto cleanup1;
      }

      timerport = CreatePort(0,0);
      if (timerport == 0) goto cleanup1;
      timermsg = CreateStdIO(timerport);
      if (timermsg == 0) goto cleanup1a;

      wG = OpenWindow(&nwGraphics); /* open a window for graphics*/
      if ( wG == NULL )
      {
            printf ("open window failed\n");
            goto cleanup2;
      }

      rpG = wG->RPort;  /* set a rastport pointer for both windows */

      /* create ports and messages for communicating with the console */

      consoleWritePort = CreatePort("my.con.write",0);
      if(consoleWritePort == 0)
            { problem = 5; goto cleanup5; }
      consoleWriteMsg =  CreateStdIO(consoleWritePort);
      if(consoleWritePort == 0)
            { problem = 6; goto cleanup6; }

      consoleReadPort = CreatePort("my.con.read",0);
      if(consoleReadPort == 0)
            { problem = 7; goto cleanup7; }
      consoleReadMsg =  CreateStdIO(consoleReadPort);
      if(consoleReadPort == 0)
            { problem = 8; goto cleanup8; }

      /* now attach a console to the window provided for its use */

      error = OpenConsole(consoleWriteMsg,consoleReadMsg,wG);
      if(error != 0)
            { problem = 10; goto cleanup10; }
/* attach a console to this window, initialize for both write and read */

      /* Initialize the timer */
      if (init_timer(timermsg) != 0) goto cleanup10;

      /* ask for the first timing interval */
      /* 40,000 microseconds = .04 sec */
      set_timer(timermsg,0,HOWMUCHTIME);
 
      /* tell console where to put a character that 
       it wants to give me queue up first read */

      QueueRead(consoleReadMsg,&letter); 

      /* reset the top offset to leave room for graphics */
      ConPutStr(consoleWriteMsg,&topoffset[0]);
      /* reset the number of lines on the page for text */
      ConPutStr(consoleWriteMsg,&linesonpage[0]);

      ConPutStr(consoleWriteMsg,"\fHello, World\n");
      ConPutStr(consoleWriteMsg,"I will echo what\n you type\n");
      ConPutStr(consoleWriteMsg,"Hit close gadget\n to exit\n");
 
/* find out which signals to wait for.... Intuition allocates a signal bit
 for an IDCMP and the call to OpenConsole allocates a different bit */

      consoleReadBit = consoleReadPort->mp_SigBit;
      intuitionMsgBit = wG->UserPort->mp_SigBit;
      timerMsgBit = timerport->mp_SigBit;

/* This code assumes that the only events we expect to wake up for are
 characters typed to the console or messages from intuition arriving at
 the IDCMP */

/*  Wait for a CLOSEWINDOW or other event from intuition as well as
  waiting for a typed character */

      do 
      {
            wakeupmask = Wait( INTUITION_MESSAGE | 
                          TYPED_CHARACTER |
                          TIME_OUT );

/* if an intuition message, copy the contents of the
 message to some global variables (our own local 
 copy of the message) and reply immediately */

            if(wakeupmask & INTUITION_MESSAGE)
            {
                  while( (message = (struct IntuiMessage *)
                         GetMsg(wG->UserPort) ) != NULL)
                  {
                        /* EMPTY THE PORT!!!! */
                        /* make a local copy of the 
                         message before replying, and
                         then try to handle the event */
                      class = message->Class;
                      code =      message->Code;
                      qualifier = message->Qualifier;
                      address = message->IAddress;
                      ReplyMsg(message);
                      if(HandleEvent() == FALSE) break;
                  }
            }
            if(wakeupmask & TYPED_CHARACTER)
            {
            /* Task woke up because a message arrived
             at this port, so we must remove the 
             message from the port to allow its reuse */

                  GetMsg(consoleReadPort);

            /* Console character was read into 'letter' */

                  ConPutChar(consoleWriteMsg,letter);
                  if(letter == '\r')
                        ConPutChar(consoleWriteMsg,'\n');
            /* added a line feed to a carriage return */

            /* Now that have typed the character, must send off
             another read request */ 
                  QueueRead(consoleReadMsg, &letter);
            }
            if(wakeupmask & TIME_OUT)
            {
            /* Task woke up because of a timeout;  that is, the timer
             completed and returned its message to the reply port */
                  GetMsg(timerport);
 
                  drawSomething(wG);
                  set_timer(timermsg,0,HOWMUCHTIME); 
            }

      } while (HandleEvent());      /* keep going as long as 
                               * HandleEvent returns nonzero */
      problem = 0;

      AbortIO(consoleReadMsg);      /* cancel the last queued read */

      CloseConsole(consoleWriteMsg);

   cleanup10:
   cleanup8:
      DeleteStdIO(consoleReadMsg);
   cleanup7:
      DeletePort(consoleReadPort);
   cleanup6:
      DeleteStdIO(consoleWriteMsg);
   cleanup5:
      DeletePort(consoleWritePort);
   cleanup3:
      CloseWindow(wG);
   cleanup2:
      if (GfxBase != NULL) CloseLibrary(GfxBase);

      AbortIO(timermsg);

      DeleteStdIO(timermsg);
   cleanup1a:
      DeletePort(timerport);
   cleanup1:
      if (IntuitionBase != NULL) CloseLibrary(IntuitionBase);
      if(problem > 0) 
            exit(problem+1000);
      else
            return(0);
}

drawSomething(w)
struct Window *w;
{
      struct RastPort *rp;
      SHORT xsize, ysize;
      rp = w->RPort;
      xsize = w->Width;       /* use current size limits for the window */
      ysize = (w->Height)/2;         /* draw only in top half of window */
      SetAPen(rp,RangeRand(4));    /* can even draw in background color */
      Move(rp, RangeRand(xsize), RangeRand(ysize));
      Draw(rp, RangeRand(xsize), RangeRand(ysize));
      return(0);
}

HandleEvent()
{
      switch(class) 
      {
            case CLOSEWINDOW:
                  return(FALSE);    /* cleanup and exit if close
                                     * gadget is hit */
                  break;
            /* could have provided processing for each of these */
            case RAWKEY:
                  ConPutStr(consoleWriteMsg,"\nRAWKEY event:\n");
                  ConPutStr(consoleWriteMsg,"value = ");
                  ConPutChar(consoleWriteMsg,
                              (tohex((code & 0xf0)>>4)));
                  ConPutChar(consoleWriteMsg,(tohex(code & 0x0f)));
                  break;
            case GADGETUP:
                  break;      /* event not in use */
            case GADGETDOWN:
                  break;      /* event not in use */
            case MOUSEBUTTONS:
                 ConPutStr(consoleWriteMsg,"\nMOUSEBUTTONS event=");
                 switch(code)
                 {
                  case SELECTDOWN:
                    ConPutStr(consoleWriteMsg,"\nSELECTDOWN\n");
                        break;
                  case SELECTUP:
                    ConPutStr(consoleWriteMsg,"\nSELECTUP\n");
                        break;
                  case MENUDOWN:
                    ConPutStr(consoleWriteMsg,
                              "\nMENUDOWN\n");
                        break;
                  case MENUUP:
                    ConPutStr(consoleWriteMsg,
                              "\nMENUUP\n");
                        break;
                 }
      }
      class = 0; code = 0; qualifier = 0; address = 0;
      return(TRUE);
}


/* These functions are taken directly from the console.device chapter
 * in the Amiga V1.1 ROM KERNEL manual.  See manual for documentation. */


/* Open a console device */
      int
OpenConsole(writerequest,readrequest,window)
      struct IOStdReq *writerequest;
      struct IOStdReq *readrequest;
      struct Window *window;
      {
            int error; 
            writerequest->io_Data = (APTR) window;
            writerequest->io_Length = sizeof(*window);
            error = OpenDevice("console.device", 0, writerequest, 0);
            readrequest->io_Device = writerequest->io_Device;
            readrequest->io_Unit   = writerequest->io_Unit;
                  /* clone required parts of the request */
            return(error);
      }

/* Output a single character to a specified console */ 

      int
ConPutChar(request,character)
      struct IOStdReq *request;
      char character;
      {
            request->io_Command = CMD_WRITE;
            request->io_Data = (APTR)&character;
            request->io_Length = 1;
            DoIO(request);
            /* caution: read comments in manual! */
            return(0);
      }
 
/* Output a NULL-terminated string of characters to a console */ 

      int
ConPutStr(request,string)
      struct IOStdReq *request;
      char *string;
      {
            request->io_Command = CMD_WRITE;
            request->io_Data = (APTR)string;
            request->io_Length = -1;
            DoIO(request);
            return(0);
      }
 
      /* queue up a read request to a console */

      int
QueueRead(request,whereto)
      struct IOStdReq *request;
      char *whereto;
      {
            request->io_Command = CMD_READ;
            request->io_Data = (APTR)whereto;
            request->io_Length = 1;
            SendIO(request);
            return(0);
      }

 
      int 
init_timer(tmsg)  /* return an error code if timer did not open */
{
      return( OpenDevice(TIMERNAME,UNIT_VBLANK,tmsg,0));
}


      int
set_timer(tmsg,sec,micro)
ULONG sec,micro;
struct IOStdReq *tmsg;
{
      tmsg->io_Command = TR_ADDREQUEST;    /* add a new timer request */
      tmsg->SECONDS = sec;        /* seconds */
      tmsg->MICROSECONDS = micro; /* microseconds */
      SendIO( tmsg );             /* post a request to the timer */
      return(0);
}

