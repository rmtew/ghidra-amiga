#ifndef INTUITION_SCREENS_H
#define INTUITION_SCREENS_H
/*
**	$Filename: intuition/screens.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif

#ifndef GRAPHICS_GFX_H
#include "graphics/gfx.h"
#endif

#ifndef GRAPHICS_CLIP_H
#include "graphics/clip.h"
#endif

#ifndef GRAPHICS_VIEW_H
#include "graphics/view.h"
#endif

#ifndef GRAPHICS_RASTPORT_H
#include "graphics/rastport.h"
#endif

#ifndef GRAPHICS_LAYERS_H
#include "graphics/layers.h"
#endif

/* ======================================================================== */
/* === Screen ============================================================= */
/* ======================================================================== */
struct Screen
{
    struct Screen *NextScreen;		/* linked list of screens */
    struct Window *FirstWindow;		/* linked list Screen's Windows */

    SHORT LeftEdge, TopEdge;		/* parameters of the screen */
    SHORT Width, Height;		/* parameters of the screen */

    SHORT MouseY, MouseX;		/* position relative to upper-left */

    USHORT Flags;			/* see definitions below */

    UBYTE *Title;			/* null-terminated Title text */
    UBYTE *DefaultTitle;		/* for Windows without ScreenTitle */

    /* Bar sizes for this Screen and all Window's in this Screen */
    BYTE BarHeight, BarVBorder, BarHBorder, MenuVBorder, MenuHBorder;
    BYTE WBorTop, WBorLeft, WBorRight, WBorBottom;

    struct TextAttr *Font;		/* this screen's default font	   */

    /* the display data structures for this Screen */
    struct ViewPort ViewPort;		/* describing the Screen's display */
    struct RastPort RastPort;		/* describing Screen rendering	   */
    struct BitMap BitMap;		/* extra copy of RastPort BitMap   */
    struct Layer_Info LayerInfo;	/* each screen gets a LayerInfo	   */

    /* You supply a linked-list of Gadgets for your Screen.
     *	This list DOES NOT include system Gadgets.  You get the standard
     *	system Screen Gadgets by default
     */
    struct Gadget *FirstGadget;

    UBYTE DetailPen, BlockPen;		/* for bar/border/gadget rendering */

    /* the following variable(s) are maintained by Intuition to support the
     * DisplayBeep() color flashing technique
     */
    USHORT SaveColor0;

    /* This layer is for the Screen and Menu bars */
    struct Layer *BarLayer;

    UBYTE *ExtData;

    UBYTE *UserData;	/* general-purpose pointer to User data extension */
};


/* --- FLAGS SET BY INTUITION --------------------------------------------- */
/* The SCREENTYPE bits are reserved for describing various Screen types
 * available under Intuition.  
 */
#define SCREENTYPE	0x000F	/* all the screens types available	*/
/* --- the definitions for the Screen Type ------------------------------- */
#define WBENCHSCREEN	0x0001	/* Ta Da!  The Workbench		*/
#define CUSTOMSCREEN	0x000F	/* for that special look		*/

#define SHOWTITLE	0x0010	/* this gets set by a call to ShowTitle() */

#define BEEPING		0x0020	/* set when Screen is beeping		*/

#define CUSTOMBITMAP	0x0040	/* if you are supplying your own BitMap */

#define SCREENBEHIND	0x0080	/* if you want your screen to open behind
				 * already open screens
				 */
#define SCREENQUIET	0x0100	/* if you do not want Intuition to render
				 * into your screen (gadgets, title)
				 */

#define STDSCREENHEIGHT -1	/* supply in NewScreen.Height		*/


/* ======================================================================== */
/* === NewScreen ========================================================== */
/* ======================================================================== */
struct NewScreen
{
    SHORT LeftEdge, TopEdge, Width, Height, Depth;  /* screen dimensions */

    UBYTE DetailPen, BlockPen;		 /* for bar/border/gadget rendering */

    USHORT ViewModes;		/* the Modes for the ViewPort (and View) */

    USHORT Type;		/* the Screen type (see defines above) */
    
    struct TextAttr *Font;	/* this Screen's default text attributes */
    
    UBYTE *DefaultTitle;	/* the default title for this Screen */
    
    struct Gadget *Gadgets;	/* your own Gadgets for this Screen */
    
    /* if you are opening a CUSTOMSCREEN and already have a BitMap 
     * that you want used for your Screen, you set the flags CUSTOMBITMAP in
     * the Type field and you set this variable to point to your BitMap
     * structure.  The structure will be copied into your Screen structure,
     * after which you may discard your own BitMap if you want
     */
    struct BitMap *CustomBitMap;
};

#endif	/* INTUITION_SCREENS_H */
