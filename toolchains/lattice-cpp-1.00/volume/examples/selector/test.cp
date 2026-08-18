#include "selector.h"

enum { SELECT_GADGET = 1, SCROLL_BAR, INSERT_GADGET, DELETE_GADGET };

#include "test.h"
#include <stdio.h>

char wrk[80];

Window *mywindow;


//========================================================================

void SelectionPrint (RastPort *, Node *);

class Counter : public Selector {
public:
	void render(RastPort *rp, Node *nn) { SelectionPrint (rp, nn); }
	Counter	(Window *w, register Gadget *xsg, List &xl) : (w, xsg, xl)
	{};
};

//Counter::	Counter	(Window *w, register Gadget *xsg, List &xl) : (w, xsg, xl);

Counter *select;

void sel_mouse (mouse_y)
{
	select->mouse_move(mouse_y);
}


void sel_ticks (USHORT)
{
	select->Intuitick();
}

void sel_scroll(int pct)
{
	select->scroll_set(pct);
}

//*************************************************************************

// Node of display

static struct NewWindow nw = {
   10, 10, 300, 150, 0, 1,
   GADGETDOWN | GADGETUP | REQCLEAR ,
   SMART_REFRESH | BORDERLESS,
   &GadgetList1, /* firstgadget */
   NULL, /* checkmark */
   NULL, /* title */
   NULL, /* screen*/
   NULL, /* bitmap */
   0, 0, 0, 0,
   WBENCHSCREEN
};

static List Qlist;

struct Qnode : public Node {
	USHORT digit;
public:
	Qnode (int);
	Qnode *next() { return (Qnode *) Node::next(); }
	friend void SelectionPrint(RastPort *, Node *n);
	int value() { return digit; }
	void append()
	{
		(select) ? select->append(this) : Qlist.append(this);
	}
	void remove()
	{
		(select) ? select->remove(this) : this->Node::remove();
	}
};

Qnode::Qnode(int n)
{
	digit = n;
}

// Print information (1 line). Rastport is already set up OK.

void
SelectionPrint(RastPort *rp, Node *n)
{
	char print_line[32];
	sprintf (print_line, "Test item %04d     ", ((Qnode *)n)->digit);
	Text(rp, print_line, 14);
}


// Add a new node to the Qnode list (Qlist)

void
create_display_queue()
{
//	for (register int i = 0; i < 25; i++)
//	{
//		Qlist.append(new Qnode);
//	}

	select = new Counter (mywindow, &GadgetList1, Qlist);
	select->display_nodes();
}

// Clean out queue display list

void
destroy_display_queue()
{
	delete select;

	while (!Qlist.isempty())
	{
		delete ( (Qnode *) Qlist.get());
	}
}

// Permit scrolling based on cursor keys

void
scroll_on_key(int code, int qual)
{
	if ((code == CURSORDOWN) || (code == CURSORUP))
	if (qual & (IEQUALIFIER_LALT | IEQUALIFIER_RALT)) // total list
	{
		(code == CURSORUP) ?
			select->scroll_to(0) :
			select->scroll_to(AT_END);
	}
	else
	if (qual & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT)) // page
	{
		select->scroll_page(code == CURSORDOWN);
	}
	else
		select->scroll(code == CURSORDOWN);
}

main()
{
	mywindow = new Window (&NewWindowStructure1);
	if (mywindow == NULL)
	{
		printf ("ERROR: couldn't open selector window!\n");
		exit(20);
	}
	create_display_queue();

  register short KeepGoing;
  for (KeepGoing = TRUE; KeepGoing; )
   {
	struct IntuiMessage *message;
	register short MessageClass, code;

      Wait (1L << mywindow->UserPort->mp_SigBit);
      while (message = (struct IntuiMessage *)GetMsg(mywindow->UserPort))
      {
	struct Gadget *igad;
	BOOL chase_mouse;
	USHORT mouse_y;

        MessageClass = message->Class;
        code = message->Code;
        USHORT qual = message->Qualifier;

        ReplyMsg((struct Message *) message);
	igad = (struct Gadget *) message->IAddress;
	mouse_y = message->MouseY;
	chase_mouse = FALSE;

        switch (MessageClass) {

	case GADGETUP    :
		switch (igad->GadgetID) {
		case SCROLL_BAR :
			sel_scroll(
// ((struct PropInfo *)igad->SpecialInfo)
				igad->Prop()->VertPot);
			break;

		case SELECT_GADGET :
			select->end_action(mouse_y, TRUE);
			chase_mouse = FALSE;
			break;

		case INSERT_GADGET :
			int j = igad->String()->LongInt; 
//			select->append(new Qnode(j));
			Qnode *nn = new Qnode(j);
			nn->append();
			break;

		case DELETE_GADGET :
			{
				Node *nn = select->selected_node();
				printf ("Remove %d\n", ((Qnode *)nn)->value());
				if (nn) select->remove(nn);
			}
			break;
		}
		break;

	case GADGETDOWN :
		if (igad->GadgetID == (int)SELECT_GADGET)
		{
			Qnode *nn;
			if ( (nn = (Qnode *) select->clicked_on()) != NULL)
				printf ("I Clicked on node %d.\n", nn->digit);

			select->start(mouse_y);
			chase_mouse = TRUE;
		}
		break;

	case MOUSEMOVE :	/* only whilst chasing mouse*/
		sel_mouse(mouse_y);
		break;

	case INTUITICKS :	/* ditto */
		sel_ticks(mouse_y);
		break;

	case MOUSEBUTTONS :	/* only whilst chasing mouse */
		if (code == SELECTUP)
		{
			select->end_action(mouse_y, FALSE);
			chase_mouse = FALSE;
		}
		else
		{
			Qnode *nn;
			if ( (nn = (Qnode *) select->clicked_on()) != NULL)
				printf ("Clicked on node %d", nn->digit);
		}
		break;

	case RAWKEY :
		if ((code == CURSORDOWN) || (code == CURSORUP))
			scroll_on_key(code, qual);
		break;

//	case MENUPICK :
//		menu_option(code);
//		break;

	case REQSET :
	case REQVERIFY : break;
		
	case CLOSEWINDOW :
		destroy_display_queue();
		KeepGoing = FALSE;
		break;

	default:
		sprintf (wrk, "Class %d", MessageClass);
		Gripe ("Unrecognized action", wrk);
        }   /* Case */
      }  /* if */
   }  /* for */
	delete mywindow;
}
