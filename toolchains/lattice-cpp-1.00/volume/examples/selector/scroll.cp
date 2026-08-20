// scroll.cp - generic selector
// optional routines to permit scrolling via cursor keys

#include "selector.h"

// position display so that node number "new_num" is the first node
// in the display.  Fudges a little to maximize display - if
// new_num is less than "slot_count" nodes from the end, backs up
// so that a full slot_count of nodes are displayed.

// Moderately efficient at positioning, but should be tweaked for VERY
// long lists so as to also scan backwards from current or final nodes, etc.
// whichever has the shortest path.

void
Selector::scroll_to(int new_num)
{
	register int i;
	register Node *nn = current_node;

	int final_node = node_count - slot_count;

	// Limits are top of list in the upwards direction and a full
	// display in the downward direction.

	if (new_num > final_node) new_num = final_node;
	if (new_num < 0) new_num = 0;	// NOT else-if!!!

	/* take shortest path forward to new node */

	if (new_num > current_node_number)
		i = new_num - current_node_number;
	else
	{
		nn = node_list->first();
		i = new_num;
	}
	while (i-- > 0) nn = nn->next();

	current_node = nn;
	current_node_number = new_num;
	display_nodes();
}

// Scroll up or down one display page (overlap by 1 line).
// By convention, as established by Commodore (NotePad, AmigaBASIC)
// Holding down the SHIFT key and pressing up- or down-arrow should
// cause this function to be called.

void
Selector::scroll_page(enum scroll_dir go_up)
{
	register int newnum = current_node_number;

	if (go_up)	// forwards is nastier, since slots may be ghosted.
		newnum += (real_slots() - 2);
	else
		newnum -= (slot_count - 2);

	scroll_to(newnum);
}
