#ifndef INTUITION_INTUITIONBASE_H
#define INTUITION_INTUITIONBASE_H 1



#ifndef EXEC_LIBRARIES_H
#include "exec/libraries.h"
#endif

#ifndef  GRAPHICS_VIEW_H
#include "graphics/view.h"
#endif






struct IntuitionBase
{
    struct Library LibNode;

    struct View ViewLord;

    struct Window *ActiveWindow;
    struct Screen *ActiveScreen;


    struct Screen *FirstScreen; 
};

#endif
