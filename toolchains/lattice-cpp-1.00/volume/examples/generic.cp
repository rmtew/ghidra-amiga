// Generic.cp - Example of generic class creation
//
// This example DOESN'T compile into a working program, but
// you may want to examine the ".p" file created to see how with a
// little help from the pre-processor, we can easily create similar
// types that reduce the need for the error-prone practice of
// type casts.

#ifndef GENERIC_H
#include <generic.h> // defines the name2 function
#endif

#define myMinNode(type) \
struct name2(type, MinNode) { \
    name2(type, MinNode) *mln_Succ;  \
    name2(type, MinNode) *mln_Pred;	\
    name2(type, MinNode) *next() { return mln_Succ; } \
    name2(type, MinNode) *prev() { return mln_Pred; } \
}

#define myNode(type) \
struct name2(type, Node) : public Node { \
	name2(type, Node) *next() { return (name2(type, Node) *) ln_Succ; } \
	name2(type, Node) *prev() { return (name2(type, Node) *) ln_Pred; } \
	name2(type, Node) *find(const TEXT *n) \
	 { return (name2(type, Node) *) FindName(this, n); } \
}

//------------------------------------------------------------------------//

/*
**	Now for an example or two:
*/

myNode (IntuiMsg);

IntuiMsgNode x;

#message "This program isn't intended to be executed."
#message "But examine your 'QUAD:generic.p' file!"

/* try it for yourself - create a generic message type! */

#exit 20
