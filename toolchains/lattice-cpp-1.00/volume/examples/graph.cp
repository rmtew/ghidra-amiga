// Graphics example program
// April 7, 1987 by Tim Holloway
//
// This is a simple example of some basic graphics functions in C++.
//

// This program illustrates:
//  A) extending functionality by over-defining of standard types
//       (graf/RastPort),
//  B) portability aspects (the generic window)
//  C) some object-oriented programming.
//
//  We played with the Exec AreaDraw function and found, to our
//  disappointment that it uses GraphPort absolute coordinates.
//  If you'd like to try something a little fancy, define an absolute Polygon
//  object that can be constructed from a Point and an relative Polygon,
//  i.e. A polygon whose component Points are relative to each other
//  instead of to an absolute graphport address:
//
//  AbsPoly = startPoint + RelPoly;
//
//  The AbsPoly would then be used to invoke AreaDraw, and you could
//  copy polygons all over the window by offsetting RelPolys by different
//  point addresses.  Since there can be any number of points in a polygon
//  (Within AreaDraw's defined limits), AbsPoly's and RelPoly's are not
//  fixed sizes.  You may want to refer to section 6.6 in Stroustrup's
//  "The C++ Programming Language".
//
#include <exec/types.h>
#include <graphics/gfx.h>
#include <graphics/rastport.h>
#include <graphics/areas.h>
#include <Intuition/intuition.h>

#include <stdio.h>

extern void Delay (ULONG);	// from libraries/dos.h

// Define a derived class so we can add new functions to it.

class graf : public RastPort {
public:
	void wheel();		// draw a wheel
	void wheelbase();	// draw two wheels
	void fill(const Point p) {Flood(this, 1, p.x, p.y);}
	void fill()
	{
		Point wrkp;
		wrkp = *this;

		this->AreaInfo->init();
		fill(wrkp);
	}

#define polygon(p) poly(sizeof(p)/(2*sizeof(UWORD)), p)
	void poly(const UWORD num, const UWORD* points)
	{
		PolyDraw(this, num, points);
	}
};

// The following two routines are a splendid demonstration of an object
// being inherited.  Much prettier than saying gp->circle(15); etc.

void
graf::wheel()
{
	Point savepoint;
	savepoint = *this; // Point=Rastport - save current location
	circle (15);
	circle (30);
	*this += Point (-20, 0);	// Move into tire

//	fill(); // Do circles leak?
	*this = savepoint;	// Reset position.
}

void
graf::wheelbase()
{
	wheel();
	delta(100, 0);		// i.e move right 100 pixels
	wheel();
}

UWORD a_shape[] = {
	3,3,
	15,3,
	15,15,
	3,15,
	3,3
};

static NewWindow nw = {
	0, 0, 0, 0,
	3, 1,		// pens
	IDCMPFlags(0),	// IDCMPflags
	0,		// flags
	NULL,		// first gadget
	NULL,		// checkmark
	"Graph Window",
	NULL,		// screen
	NULL,		// bitmap
	0,0,0,0,	// size limits
	WBENCHSCREEN	// screen type
};

struct window {
	Window *win;

	window (UWORD left, UWORD top, UWORD width, UWORD height,
		const char *title)
	{ nw.LeftEdge = left; nw.Width = width;
	  nw.TopEdge = top; nw.Height = height;
	  nw.Title = (UBYTE *) title;
	  win = new Window(&nw);
	  if (win == 0)
	  {
		printf ("Error: could not open window\n");
		exit(20);
	  }
	}
	~window() { delete win; }

	// Extract the RastPort address of the window and use it as the
	// "graf" address.  Casting is necessary (bleah!), see Stroustrup,
	// page 197.

	graf *graphic() { return (graf *) win->graphic(); }
};

main()
{
	window me(10,10,240,150,"Graphics Window test");

	graf *gp = me.graphic();

	gp->move(20, 120);	// Initial point is 0,0 (in title bar).
	*gp << "Start with the wheels...";	// caption.

	// Here we define an absolute point at x=50, y=70.
	// This allows a demonstration of how the assignment operator
	// is overlaid so that assigning a Point to a RastPort causes
	// a Move to be performed.

	Point left_rear(50, 70);

	// The following is required for flood-fill:

printf ("Create new AREAINFO\n");
	gp->AreaInfo = new AreaInfo(10);
printf ("Create new ScratchRaster\n");
	gp->TmpRas =  new ScratchRaster (200, 100);
printf ("Done\n");

	*gp = left_rear;	// Notice the indirection. gp is a POINTER.
	gp->wheelbase();	// Draw the wheels.

	gp->polygon(a_shape);	// Just for kicks, but looks kinda tacky.
	Delay(500);		// Hang around to be admired.

printf ("Cleanup AreaInfo\n");
	delete gp->AreaInfo;
printf ("Create new Raster\n");
	delete gp->TmpRas;
}
