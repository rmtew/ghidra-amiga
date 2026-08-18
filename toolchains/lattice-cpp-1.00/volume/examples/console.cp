// Console - Use of the Amiga Console Device

//	If you've ever tried coding a program in C to use the Console
//	Device, you know it's no easy task.
//
//	Except in C++.
//

#include <stream.h>
#include <exec/types.h>
#include <devices/console.h>
#include <intuition/intuition.h>
#define LATTICE_ASSIGN_BUG /* structure assignment bug */

extern void Delay (ULONG);	// from libraries/dos.h

static const NewWindow mynw = {
	10,10,240,100,
	3, 1,		// pens
	IDCMPFlags(0),	// IDCMPflags
	0,		// flags
	NULL,		// first gadget
	NULL,		// checkmark
	"Console Device",
	NULL,		// screen
	NULL,		// bitmap
	0,0,0,0,	// size limits
	WBENCHSCREEN	// screen type
};

#define MESSAGE "Hi World!\n"

Window *win;
Console *console;

void
go_away (UWORD return_code)
{
	if (console != NULL) delete console;
	if (win != NULL) delete win;
	exit (return_code);
}

main()
{
	win = new Window(&mynw);
	if (win == NULL)
	{
		Gripe ("Window didn't open");
		go_away(20);
	}

	console = new Console(win);
	if(console->error())
	{
		Gripe ("Open Console failed.");
		go_away(20);
	}

	console->write(MESSAGE, strlen(MESSAGE));

	Delay(500);
	go_away (0);
}
