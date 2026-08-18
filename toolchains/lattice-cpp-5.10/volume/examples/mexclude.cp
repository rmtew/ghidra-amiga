/* mexclude.c - Mutual exlcusion */

/*
** MutualExclude - deselect gadgets based on exclusion specifications
**
** Author: Tim Holloway
**
** Date Written: 9/86
**
** Usage:
**
** (void) mutualexclude (window, gadget, requester);
**
** Description:
**
** The Gadget ID in the gadget passed to mutualexlcude is used as
** a bit flag number.  The list of gadgets on the window (or requester,
** if not NULL) is scanned.  For each gadget on the list which has
** MutualExclude bit # "gadget->GadgetID" set AND is presently SELECTED,
** remove the gadget, deselect it and re-add it.  The gadget list is
** then refreshed so that the deselected gadgets' images are updated.
*/

#include <exec/types.h>
#include <intuition/intuition.h>

#include <stdio.h>

mutualexclude(Window *win, Requester *req, Gadget * gad)
{
	register Gadget *gg;
	register LONGBITS turnoff;

	turnoff = (1L << gad->GadgetID);
	gg = (req != NULL) ? req->ReqGadget : win->FirstGadget;
	for ( ; gg != NULL; gg = gg->NextGadget)
	{
		register int i;

		if ((gg->Flags & SELECTED) && (gg->MutualExclude & turnoff))
		{
			i=RemoveGList (win, gg, 1);
			gg->Flags &= ~SELECTED;
			AddGList (win, gg, i, 1, req);
			RefreshGList (gg, win, req, 1);
		}
	}
}
