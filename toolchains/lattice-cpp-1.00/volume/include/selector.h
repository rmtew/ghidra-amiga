// selector.h

#ifndef SELECTOR_H
#define SELECTOR_H 1

#include <exec/types.h>
#ifndef INTUITION_H
#include <exec/nodes.h>
#include <graphics/rastport.h>
#include <Intuition/intuition.h>
#endif

/** define a class for scrollable selection requester **/

typedef unsigned short SlotNumber;

class Selector {
	RastPort *rp;		// Rastport of selector
	Window	*sel_w;		// Window of same
	Gadget	*select_gadget;
	ULONG   prev_secs, prev_mics;
	LONGBITS ghosted_slots;	// slots corresponding to deleted nodes
	SlotNumber	slot_count, current_slot, slots_displayed;
	USHORT	min_y, max_y, slot_height;
	SHORT	ticker;

	List	*node_list;
	Node	*current_node;
	USHORT	node_count, current_node_number;

	void complement();
	BOOL is_ghosted(SlotNumber n) { return (ghosted_slots >> n) & 1; }
	SlotNumber displayed_at(Node *n);
	void ghost(SlotNumber n);
	int real_slots();
	void point_to (USHORT);
	SlotNumber get_slot_number(USHORT);
	void highlight(SlotNumber);
	Node *seek_node(Node *, SHORT);
	Node *node_displayed(SlotNumber);

	void calc_pot();
	void ghost_everything();

public:
	Selector (Window *w, register Gadget *xsg, List &xl);
	void display_nodes();

	void start(USHORT);
	void end_action(USHORT, BOOL);
	void mouse_move(USHORT);
	void scroll_set(int);
	void Intuitick();

	Node *clicked_on();	// return pointer if double-clicked
	Node *selected_node()	// return pointer to highlighed node if any
		{ return node_displayed(current_slot); }

	virtual void render(RastPort *, Node *);
	virtual void adjust_scrollbar(Gadget *bar, int pot, int body);

	void append(Node *);	// scrollbar selector
	void remove(Node *);

//
// Scrolling.  Amiga conventions (raw keycodes) are:
//
// up-arrow 	- CURSORUP	- scroll(SELECT_UPWARDS);
// down-arrow	- CURSORDOWN	- scroll(SELECT_DOWNWARDS);
// shift+up	- CURSORUP,  IEQUALIFIER_LSHIFT | IEQUALIFIER+_RSHIFT
//			- scroll_page(SELECT_UP)
// shift+down	- can't you guess?
// ALT+up	- CURSORUP, ALTLEFT | ALTRIGHT - scroll_to(SELECT_TOP)
// ALT+down	- CURSORDOWN,  ""		- scroll_to(SELECT_BOTTOM)

	enum scroll_dir { SELECT_DOWNWARDS, SELECT_UPWARDS };
	void scroll(enum scroll_dir);		// 1 line
	void scroll_page(enum scroll_dir);	// display frame
#define SELECT_TOP 0
#define SELECT_BOTTOM 65535
	void scroll_to(int);			// Absolute position.
};

const int NO_SLOT = 4000;

#endif
