// Selector.cp - Generic selector
//
// © 1987, MTS Associates, Inc. - the accompanying code and documentation
// may be used freely by public and commercial users, as long as it is
// not sold for profit and providing this notice remains intact.
//
// This is a definition of a generic sliding-list selector.  It permits
// easily creating and maintaining an area within a window
// display where a list of names is shown. allowing selection by rolling
// the mouse up or down, scrolling the list when items outside the current
// display are desired, and support for an optional scroll bar.
//
// The immense flexibility of this function lies in the fact that unlike
// similar requesters, it is not limited to file lists - any data items
// chained together as a list of Amiga Nodes will do - you simply supply
// the item rendering routine - which can be anything, including routines
// that render graphics as well as text.
//
// Like similar selection list functions, this one divides the display
// into a series of vertically-stacked selection boxes.  However, UNLIKE
// most, they are all a single gadget!  You supply a gadget of the
// appropriate size, and the selection-list routine subdivides it, based
// on the characteristics of the current font of the RastPort supplied.
// There's only one limitation - a maximum of 32 entries are supported,
// because of the mechanism by which I disarm any deleted nodes which are
// currently displayed.  That can be changed by modifying the source code
// slightly, however.

const ROLL_TIME = 5;	// # of IntuiTicks between scrolls at fringes of req.
const MAX_NODES_DISPLAYED = 32; // See above

//
// Basic usage:
//
// 1. Create a new instance of a SelectionList.
// 2. Supply routines to build and manage the list of items being selected.
// 3. Supply a SelectRender routine to render a single selection line.
//   This routine can DO anything, but if you render into other
//   SelectionList lines, they may do likewise unto you!
// 4. Place calls from the IDCMP event dispatcher controlling this window.
//   These typically include MOUSEMOVE, MOUSEBUTTONS,
//   GADGETDOWN, GADGETUP, and INTUITICKS.
// 5. have fun!

#include "selector.h"
#include <stdio.h>

const SCROLL_UP = -1;
const SCROLL_DOWN = 32767;

//************************************************************************//

// determine the parameters for, and call, update_scrollbar()

int			// count slots not ghosted
Selector::real_slots()
{
	register int total = slots_displayed;
	register int ctr = 0;

	while (total-- > 0)
	{
		if (!is_ghosted(total)) ctr++;
	}
	return ctr;
}

void
Selector::calc_pot()
{
	register int potv = 0, bodyv = MAXBODY;
	if (node_count != 0)
	{
		register int rs = real_slots();
		register int nnum =  (node_count - rs);
		if (nnum < 1) nnum = 1;
		potv = current_node_number * MAXPOT / nnum;
		bodyv = rs * MAXBODY / node_count;
	}

// printf ("calc-pot cnode=%d, slots displayed=%d, %d, nodes=%d, potv=%d body=%d\n",
//	current_node_number, slots_displayed, real_slots(), node_count, potv, bodyv);

	adjust_scrollbar(select_gadget->next(), potv, bodyv);
}

Selector::Selector (Window *xw, register Gadget *xsg, List &xl)
{
	sel_w = xw;
	rp = xw->RPort;
	select_gadget = xsg;
	slot_height = rp->TxHeight;
	current_slot = NO_SLOT;
	min_y = xsg->TopEdge;
	slot_count = xsg->Height / slot_height;	// 8 for overhead
	max_y = xsg->TopEdge + slot_height*slot_count;
	slots_displayed = 0;
	node_list = &xl;
	prev_secs = prev_mics = 0;

	register int sc;
	register Node *nn = node_list->first();
	current_node = nn;
	for (sc = 0; ! end_of(nn); nn = nn->next())
		sc++;
	node_count = sc; 
	if (node_list->isempty()) ghost_everything();
	current_node_number = 0;
}

//************************************************************************//

// Highlight active slot.  Unhighlight any previously active one.
// Slots with nothing displayed in them don't count.

#define flop_slot(n) { point_to(n); complement(); }

void
Selector::highlight (SlotNumber new_slot)
{
	if (current_slot == new_slot) return;

	if (current_slot != NO_SLOT)
		flop_slot(current_slot);	// else unhighlight old slot

	if ((new_slot >= slots_displayed) ||
	    (is_ghosted(new_slot)))
	{
		current_slot = NO_SLOT;
		prev_secs = prev_mics = 0;	// forget about clicking!
		return;
	}

	flop_slot(new_slot);
	current_slot = new_slot;
	ticker = ROLL_TIME;	// fringe counter
}

// Ghost - we have removed the node corresponding to the current slot.
// unhighlight it, ghost-pattern its image and mark the slot
// as persona non grata.

#include <graphics/gfxmacros.h>

void
Selector::ghost(register SlotNumber n)
{
	static const UWORD ghost_pattern[] = {
//		0x8888, 0x8888, 0x2222, 0x2222
		0xcccc, 0xcccc, 0x3333, 0x3333
	};
 
	ghosted_slots |= (1 << n);
	if (n == current_slot)
	{
		flop_slot(n)
		current_slot = NO_SLOT;
	}
	// we need to RectFill with ghost pattern here!
	point_to(n);
//	rp->line(rp->cp_x + 20, rp->cp_y);
	SetAfPt(rp, ghost_pattern, 2);
	complement();
	SetAfPt(rp, NULL, 0);
}

Node *	// return address of n'th node down from given node.
Selector::seek_node(register Node *nn, register SHORT n)
{
	while (n-- > 0)
	{
		nn = nn->next();
		if (end_of(nn)) return NULL;
	}
	return nn;
}

// See if this node is currently being displayed

SlotNumber
Selector::displayed_at(Node *nd)
{
	register Node *xnn = current_node;
	register SlotNumber i;

	for (i = 0; i < slots_displayed; i++)
	{
		if (is_ghosted(i)) continue;	// doesn't count
		if (nd == xnn) return i;
		xnn = xnn->next();
	}
	return NO_SLOT;
}

void
Selector::start (USHORT mouse_y)
{
	ULONG myIDCMP = sel_w->IDCMPFlags;

// begin sensing mouse:
	myIDCMP |= MOUSEMOVE | MOUSEBUTTONS | INTUITICKS;
	ModifyIDCMP(sel_w, myIDCMP);
	USHORT slotnum =get_slot_number(mouse_y);

	ULONG secs, mics;
	CurrentTime (&secs, &mics);	
	if ((slotnum == current_slot) &&
		DoubleClick(prev_secs, prev_mics, secs, mics))
		; // remove this window, display specific option window
	else
		highlight(slotnum);
	prev_secs = secs; prev_mics = mics;
}

void
Selector::end_action (USHORT mouse_y, BOOL gadget_released)
{
	ULONG myIDCMP = sel_w->IDCMPFlags;
	myIDCMP &= ~(MOUSEMOVE | MOUSEBUTTONS | INTUITICKS);
	ModifyIDCMP(sel_w, myIDCMP);
//	if (gadget_released) CurrentTime (&prev_secs, &prev_mics);
}

// Track the mouse.  Unhighlights previous slot (if any) and
// highlights the slot that the mouse now indicates - PROVIDED
// that it's a slot with something displayed in it!

void
Selector::mouse_move (USHORT mouse_y)
{
	register USHORT new_slot = get_slot_number(mouse_y);

	if (new_slot == current_slot) return;

	if ((new_slot == SCROLL_UP) || (new_slot == SCROLL_DOWN))
		/* think of something */ ;
	else
		highlight(new_slot);
}

void
Selector::Intuitick()	// IntuiTick count
{
	ticker--;
	if (ticker == 0)
	{
		if (current_slot == 0) scroll(SELECT_DOWNWARDS);
		else
		if (current_slot == (slot_count-1)) scroll(SELECT_UPWARDS);
		ticker = ROLL_TIME;
	}
}

//************************************************************************//

// return slot number for given (mouse) vertical position

SlotNumber
Selector::get_slot_number(USHORT yy)
{
	if (yy < min_y)
		return 0;
	if (yy >= max_y)
		return slot_count - 1;
	return (yy - min_y) / slot_height;
}



Node *	// Same as above, but relative to display
Selector::node_displayed(SlotNumber nd)
{
	if (nd == NO_SLOT) return NULL;

	register Node *znn = current_node;
	for (register int i = 0; i < nd; i++)
	{
		if (is_ghosted(i)) continue;
		znn = znn->next();
		if (end_of(znn)) return NULL;
	}
	return (is_ghosted(i)) ? NULL : znn;
}

void
Selector::point_to(SlotNumber slot)
{
	rp->move(select_gadget->LeftEdge, min_y + slot_height * (slot+1)-1);
}

void
Selector::complement()	// XOR image of the slot set by point_to()
{
	register BYTE oldmode = rp->DrawMode;
	SetDrMd (rp, COMPLEMENT);
	RectFill (rp, select_gadget->LeftEdge, rp->cp_y-slot_height+2,
		select_gadget->LeftEdge+select_gadget->Width, rp->cp_y+1);
	SetDrMd (rp, oldmode);
}

// Scroll selection list up (or down) 1 entry.  Current slot number
// remains unchanged - it just indicates a different node.

void
Selector::scroll(enum scroll_dir up_dir)
{
	if (up_dir)
	{
		register Node *nn;

		nn = node_displayed(slot_count);
		if (nn == NULL)
			goto bleep;

		point_to(current_slot); complement();	// un-highlight
		current_node = current_node->next();
		current_node_number++;
		ScrollRaster (rp, 0, slot_height,
			select_gadget->LeftEdge, min_y,
			select_gadget->LeftEdge+select_gadget->Width,
			max_y);
		ghosted_slots >>= 1;
		point_to(slot_count-1);
		render (rp, nn);
	}
	else // down
	{
		if ((start_of(current_node)) ||
		    (start_of(current_node->prev() ))) // bol
			goto bleep;
		point_to(current_slot); complement();	// un-highlight
		current_node = current_node->prev();
		current_node_number--;
		ScrollRaster (rp, 0, -slot_height,
			select_gadget->LeftEdge, min_y,
			select_gadget->LeftEdge+select_gadget->Width,
			max_y);
		ghosted_slots <<= 1;
		point_to(0);
		render(rp, current_node);
		if (slots_displayed < slot_count) slots_displayed++;
	}
	point_to(current_slot); complement();	// highlight -v cant div by 0
	calc_pot();
	return;

bleep:
	DisplayBeep(NULL);
}

// Adjust display to point based on scroll bar.  pct: MAXPOT = 100%

void
Selector::scroll_set(int pct)
{
	current_slot = NO_SLOT;	// Turn off selection - we're about to wipe.
	register int i = node_count - slot_count;
	if (i < 0) i = 0;
	i = i * pct / MAXPOT;
	current_node = seek_node(node_list->first(), i);
	current_node_number = i;
	display_nodes();
}

Node *
Selector::clicked_on()
{
	if (current_slot == NO_SLOT) return NULL;

	ULONG secs, mics;
	CurrentTime (&secs, &mics);	
	if (DoubleClick(prev_secs, prev_mics, secs, mics))
		return node_displayed(current_slot);
	return NULL;
}

// Display

void
Selector::display_nodes()
{
	SetAPen(rp, 0);
	RectFill (rp, select_gadget->LeftEdge, min_y,	// clear whole area
		select_gadget->LeftEdge+select_gadget->Width, max_y);
	SetAPen(rp, 1);

// Gripe ("display queue nodes", "QN");
	slots_displayed = 0;

	register Node *qn = current_node;
	register SlotNumber i;
	for ( i = 0; i < slot_count; i++)
	{
		if (end_of(qn)) break;
		point_to(i);
		render (rp, qn);
		qn = qn->next();
	}
	slots_displayed = i;
	ghosted_slots = 0;		// All (filled) slots are enabled.
	current_slot = NO_SLOT;
	calc_pot();
	if (node_list->isempty()) ghost_everything();
	prev_secs = 0; prev_mics = 0;	// reset double-click detection.
}

// List management

void	// flip-flop,  dotting out display if nothing's in it.
Selector::ghost_everything()
{
	static const UWORD dots_me[] = {0x8888, 0x0000};

	register BYTE oldmode = rp->DrawMode;
	SetAPen(rp, 1);
	SetAfPt(rp, dots_me, 1);	// hazy line pattern.
	SetDrMd(rp, COMPLEMENT);
	RectFill (rp, select_gadget->LeftEdge, min_y,	// zap whole area
		select_gadget->LeftEdge+select_gadget->Width, max_y);
	SetAfPt(rp,NULL, 0);	// restore fill pattern
	SetDrMd(rp, oldmode);	// .etc
}

void		// append item to list.  Doesn't update display yet.
Selector::append(Node *n)
{
	if (node_list->isempty())
		ghost_everything();	// Un-ghost, actually
	node_list->append(n);
	node_count++;
	if (node_count == 1) current_node = n;	// handle case of the empty
	if (slots_displayed < slot_count)	// list!
	{
		point_to(slots_displayed++);	// add to display
		render(rp, n);
	}
	calc_pot();
}

// how about insert and enqueue??? Do later, I guess.

void		// remove item from list. Update display.
Selector::remove(Node *n)
{
	SlotNumber shown_at = displayed_at(n);

// printf ("Node %x displayed at %d has been removed\n", n, shown_at);
	if (shown_at != NO_SLOT) ghost(shown_at);

	if (n == current_node) current_node = current_node->next();
	n->remove();
	node_count--;
	if (node_list->isempty())
	{
		display_nodes();	// reset the world
//		SetAPen(rp, 0);	// clear whole area, then ghsot
//		RectFill (rp, select_gadget->LeftEdge, min_y,
//			select_gadget->LeftEdge+select_gadget->Width, max_y);
//		SetAPen(rp, 1);
//		ghost_everything();
	}
	calc_pot();
}

//=========================================================================

// dummy routines, if none supplied

void
Selector::render(RastPort *rp, Node *n)
{
	rp->text("ERROR: No render routine!");
}

void
Selector::adjust_scrollbar(Gadget *bar, register int pot, register int body)
{
	USHORT oldflags = ((PropInfo *)bar->SpecialInfo)->Flags;

	NewModifyProp (bar, sel_w, NULL,
		oldflags, 0, pot, 0, body, 1);
}
