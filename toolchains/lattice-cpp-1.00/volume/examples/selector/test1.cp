// test1.cp - file selector

// This is a generic selector program that will walk down a directory
// tree and display file and subdirectory names.  Double-clicking
// on a directory name causes the contents of that directory to be displayed.
// A directory path may be supplied via the string gadget above the display.
// Since this is only a demo, most of the other gadgets are not actually
// used.

// For neatness' sake, I have called a simple sort routine that bubble-sorts
// the Qnodes in the generic selector list, so that all the directories
// display first.  The node sorter is also generic, and is modelled after
// the Unix [tm] qsort() routine - you pass the address of a comparator
// function that returns -1, 0, or +1 to indicate whether the first operand
// is less, equal, or greater than the second.  Simple modifications would
// permit sorting on filenames, etc. as well.  Astute observers will notice
// type mismatches between the protoypical generic sort compare function and
// its actual implementation here.  In short, I lied to C++, and this is ONE
// case where it can't protect me!  Life would be a bit stickier if this
// program required more than one sort of list to be sorted.  In "pure"
// C++, the solution is to make the sort compare be a virtual function
// for Nodes, but Nodes are part of Amiga EXEC and not suited for such 
// treatment.  The sort is just window dressing in this case - we COULD
// have built the list in order.

// You may notice a little sluggishness about the select bar not
// keeping up with the mouse.  This is probably Intuition's fault.
// I seem to recall that it collects update requests and schedules them
// every couple of doozen milliseconds - thus the refresh function for the
// filenote gadget causes the program to be delayed for small periods of
// time.  The problem's purely cosmetic, but a little behind the scenes
// trickery (like rendering the filenote directly) would probably
// suffice.

// More serious is the delay during which the directory is read (and
// sorted) before anything actually displays.  This could be avoided by
// creating the "Counter" FIRST, and using the append() function to add
// each new name as they come back from the ExNext() function.  However,
// you'd then have to briefly DESTROY the Counter if you wanted to sort
// the list or really crazy things can happen!

#include "selector.h"

enum { SELECT_GADGET = 1, SCROLL_BAR, INSERT_GADGET, DELETE_GADGET,
	DIR_GAD, NOTE_GAD, FILE_GAD };

// For expanded version:  Add a dummy Qnode at the front (sort low)
// to allow backtracing:

const char *PARENT = "« Parent Directory";

#include "test1.h"
#include <stdio.h>
#include <libraries/dos.h>
#include <string.h>

char wrk[80];

Window *mywindow;


//========================================================================

void SelectionPrint (RastPort *, Node *);

class Counter : public Selector {
public:
	inline void render(RastPort *rp, Node *nn);
	Counter	(Window *w, register Gadget *xsg, List &xl) : (w, xsg, xl)
	{};
};

//Counter::	Counter	(Window *w, register Gadget *xsg, List *xl) : (w, xsg, xl);

Counter *select;
static FileInfoBlock *dir_info;

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
	char filename[33];
	char filenote[81];
	char filetype;
public:
	Qnode (FileInfoBlock *);
	Qnode *next() { return (Qnode *) Node::next(); }
	void render(RastPort *);

	void append()
	{
		(select) ? select->append(this) : Qlist.append(this);
	}
	void remove()
	{
		(select) ? select->remove(this) : this->Node::remove();
	}
};

inline void
Counter::render(RastPort *rp, Node *nn)
{ ((Qnode *) nn)->render(rp); }

Qnode::Qnode(FileInfoBlock *fb)
{
	strcpy(filename, fb->fib_FileName);
	strcpy(filenote, fb->fib_Comment);
	filetype = (fb->is_file() ) ? 'F' : 'D';
}

void
new_note(const char *s)
{
	strcpy(note_gadSIBuff, s);
	mywindow->refresh(&note_gad, 1);
}

void		// Add a level to the displayed/used directory path
new_level(const char *s)
{
	register int i = strlen(dir_gadSIBuff);

	if ((i > 0) && (dir_gadSIBuff[i-1] != ':'))
		dir_gadSIBuff[i++] = '/';
	strcpy (dir_gadSIBuff+i, s);
}

// Print information (1 line). Rastport is already set up OK.

void
Qnode::render(RastPort *rp)
{
	if (filetype == 'F')
		Text (rp, "  ", 2);
	else
		Text (rp, "» ", 2);
	Text(rp, filename, strlen(filename) );
}

// compare function for Qnode sorting.  Directories sort ahead of files.
// *** Caution *** for any sort compare function, make SURE that when
// two items are compared, they compare in the same way regardless
// of which one is the first argument.  Otherwise the sort will oscillate
// instead of terminating.

int
dcompare (Qnode *n1, Qnode *n2)
{
	if (n1->filetype == n2->filetype) return 0;

	return (n1->filetype == 'F') ? 1 : -1;
}

// Add a new node to the Qnode list (Qlist).  Icon files (.info files)
// are not included.

void
create_display_queue(const char *dirname)
{
	BPTR dirlock;

strcpy (dir_gadSIBuff, dirname);
mywindow->refresh(&dir_gad, 1);

	dirlock = Lock(dirname, ACCESS_READ);
	if (dirlock == NULL)
	{
		new_note("  ««« No such Drawer! »»»");
		goto phooey;
	}

	Examine (dirlock, dir_info);
	new_note(dir_info->fib_Comment);

	while (TRUE)
	{
		char *dummy;

		if (!ExNext(dirlock, dir_info)) break;
		if (stcpm(dir_info->fib_FileName, ".info", &dummy) != 0)
			continue;
		Qlist.append(new Qnode (dir_info));
	}
	UnLock(dirlock);

phooey:
	extern void
	bubblesort(List &ls,  int (*compare)(Qnode *, Qnode *));

	bubblesort(Qlist, dcompare);

	select = new Counter (mywindow, &sel_gad, Qlist);

	select->display_nodes();
}

// Clean out queue display list

void
destroy_display_queue()
{
	if (select)
	{
		delete select;
		select = NULL;
	}

	while (!Qlist.isempty())
	{
		delete ( (Qnode *) Qlist.get());
	}
}

void
display_directory(char *str)
{
	strcpy(dir_gadSIBuff, str);
	if (streq (str, PARENT))
	{
		register char *cp = dir_gadSIBuff + strlen(dir_gadSIBuff) -1;
		while (cp > dir_gadSIBuff)
		{
			if (*cp == '/')
			{
				*cp = '\000';
				break;}
			else
			if (*cp == ':')
			{
				*(cp+1) = '\000';
				break;
			}
			cp--;
		}
	}

	mywindow->remove(&sel_gad, 2); // disable
	destroy_display_queue();
	mywindow->refresh(&dir_gad, 1);
	create_display_queue(dir_gadSIBuff);
	mywindow->add(&sel_gad, AT_END, 2);	// re-add
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
	dir_info = new FileInfoBlock;
	create_display_queue("df0:");


  register short KeepGoing;
  for (KeepGoing = TRUE; KeepGoing; )
   {
	struct IntuiMessage *message;
	register short MessageClass, code;

      Wait (1L << mywindow->UserPort->mp_SigBit);
      while (message = (struct IntuiMessage *)GetMsg(mywindow->UserPort))
      {
	struct Gadget *igad;
	USHORT mouse_y;

        MessageClass = message->Class;
        code = message->Code;
        USHORT qual = message->Qualifier;

        ReplyMsg((struct Message *) message);
	igad = (struct Gadget *) message->IAddress;
	mouse_y = message->MouseY;

        switch (MessageClass) {

	case GADGETUP    :
		switch (igad->GadgetID) {
		case SCROLL_BAR :
			select->scroll_set(igad->Prop()->VertPot);
			break;

		case SELECT_GADGET :
			select->end_action(mouse_y, TRUE);
			break;

		case INSERT_GADGET :
			int j = igad->String()->LongInt; 
//			select->append(new Qnode(j));
//			Qnode *nn = new Qnode(j);
//			nn->append();
			break;

		case DELETE_GADGET :
			{
//				Node *nn = select->selected_node();
//				printf ("Remove %d\n", ((Qnode *)nn)->value());
//				if (nn) select->remove(nn);
			}
			break;
		case DIR_GAD :
			display_directory(dir_gadSIBuff);
		}
		break;

	case GADGETDOWN :
		if (igad->GadgetID == (int)SELECT_GADGET)
		{	// if dir, new level, else ???
			select->start(mouse_y);
			Qnode *nn = (Qnode *) select->selected_node();
			if (nn) new_note( nn->filenote);

			// for directories, go down a level.

			nn = (Qnode *) select->clicked_on();
			if (nn && nn->filetype != 'F')
			{
				new_level(nn->filename);
				display_directory(dir_gadSIBuff);
			}
		}
		break;

	case MOUSEMOVE :	/* only whilst chasing mouse*/
		select->mouse_move(mouse_y);
		Qnode *nn = (Qnode *) select->selected_node();
		if (nn) new_note( nn->filenote);
		break;

	case INTUITICKS :	/* ditto */
		select->Intuitick();
		break;

	case MOUSEBUTTONS :	/* only whilst chasing mouse */
		if (code == SELECTUP)
		{
			select->end_action(mouse_y, FALSE);
		}
		else
		{
			Qnode *nn = (Qnode *) select->selected_node();
			if (nn) new_note( nn->filenote);
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
	delete dir_info;
}
