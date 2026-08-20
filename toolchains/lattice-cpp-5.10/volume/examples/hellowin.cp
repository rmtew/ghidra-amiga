// Hellowin.cp

// Hello, World in an Amiga window

#include <exec/types.h>
#include <Intuition/intuition.h>
#include <libraries/dos.h>	// For Delay
#include <stream.h>

const NewWindow nw = {
	10, 10, 300, 50,
	0,1,		// detail and block pens
	0,	 	// IDCMP flags
	0,		// other window flags
	NULL,		/* first gadget in gadget list */
	NULL,		/* custom CHECKMARK imagery */
	"C++",		/* window title */
	NULL,		/* custom screen pointer */
	NULL,		/* custom bitmap */
	0,0,0,0,	// Minimum and maximum width and heigh
	WBENCHSCREEN	// destination screen type
};

main()
{
	Window *hi = new Window(&nw);

	if (hi == NULL)
	{
		cerr << "Error: I couldn't open the window!\n";
		exit(20);
	}

	RastPort *ho = hi->graphic();
	ho->move(20, 20);
	*ho << "Hello, World!";
	Delay (50 * 5);	// 5 seconds
	delete hi;
}
