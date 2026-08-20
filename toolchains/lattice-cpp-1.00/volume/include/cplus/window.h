// cplus/window.h
//
// (C) 1987, Lattice, Inc Associates, Inc.
//
#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif

// definitions for simple windows.

class window {
public:
	Window	*win;
public:
	void new_line();
	int error() { return win == 0; }
	friend window& operator<< (window& w, const TEXT *s);
	friend window& operator<< (window& w, const char c);
	friend window& operator<< (window& w, const UBYTE c);
	friend window& operator<< (window& w, const int i);
	friend window& operator<< (window& w, const LONG i);
	friend window& operator<< (window& w, const double d);

	window(const USHORT left, const USHORT top,
		const USHORT width, const USHORT height,
		const TEXT *title,
		const LONGBITS flags=0, const IDCMPFlags idc=0);
	~window() {CloseWindow(win);}


	void circle(const USHORT r)
		{ win->graphic()->ellipse(r, r); }
	void ellipse(const USHORT r1, const USHORT r2)
		{ win->graphic()->ellipse(r1, r2); }
	void line (const USHORT x1, const USHORT y1)
		{ win->graphic()->line(x1, y1); }

	void move (const USHORT x, const USHORT y) { win->graphic()->move(x,y); }
	void text (const TEXT *s)
		{ (void) win->graphic()->text(s);}
	void draw (class Image * image, const SHORT x, const SHORT y)
		{ DrawImage (win->RPort, image, x, y); }
};


