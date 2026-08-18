// Another Graphics example program
// April 8, 1987 by Tim Holloway


// In this example, we take the shapes as objects

#include <exec/types.h>
#include <graphics/gfx.h>
#include <graphics/rastport.h>
#include <Intuition/intuition.h>
#include <cplus/window.h>

#include <stdio.h>

// Define a derived class so we can add new functions to it.

class graf : public RastPort {
public:
	void wheel();		// draw a wheel
	void wheelbase();	// draw two wheels
};

class circle : Point {
	UWORD	radius;
public:
	void draw();
	circle(const UWORD r) { radius = r; }
};

class box : Point {
	UWORD width, height;
public:
	void draw();
	box(const UWORD w, const UWORD h) { width = w; height = h; }
};

main()
{
	window me(0,0,64,200,"Shapes test");

	if (me.error() )
	{
		printf ("Window didn't open");
		exit(20);
	}

	circle *c1 = new circle(50),(100,100);
	circle *c2 = new circle(100,120),(100);
	box *b1 = new box(20,20), (100,140);

	Wait(me.win);
}
