// Flowplot.cp - plot flowchart symbols

#include <exec/types.h>
#include <intuition/intuition.h>
#include <stream.h>

#define nl "\n";

Window *win;

void pad (char *xto, const char *xfrom, int len)
{
	while (*xfrom && (len > 0))
	{
		*xto++ = *xfrom++;
		len--;
	}
	while (len-- > 0)
	{
		*xto++ = ' ';
	}
}

struct ploc {
	USHORT row, col, page;
public:
	ploc(USHORT xr, USHORT xc, USHORT xp)
	{ row = xr; col = xc; page = xp; }
	ploc(ploc& pp) { *this = pp; }
protected:
	void movedraw();

};

class graphic : public ploc {
public:
	graphic(ploc& pp) : (pp) { }
	virtual void draw();
};

void
graphic::draw()
{
	cerr << "ERROR: A drawing routine is missing.\n";
}

void
ploc::movedraw()	// Move to coordinates of box
{
	win->RPort->move(col*100, row * 60);
}

class iographic : public graphic {
	char text1[10];
	char text2[10];
	char text3[10];
	char text4[10];
public:
	iographic(ploc p,const char *l1, const char *l2,
		const char *l3, const char *l4);
	void draw();
};

iographic::iographic(ploc p,const char *l1, const char *l2,
		const char *l3, const char *l4) : (p)
{
	pad (text1, l1, 10);
	pad (text2, l2, 10);
	pad (text3, l3, 10);
	pad (text4, l4, 10);
}

void
iographic::draw()
{
	movedraw();
	register RastPort *rp = win->RPort;
	Point basis; basis = *rp;

	rp->move (basis.x + 20, basis.y);
	rp->line (basis.x + 20 + 100, basis.y);
	rp->line (basis.x - 20 + 100, basis.y+50);
	rp->line (basis.x - 20, basis.y+50);
	rp->line (basis.x + 20, basis.y);

	rp->move(basis.x + 20, basis.y + 12);
	Text (rp, text1, 10);
	rp->delta( -88, 11);
	Text (rp, text2, 10);
	rp->delta( -88, 11);
	Text (rp, text3, 10);
	rp->delta( -88, 11);
	Text (rp, text4, 10);
}

NewWindow nw = {
	0, 0, 640, 200,
	1, 3,		// pens
	CLOSEWINDOW,	// IDCMPFlags
	WINDOWCLOSE,	// flags
	NULL, NULL, "Flowchart Window", NULL, NULL,
	0, 0, 0, 0, WBENCHSCREEN
};

main()
{
	win = new Window(&nw);

	if (win == NULL)
	{
		cerr << "Couldn't open window.\n";
		exit(20);
	}

	ploc here (1, 2, 1);
	iographic zazz (here, "I/O Test", "Symbol", "", "");

	zazz.draw();

	here = ploc (2, 2, 1);
	iographic zooz (here, "Another", "Symbol", "", "");

	zooz.draw();

	register short KeepGoing = TRUE;
	struct IntuiMessage *message;
	register short MessageClass, code;
//	struct Gadget *igad;
//	int gadgid, last_gadget = -1;

   while (KeepGoing)
   {
      Wait (1 << win->UserPort->mp_SigBit);
      while (message = (struct IntuiMessage *)GetMsg(win->UserPort))  {
        MessageClass = message->Class;
        code = message->Code;
        message->reply();

        switch (MessageClass)
	{
	case CLOSEWINDOW : /* don't count on it! */
		KeepGoing = FALSE;
        }   /* Case */
    } /* while */
    } /* also while */
    delete win;

}
