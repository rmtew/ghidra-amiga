// nodesort.cp - sort Nodes

#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/lists.h>

// Exchange adjacent nodes
// *** CAUTION ** Forbid or Disable before working on multi-tasking Nodes!

void
exchange(register Node *n1)
{
	register Node *n2 = n1->next();

	n1->prev()->ln_Succ = n2;
	n2->next()->ln_Pred = n1;

	n1->ln_Succ = n2->next();
	n2->ln_Pred = n1->prev();

	n1->ln_Pred = n2;
	n2->ln_Succ = n1;
}

inline BOOL
at_start(Node *n) { return n->prev()->prev() == NULL; }

inline BOOL
at_end(Node *n) { return n->next()->next() == NULL; }

// bubble sort a List:

	// sort at least 2 nodes

static BOOL
bubble_pass(register Node *n1, int (*compare)(Node *, Node *))
{
	register Node *n2 = n1->next();
	register BOOL in_sequence = TRUE;

	while (! at_end(n1) )
	{
		if ((*compare)(n1, n2) > 0)
		{
			exchange (n1);
			in_sequence = FALSE;
			n2 = n1;	// 'cause we just switched order!
		}
		n1 = n2; n2 = n2->next();
	}
	return in_sequence;
}

void
bubblesort(List &ls,  int (*compare)(Node *, Node *))
{
	if (! ls.isempty() && !at_end(ls.first()))
		while (!bubble_pass(ls.first(), compare)) /* sort it */;
}
