// Cplus/TextWindow.h
//
// (C) 1987, MTS Associates
//

// ** Defines TextWindow device I/O

#ifndef EXEC_IO_H
#include <exec/io.h>
#endif

#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif

class TextWindow {
	IOStdReq *req;
	Window	*my_window;
	int	error_code;
	BYTE	own_window;	// TRUE if I built the window.

	void open_channel();
public:
	TextWindow(const UWORD left, const UWORD top,
		const UWORD width, const UWORD height,
		const TEXT *title,
		const LONGBITS flags=0, const IDCMPFlags IDCF=0);

	TextWindow(const Screen *screen,
		const UWORD left, const UWORD top,
		const UWORD width, const UWORD height,
		const TEXT *title,
		const LONGBITS flags=0, const IDCMPFlags IDCF=0);

	TextWindow(Window *w);

	~TextWindow();

	TextWindow& operator <<(const TEXT *t)
	{
	  req->io_Command = CMD_WRITE;
	  req->io_Data = t;
	  req->io_Length = -1;
	  DoIO(req);
	  error_code = req->io_Error;
	  return *this;
	}


// WARNING: BUG in Lattice C version 3.10 - DON'T use the
// following:
//
//	TextWindow& operator>>(register char &c)
//	{
//	  char cb;
//
//	  req->io_Command = CMD_READ;
//	  req->io_Data = &cb;
//	  req->io_Length = 1;
//	  DoIO(req);
//	  error_code = req->io_Error;
//	  c = (req->io_Error) ? '?' : cb;
//	  return *this;
//	}

	unsigned char getc()
	{
	  char cb = '?';

printf ("Setup to getc, req=%x\n", req);
	  req->io_Command = CMD_READ;
	  req->io_Data = &cb;
	  req->io_Length = 1;
printf ("Prepare to getc\n");
	  DoIO(req);
printf ("Got character: %c, error=%x\n", cb, req->io_Error);
	  error_code = req->io_Error;
	  return cb;
	}
	

	TextWindow& operator <<(const char ch)
	{
	  char cb = ch;
	
	  req->io_Command = CMD_WRITE;
	  req->io_Data = &cb;
	  req->io_Length = 1;
	  DoIO(req);
	  error_code = req->io_Error;
	  return *this;
	}

	int error() { return error_code; }
};


//*************************************************************************//


// Definition part:

// #include <stdio.h>

void
TextWindow::open_channel()
{
	if ((req = new IOStdReq) == NULL)
	{error_code = 999; goto fail; }

printf ("Open channel.  This=%x, req=%x, window=%x\n", this, req, my_window);

	req->io_Data = my_window;
	req->io_Length = sizeof(APTR);

	error_code = OpenDevice("console.device", 0, req, 0);
	if(error_code != 0)
	{
		delete req;
		goto fail;
	}

	return;

fail:
	req = NULL;
}

TextWindow::TextWindow(Window *w)
{
	my_window = w;
	own_window = FALSE;
	error_code = 0;
	open_channel();
}

static const NewWindow anw = {	// prototype
	0,0,0,0,
	0, 1,
	IDCMPFlags(0),		// IDCMPflags
	0,		// flags
	NULL,		// first gadget
	NULL,		// checkmark
	"Test Window",
	NULL,		// screen
	NULL,		// bitmap
	0,0,0,0,	// size limits
	WBENCHSCREEN	// screen type
};

TextWindow::TextWindow(const UWORD left, const UWORD top,
	const UWORD width, const UWORD height,
	const TEXT *title,
	const LONGBITS flags=0, const IDCMPFlags IDCF=0)
{
	NewWindow mynw = anw;

	mynw.LeftEdge = left; mynw.TopEdge = top;
	mynw.Width = width; mynw.Height = height;
	mynw.Title = title;
	mynw.Flags = flags;
	mynw.IDCMPFlags = IDCF;
	const Window *const win = new Window(&mynw);
	my_window = win;
	own_window = TRUE;
	open_channel();
}


TextWindow::TextWindow(const Screen *screen,
	const UWORD left, const UWORD top,
	const UWORD width, const UWORD height,
	const TEXT *title,
	const LONGBITS flags=0, const IDCMPFlags IDCF=0)
{
	NewWindow mynw = anw;

	mynw.LeftEdge	= left;  mynw.TopEdge	= top;
	mynw.Width	= width; mynw.Height	= height;
	mynw.Title	= title;
	mynw.Flags	= flags;
	mynw.IDCMPFlags = IDCF;
	mynw.Screen	= screen;
	mynw.Type	= CUSTOMSCREEN;
	const Window *const win = new Window(&mynw);
	my_window = win;
	own_window = TRUE;
	open_channel();
}

TextWindow::~TextWindow()
{
	if (req)
	{
		CloseDevice(req);
		delete req;
	}
	if (own_window)
		delete my_window;
}
