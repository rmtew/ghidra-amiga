#ifndef GRAPHICS_GRAPHINT_H
#define GRAPHICS_GRAPHINT_H
#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif
struct Isrvstr : public Node
{
    Isrvstr *Iptr;
    int (*code)();
    int (*ccode)();
    int Carg;
};
#endif
