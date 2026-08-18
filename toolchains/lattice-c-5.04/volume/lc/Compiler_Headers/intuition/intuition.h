#ifndef INTUITION_INTUITION_H
#define INTUITION_INTUITION_H
/*
**	$Filename: intuition/intuition.h $
**	$Release: 1.3 $
**
**	main intuition include 
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
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

#ifndef GRAPHICS_TEXT_H
#include "graphics/text.h"
#endif

#ifndef EXEC_PORTS_H
#include "exec/ports.h"
#endif

#ifndef DEVICES_TIMER_H
#include "devices/timer.h"
#endif

#ifndef DEVICES_INPUTEVENT_H
#include "devices/inputevent.h"
#endif


/* ======================================================================== */
/* === Menu =============================================================== */
/* ======================================================================== */
struct Menu
{
    struct Menu *NextMenu;	/* same level */
    SHORT LeftEdge, TopEdge;	/* position of the select box */
    SHORT Width, Height;	/* dimensions of the select box */
    USHORT Flags;		/* see flag definitions below */
    BYTE *MenuName;		/* text for this Menu Header */
    struct MenuItem *FirstItem; /* pointer to first in chain */

    /* these mysteriously-named variables are for internal use only */
    SHORT JazzX, JazzY, BeatX, BeatY;
};


/* FLAGS SET BY BOTH THE APPLIPROG AND INTUITION */
#define MENUENABLED 0x0001	/* whether or not this menu is enabled */

/* FLAGS SET BY INTUITION */
#define MIDRAWN 0x0100		/* this menu's items are currently drawn */






/* ======================================================================== */
/* === MenuItem =========================================================== */
/* ======================================================================== */
struct MenuItem 
{ 
    struct MenuItem *NextItem;	/* pointer to next in chained list */ 
    SHORT LeftEdge, TopEdge;	/* position of the select box */ 
    SHORT Width, Height;	/* dimensions of the select box */ 
    USHORT Flags;		/* see the defines below */ 
 
    LONG MutualExclude;		/* set bits mean this item excludes that */ 
 
    APTR ItemFill;		/* points to Image, IntuiText, or NULL */ 
 
    /* when this item is pointed to by the cursor and the items highlight 
     *	mode HIGHIMAGE is selected, this alternate image will be displayed 
     */ 
    APTR SelectFill;		/* points to Image, IntuiText, or NULL */

    BYTE Command;		/* only if appliprog sets the COMMSEQ flag */

    struct MenuItem *SubItem;	/* if non-zero, DrawMenu shows "->" */

    /* The NextSelect field represents the menu number of next selected 
     *	item (when user has drag-selected several items)
     */
    USHORT NextSelect;
};


/* FLAGS SET BY THE APPLIPROG */ 
#define CHECKIT		0x0001	/* whether to check this item if selected */ 
#define ITEMTEXT	0x0002	/* set if textual, clear if graphical item */ 
#define COMMSEQ		0x0004	/* set if there's an command sequence */ 
#define MENUTOGGLE	0x0008	/* set to toggle the check of a menu item */ 
#define ITEMENABLED	0x0010	/* set if this item is enabled */ 
 
/* these are the SPECIAL HIGHLIGHT FLAG state meanings */ 
#define HIGHFLAGS	0x00C0	/* see definitions below for these bits */ 
#define HIGHIMAGE	0x0000	/* use the user's "select image" */ 
#define HIGHCOMP	0x0040	/* highlight by complementing the selectbox */
#define HIGHBOX		0x0080	/* highlight by "boxing" the selectbox */ 
#define HIGHNONE	0x00C0	/* don't highlight */ 
 
/* FLAGS SET BY BOTH APPLIPROG AND INTUITION */ 
#define CHECKED		0x0100	/* if CHECKIT, then set this when selected */ 
 
/* FLAGS SET BY INTUITION */ 
#define ISDRAWN		0x1000	/* this item's subs are currently drawn */ 
#define HIGHITEM	0x2000	/* this item is currently highlighted */ 
#define MENUTOGGLED	0x4000	/* this item was already toggled */ 
 
 
 
 
 
/* ======================================================================== */
/* === Requester ========================================================== */
/* ======================================================================== */
struct Requester
{
    /* the ClipRect and BitMap and used for rendering the requester */
    struct Requester *OlderRequest;
    SHORT LeftEdge, TopEdge;		/* dimensions of the entire box */
    SHORT Width, Height;		/* dimensions of the entire box */
    SHORT RelLeft, RelTop;		/* for Pointer relativity offsets */

    struct Gadget *ReqGadget;		/* pointer to a list of Gadgets */
    struct Border *ReqBorder;		/* the box's border */
    struct IntuiText *ReqText;		/* the box's text */
    USHORT Flags;			/* see definitions below */

    /* pen number for back-plane fill before draws */
    UBYTE BackFill;
    /* Layer in place of clip rect	*/
    struct Layer *ReqLayer;

    UBYTE ReqPad1[32];

    /* If the BitMap plane pointers are non-zero, this tells the system 
     * that the image comes pre-drawn (if the appliprog wants to define 
     * it's own box, in any shape or size it wants!);  this is OK by 
     * Intuition as long as there's a good correspondence between 
     * the image and the specified Gadgets
     */
    struct BitMap *ImageBMap;	/* points to the BitMap of PREDRAWN imagery */
    struct Window *RWindow;	/* added.  points back to Window */
    UBYTE ReqPad2[36];
};


/* FLAGS SET BY THE APPLIPROG */
#define POINTREL 0x0001	 /* if POINTREL set, TopLeft is relative to pointer*/
#define PREDRAWN 0x0002	 /* if ReqBMap points to predrawn Requester imagery */
#define NOISYREQ 0x0004	 /* if you don't want requester to filter input	   */
/* FLAGS SET BY BOTH THE APPLIPROG AND INTUITION */

/* FLAGS SET BY INTUITION */
#define REQOFFWINDOW	0x1000	/* part of one of the Gadgets was offwindow */
#define REQACTIVE	0x2000	/* this requester is active */
#define SYSREQUEST	0x4000	/* this requester caused by system */
#define DEFERREFRESH	0x8000	/* this Requester stops a Refresh broadcast */






/* ======================================================================== */
/* === Gadget ============================================================= */
/* ======================================================================== */
struct Gadget
{
    struct Gadget *NextGadget;	/* next gadget in the list */

    SHORT LeftEdge, TopEdge;	/* "hit box" of gadget */
    SHORT Width, Height;	/* "hit box" of gadget */

    USHORT Flags;		/* see below for list of defines */

    USHORT Activation;		/* see below for list of defines */

    USHORT GadgetType;		/* see below for defines */

    /* appliprog can specify that the Gadget be rendered as either as Border
     * or an Image.  This variable points to which (or equals NULL if there's
     * nothing to be rendered about this Gadget)
     */
    APTR GadgetRender;

    /* appliprog can specify "highlighted" imagery rather than algorithmic
     * this can point to either Border or Image data
     */
    APTR SelectRender;

    struct IntuiText *GadgetText;   /* text for this gadget */

    /* by using the MutualExclude word, the appliprog can describe 
     * which gadgets mutually-exclude which other ones.	 The bits 
     * in MutualExclude correspond to the gadgets in object containing 
     * the gadget list.	 If this gadget is selected and a bit is set 
     * in this gadget's MutualExclude and the gadget corresponding to 
     * that bit is currently selected (e.g. bit 2 set and gadget 2 
     * is currently selected) that gadget must be unselected.  
     * Intuition does the visual unselecting (with checkmarks) and 
     * leaves it up to the program to unselect internally
     */
    LONG MutualExclude;	 /* set bits mean this gadget excludes that gadget */

    /* pointer to a structure of special data required by Proportional, 
     * String and Integer Gadgets 
     */
    APTR SpecialInfo;

    USHORT GadgetID;	/* user-definable ID field */
    APTR UserData;	/* ptr to general purpose User data (ignored by In) */
};


/* --- FLAGS SET BY THE APPLIPROG ----------------------------------------- */
/* combinations in these bits describe the highlight technique to be used */
#define GADGHIGHBITS	0x0003
#define GADGHCOMP	0x0000	/* Complement the select box */
#define GADGHBOX	0x0001	/* Draw a box around the image */
#define GADGHIMAGE	0x0002	/* Blast in this alternate image */
#define GADGHNONE	0x0003	/* don't highlight */

/* set this flag if the GadgetRender and SelectRender point to Image imagery, 
 * clear if it's a Border 
 */
#define GADGIMAGE 0x0004 

/* combinations in these next two bits specify to which corner the gadget's
 *  Left & Top coordinates are relative.  If relative to Top/Left,
 *  these are "normal" coordinates (everything is relative to something in
 *  this universe)
 */
#define GRELBOTTOM	0x0008	/* set if rel to bottom, clear if rel top */
#define GRELRIGHT	0x0010	/* set if rel to right, clear if to left */
/* set the RELWIDTH bit to spec that Width is relative to width of screen */
#define GRELWIDTH	0x0020
/* set the RELHEIGHT bit to spec that Height is rel to height of screen */
#define GRELHEIGHT	0x0040

/* the SELECTED flag is initialized by you and set by Intuition.  It 
 * specifies whether or not this Gadget is currently selected/highlighted
 */
#define SELECTED	0x0080


/* the GADGDISABLED flag is initialized by you and later set by Intuition
 * according to your calls to On/OffGadget().  It specifies whether or not 
 * this Gadget is currently disabled from being selected
 */
#define GADGDISABLED	0x0100


/* --- These are the Activation flag bits --------------------------------- */
/* RELVERIFY is set if you want to verify that the pointer was still over
 *  the gadget when the select button was released
 */
#define RELVERIFY	0x0001

/* the flag GADGIMMEDIATE, when set, informs the caller that the gadget
 *  was activated when it was activated.  this flag works in conjunction with
 *  the RELVERIFY flag
 */
#define GADGIMMEDIATE	0x0002

/* the flag ENDGADGET, when set, tells the system that this gadget, when
 * selected, causes the Requester or AbsMessage to be ended.  Requesters or
 *  AbsMessages that are ended are erased and unlinked from the system */
#define ENDGADGET	0x0004

/* the FOLLOWMOUSE flag, when set, specifies that you want to receive
 * reports on mouse movements (ie, you want the REPORTMOUSE function for
 * your Window).  When the Gadget is deselected (immediately if you have
 * no RELVERIFY) the previous state of the REPORTMOUSE flag is restored
 * You probably want to set the GADGIMMEDIATE flag when using FOLLOWMOUSE,
 * since that's the only reasonable way you have of learning why Intuition
 * is suddenly sending you a stream of mouse movement events.  If you don't
 * set RELVERIFY, you'll get at least one Mouse Position event.
 */
#define FOLLOWMOUSE	0x0008

/* if any of the BORDER flags are set in a Gadget that's included in the
 * Gadget list when a Window is opened, the corresponding Border will
 * be adjusted to make room for the Gadget
 */
#define RIGHTBORDER	0x0010
#define LEFTBORDER	0x0020
#define TOPBORDER	0x0040
#define BOTTOMBORDER	0x0080

#define TOGGLESELECT	0x0100	/* this bit for toggle-select mode */

#define STRINGCENTER	0x0200	/* should be a StringInfo flag, but it's OK */
#define STRINGRIGHT	0x0400	/* should be a StringInfo flag, but it's OK */

#define LONGINT		0x0800	/* this String Gadget is actually LONG Int */

#define ALTKEYMAP	0x1000	/* this String has an alternate keymap */

#define BOOLEXTEND	0x2000	/* this Boolean Gadget has a BoolInfo	*/


/* --- GADGET TYPES ------------------------------------------------------- */
/* These are the Gadget Type definitions for the variable GadgetType
 * gadget number type MUST start from one.  NO TYPES OF ZERO ALLOWED.
 * first comes the mask for Gadget flags reserved for Gadget typing
 */
#define GADGETTYPE	0xFC00	/* all Gadget Global Type flags (padded) */
#define SYSGADGET	0x8000	/* 1 = SysGadget, 0 = AppliGadget */
#define SCRGADGET	0x4000	/* 1 = ScreenGadget, 0 = WindowGadget */
#define GZZGADGET	0x2000	/* 1 = Gadget for GIMMEZEROZERO borders */
#define REQGADGET	0x1000	/* 1 = this is a Requester Gadget */
/* system gadgets */
#define SIZING		0x0010
#define WDRAGGING	0x0020
#define SDRAGGING	0x0030
#define WUPFRONT	0x0040
#define SUPFRONT	0x0050
#define WDOWNBACK	0x0060
#define SDOWNBACK	0x0070
#define CLOSE		0x0080
/* application gadgets */
#define BOOLGADGET	0x0001
#define GADGET0002	0x0002
#define PROPGADGET	0x0003
#define STRGADGET	0x0004


/* ======================================================================== */
/* === BoolInfo======================================================= */
/* ======================================================================== */
/* This is the special data needed by an Extended Boolean Gadget
 * Typically this structure will be pointed to by the Gadget field SpecialInfo
 */
struct BoolInfo
{
    USHORT Flags;	/* defined below */
    UWORD  *Mask;	/* bit mask for highlighting and selecting
			 * mask must follow the same rules as an Image
			 * plane.  It's width and height are determined
			 * by the width and height of the gadget's 
			 * select box. (i.e. Gadget.Width and .Height).
			 */
    ULONG  Reserved;	/* set to 0	*/
};

/* set BoolInfo.Flags to this flag bit.
 * in the future, additional bits might mean more stuff hanging
 * off of BoolInfo.Reserved.
 */
#define BOOLMASK	0x0001	/* extension is for masked gadget */

/* ======================================================================== */
/* === PropInfo =========================================================== */
/* ======================================================================== */
/* this is the special data required by the proportional Gadget
 * typically, this data will be pointed to by the Gadget variable SpecialInfo
 */
struct PropInfo
{
    USHORT Flags;	/* general purpose flag bits (see defines below) */

    /* You initialize the Pot variables before the Gadget is added to 
     * the system.  Then you can look here for the current settings 
     * any time, even while User is playing with this Gadget.  To 
     * adjust these after the Gadget is added to the System, use 
     * ModifyProp();  The Pots are the actual proportional settings, 
     * where a value of zero means zero and a value of MAXPOT means 
     * that the Gadget is set to its maximum setting.  
     */
    USHORT HorizPot;	/* 16-bit FixedPoint horizontal quantity percentage */
    USHORT VertPot;	/* 16-bit FixedPoint vertical quantity percentage */

    /* the 16-bit FixedPoint Body variables describe what percentage of 
     * the entire body of stuff referred to by this Gadget is actually 
     * shown at one time.  This is used with the AUTOKNOB routines, 
     * to adjust the size of the AUTOKNOB according to how much of 
     * the data can be seen.  This is also used to decide how far 
     * to advance the Pots when User hits the Container of the Gadget.	
     * For instance, if you were controlling the display of a 5-line 
     * Window of text with this Gadget, and there was a total of 15 
     * lines that could be displayed, you would set the VertBody value to 
     *	   (MAXBODY / (TotalLines / DisplayLines)) = MAXBODY / 3.
     * Therefore, the AUTOKNOB would fill 1/3 of the container, and 
     * if User hits the Cotainer outside of the knob, the pot would 
     * advance 1/3 (plus or minus) If there's no body to show, or 
     * the total amount of displayable info is less than the display area, 
     * set the Body variables to the MAX.  To adjust these after the 
     * Gadget is added to the System, use ModifyProp();	 
     */
    USHORT HorizBody;		/* horizontal Body */ 
    USHORT VertBody;		/* vertical Body */

    /* these are the variables that Intuition sets and maintains */
    USHORT CWidth;	/* Container width (with any relativity absoluted) */
    USHORT CHeight;	/* Container height (with any relativity absoluted) */
    USHORT HPotRes, VPotRes; /* pot increments */
    USHORT LeftBorder;		/* Container borders */
    USHORT TopBorder;		/* Container borders */
};


/* --- FLAG BITS ---------------------------------------------------------- */
#define AUTOKNOB	0x0001	/* this flag sez:  gimme that old auto-knob */
#define FREEHORIZ	0x0002	/* if set, the knob can move horizontally */
#define FREEVERT	0x0004	/* if set, the knob can move vertically */
#define PROPBORDERLESS	0x0008	/* if set, no border will be rendered */
#define KNOBHIT		0x0100	/* set when this Knob is hit */

#define KNOBHMIN	6	/* minimum horizontal size of the Knob */
#define KNOBVMIN	4	/* minimum vertical size of the Knob */
#define MAXBODY		0xFFFF	/* maximum body value */
#define MAXPOT		0xFFFF	/* maximum pot value */






/* ======================================================================== */
/* === StringInfo ========================================================= */
/* ======================================================================== */
/* this is the special data required by the string Gadget
 * typically, this data will be pointed to by the Gadget variable SpecialInfo
 */
struct StringInfo
{
    /* you initialize these variables, and then Intuition maintains them */
    UBYTE *Buffer;	/* the buffer containing the start and final string */
    UBYTE *UndoBuffer;	/* optional buffer for undoing current entry */
    SHORT BufferPos;	/* character position in Buffer */
    SHORT MaxChars;	/* max number of chars in Buffer (including NULL) */
    SHORT DispPos;	/* Buffer position of first displayed character */

    /* Intuition initializes and maintains these variables for you */
    SHORT UndoPos;	/* character position in the undo buffer */
    SHORT NumChars;	/* number of characters currently in Buffer */
    SHORT DispCount;	/* number of whole characters visible in Container */
    SHORT CLeft, CTop;	/* topleft offset of the container */
    struct Layer *LayerPtr;	/* the RastPort containing this Gadget */

    /* you can initialize this variable before the gadget is submitted to
     * Intuition, and then examine it later to discover what integer 
     * the user has entered (if the user never plays with the gadget, 
     * the value will be unchanged from your initial setting)
     */
    LONG LongInt;

    /* If you want this Gadget to use your own Console keymapping, you
     * set the ALTKEYMAP bit in the Activation flags of the Gadget, and then
     * set this variable to point to your keymap.  If you don't set the
     * ALTKEYMAP, you'll get the standard ASCII keymapping.
     */
    struct KeyMap *AltKeyMap;
};






/* ======================================================================== */
/* === IntuiText ========================================================== */
/* ======================================================================== */
/* IntuiText is a series of strings that start with a screen location
 *  (always relative to the upper-left corner of something) and then the
 *  text of the string.	 The text is null-terminated.
 */
struct IntuiText
{
    UBYTE FrontPen, BackPen;	/* the pen numbers for the rendering */
    UBYTE DrawMode;		/* the mode for rendering the text */
    SHORT LeftEdge;		/* relative start location for the text */
    SHORT TopEdge;		/* relative start location for the text */
    struct TextAttr *ITextFont; /* if NULL, you accept the default */
    UBYTE *IText;		/* pointer to null-terminated text */
    struct IntuiText *NextText; /* continuation to TxWrite another text */
};






/* ======================================================================== */
/* === Border ============================================================= */
/* ======================================================================== */
/* Data type Border, used for drawing a series of lines which is intended for
 *  use as a border drawing, but which may, in fact, be used to render any
 *  arbitrary vector shape.
 *  The routine DrawBorder sets up the RastPort with the appropriate
 *  variables, then does a Move to the first coordinate, then does Draws
 *  to the subsequent coordinates.
 *  After all the Draws are done, if NextBorder is non-zero we call DrawBorder
 *  recursively
 */
struct Border
{
    SHORT LeftEdge, TopEdge;	/* initial offsets from the origin */
    UBYTE FrontPen, BackPen;	/* pens numbers for rendering */
    UBYTE DrawMode;		/* mode for rendering */
    BYTE Count;			/* number of XY pairs */
    SHORT *XY;			/* vector coordinate pairs rel to LeftTop*/
    struct Border *NextBorder;	/* pointer to any other Border too */
};






/* ======================================================================== */
/* === Image ============================================================== */
/* ======================================================================== */
/* This is a brief image structure for very simple transfers of 
 * image data to a RastPort
 */
struct Image
{
    SHORT LeftEdge;		/* starting offset relative to some origin */
    SHORT TopEdge;		/* starting offsets relative to some origin */
    SHORT Width;		/* pixel size (though data is word-aligned) */
    SHORT Height, Depth;	/* pixel sizes */
    USHORT *ImageData;		/* pointer to the actual word-aligned bits */

    /* the PlanePick and PlaneOnOff variables work much the same way as the
     * equivalent GELS Bob variables.  It's a space-saving 
     * mechanism for image data.  Rather than defining the image data
     * for every plane of the RastPort, you need define data only 
     * for the planes that are not entirely zero or one.  As you 
     * define your Imagery, you will often find that most of the planes 
     * ARE just as color selectors.  For instance, if you're designing 
     * a two-color Gadget to use colors two and three, and the Gadget 
     * will reside in a five-plane display, bit plane zero of your 
     * imagery would be all ones, bit plane one would have data that 
     * describes the imagery, and bit planes two through four would be 
     * all zeroes.  Using these flags allows you to avoid wasting all 
     * that memory in this way:	 first, you specify which planes you 
     * want your data to appear in using the PlanePick variable.  For 
     * each bit set in the variable, the next "plane" of your image 
     * data is blitted to the display.	For each bit clear in this 
     * variable, the corresponding bit in PlaneOnOff is examined.  
     * If that bit is clear, a "plane" of zeroes will be used.	
     * If the bit is set, ones will go out instead.  So, for our example:
     *	 Gadget.PlanePick = 0x02;
     *	 Gadget.PlaneOnOff = 0x01;
     * Note that this also allows for generic Gadgets, like the 
     * System Gadgets, which will work in any number of bit planes.  
     * Note also that if you want an Image that is only a filled 
     * rectangle, you can get this by setting PlanePick to zero 
     * (pick no planes of data) and set PlaneOnOff to describe the pen 
     * color of the rectangle.	
     */
    UBYTE PlanePick, PlaneOnOff;

    /* if the NextImage variable is not NULL, Intuition presumes that 
     * it points to another Image structure with another Image to be 
     * rendered
     */
    struct Image *NextImage;
};






/* ======================================================================== */
/* === IntuiMessage ======================================================= */
/* ======================================================================== */
struct IntuiMessage
{
    struct Message ExecMessage;

    /* the Class bits correspond directly with the IDCMP Flags, except for the
     * special bit LONELYMESSAGE (defined below)
     */
    ULONG Class;

    /* the Code field is for special values like MENU number */
    USHORT Code;

    /* the Qualifier field is a copy of the current InputEvent's Qualifier */
    USHORT Qualifier;

    /* IAddress contains particular addresses for Intuition functions, like
     * the pointer to the Gadget or the Screen
     */
    APTR IAddress;

    /* when getting mouse movement reports, any event you get will have the
     * the mouse coordinates in these variables.  the coordinates are relative
     * to the upper-left corner of your Window (GIMMEZEROZERO notwithstanding)
     */
    SHORT MouseX, MouseY; 

    /* the time values are copies of the current system clock time.  Micros
     * are in units of microseconds, Seconds in seconds.
     */
    ULONG Seconds, Micros;

    /* the IDCMPWindow variable will always have the address of the Window of 
     * this IDCMP 
     */
    struct Window *IDCMPWindow;

    /* system-use variable */
    struct IntuiMessage *SpecialLink;
};


/* --- IDCMP Classes ------------------------------------------------------ */
#define SIZEVERIFY		0x00000001  /*	See the Programmer's Guide  */
#define NEWSIZE			0x00000002  /*	See the Programmer's Guide  */
#define REFRESHWINDOW		0x00000004  /*	See the Programmer's Guide  */
#define MOUSEBUTTONS		0x00000008  /*	See the Programmer's Guide  */
#define MOUSEMOVE		0x00000010  /*	See the Programmer's Guide  */
#define GADGETDOWN		0x00000020  /*	See the Programmer's Guide  */
#define GADGETUP		0x00000040  /*	See the Programmer's Guide  */
#define REQSET			0x00000080  /*	See the Programmer's Guide  */
#define MENUPICK		0x00000100  /*	See the Programmer's Guide  */
#define CLOSEWINDOW		0x00000200  /*	See the Programmer's Guide  */
#define RAWKEY			0x00000400  /*	See the Programmer's Guide  */
#define REQVERIFY		0x00000800  /*	See the Programmer's Guide  */
#define REQCLEAR		0x00001000  /*	See the Programmer's Guide  */
#define MENUVERIFY		0x00002000  /*	See the Programmer's Guide  */
#define NEWPREFS		0x00004000  /*	See the Programmer's Guide  */
#define DISKINSERTED		0x00008000  /*	See the Programmer's Guide  */
#define DISKREMOVED		0x00010000  /*	See the Programmer's Guide  */
#define WBENCHMESSAGE		0x00020000  /*	See the Programmer's Guide  */
#define ACTIVEWINDOW		0x00040000  /*	See the Programmer's Guide  */
#define INACTIVEWINDOW		0x00080000  /*	See the Programmer's Guide  */
#define DELTAMOVE		0x00100000  /*	See the Programmer's Guide  */
#define VANILLAKEY		0x00200000  /*	See the Programmer's Guide  */
#define INTUITICKS		0x00400000  /*	See the Programmer's Guide  */
/* NOTEZ-BIEN:			0x80000000 is reserved for internal use	  */

/* the IDCMP Flags do not use this special bit, which is cleared when
 * Intuition sends its special message to the Task, and set when Intuition
 * gets its Message back from the Task.	 Therefore, I can check here to
 * find out fast whether or not this Message is available for me to send
 */
#define LONELYMESSAGE	0x80000000


/* --- IDCMP Codes -------------------------------------------------------- */
/* This group of codes is for the MENUVERIFY function */
#define MENUHOT		0x0001	/* IntuiWants verification or MENUCANCEL    */
#define MENUCANCEL	0x0002	/* HOT Reply of this cancels Menu operation */
#define MENUWAITING	0x0003	/* Intuition simply wants a ReplyMsg() ASAP */

/* These are internal tokens to represent state of verification attempts
 * shown here as a clue.
 */
#define OKOK		MENUHOT /* guy didn't care			*/
#define OKABORT		0x0004	/* window rendered question moot	*/
#define OKCANCEL	MENUCANCEL /* window sent cancel reply		*/

/* This group of codes is for the WBENCHMESSAGE messages */
#define WBENCHOPEN	0x0001
#define WBENCHCLOSE	0x0002



/* ======================================================================== */
/* === Window ============================================================= */
/* ======================================================================== */
struct Window
{
    struct Window *NextWindow;		/* for the linked list in a screen */

    SHORT LeftEdge, TopEdge;		/* screen dimensions of window */
    SHORT Width, Height;		/* screen dimensions of window */

    SHORT MouseY, MouseX;		/* relative to upper-left of window */

    SHORT MinWidth, MinHeight;		/* minimum sizes */
    USHORT MaxWidth, MaxHeight;		/* maximum sizes */

    ULONG Flags;			/* see below for defines */

    struct Menu *MenuStrip;		/* the strip of Menu headers */

    UBYTE *Title;			/* the title text for this window */

    struct Requester *FirstRequest;	/* all active Requesters */

    struct Requester *DMRequest;	/* double-click Requester */

    SHORT ReqCount;			/* count of reqs blocking Window */

    struct Screen *WScreen;		/* this Window's Screen */
    struct RastPort *RPort;		/* this Window's very own RastPort */

    /* the border variables describe the window border.	 If you specify
     * GIMMEZEROZERO when you open the window, then the upper-left of the
     * ClipRect for this window will be upper-left of the BitMap (with correct
     * offsets when in SuperBitMap mode; you MUST select GIMMEZEROZERO when
     * using SuperBitMap).  If you don't specify ZeroZero, then you save
     * memory (no allocation of RastPort, Layer, ClipRect and associated
     * Bitmaps), but you also must offset all your writes by BorderTop,
     * BorderLeft and do your own mini-clipping to prevent writing over the
     * system gadgets
     */
    BYTE BorderLeft, BorderTop, BorderRight, BorderBottom;
    struct RastPort *BorderRPort;


    /* You supply a linked-list of Gadgets for your Window.
     * This list DOES NOT include system gadgets.  You get the standard
     * window system gadgets by setting flag-bits in the variable Flags (see
     * the bit definitions below)
     */
    struct Gadget *FirstGadget;

    /* these are for opening/closing the windows */
    struct Window *Parent, *Descendant;

    /* sprite data information for your own Pointer
     * set these AFTER you Open the Window by calling SetPointer()
     */
    USHORT *Pointer;	/* sprite data */
    BYTE PtrHeight;	/* sprite height (not including sprite padding) */
    BYTE PtrWidth;	/* sprite width (must be less than or equal to 16) */
    BYTE XOffset, YOffset;	/* sprite offsets */

    /* the IDCMP Flags and User's and Intuition's Message Ports */
    ULONG IDCMPFlags;	/* User-selected flags */
    struct MsgPort *UserPort, *WindowPort;
    struct IntuiMessage *MessageKey;

    UBYTE DetailPen, BlockPen;	/* for bar/border/gadget rendering */

    /* the CheckMark is a pointer to the imagery that will be used when 
     * rendering MenuItems of this Window that want to be checkmarked
     * if this is equal to NULL, you'll get the default imagery
     */
    struct Image *CheckMark;

    UBYTE *ScreenTitle; /* if non-null, Screen title when Window is active */

    /* These variables have the mouse coordinates relative to the 
     * inner-Window of GIMMEZEROZERO Windows.  This is compared with the
     * MouseX and MouseY variables, which contain the mouse coordinates
     * relative to the upper-left corner of the Window, GIMMEZEROZERO
     * notwithstanding
     */
    SHORT GZZMouseX;
    SHORT GZZMouseY;
    /* these variables contain the width and height of the inner-Window of
     * GIMMEZEROZERO Windows
     */
    SHORT GZZWidth;
    SHORT GZZHeight;

    UBYTE *ExtData;

    BYTE *UserData;	/* general-purpose pointer to User data extension */

    /** jimm: NEW: 11/18/85: this pointer keeps a duplicate of what 
     * Window.RPort->Layer is _supposed_ to be pointing at
     */
    struct Layer *WLayer;

    /* jimm: NEW 1.2: need to keep track of the font that
     * OpenWindow opened, in case user SetFont's into RastPort
     */
    struct TextFont *IFont;
};


/* --- FLAGS REQUESTED (NOT DIRECTLY SET THOUGH) BY THE APPLIPROG --------- */
#define WINDOWSIZING	0x0001	  /* include sizing system-gadget? */
#define WINDOWDRAG	0x0002	  /* include dragging system-gadget? */
#define WINDOWDEPTH	0x0004	  /* include depth arrangement gadget? */
#define WINDOWCLOSE	0x0008	  /* include close-box system-gadget? */

#define SIZEBRIGHT	0x0010	/* size gadget uses right border */
#define SIZEBBOTTOM	0x0020	/* size gadget uses bottom border */

/* --- refresh modes ------------------------------------------------------ */
/* combinations of the REFRESHBITS select the refresh type */
#define REFRESHBITS	0x00C0
#define SMART_REFRESH	0x0000
#define SIMPLE_REFRESH	0x0040
#define SUPER_BITMAP	0x0080
#define OTHER_REFRESH	0x00C0

#define BACKDROP	0x0100	/* this is an ever-popular BACKDROP window */

#define REPORTMOUSE	0x0200	/* set this to hear about every mouse move */

#define GIMMEZEROZERO	0x0400	/* make extra border stuff */

#define BORDERLESS	0x0800	/* set this to get a Window sans border */

#define ACTIVATE	0x1000	/* when Window opens, it's the Active one */

/* FLAGS SET BY INTUITION */
#define WINDOWACTIVE  0x2000	/* this window is the active one */
#define INREQUEST     0x4000	/* this window is in request mode */
#define MENUSTATE     0x8000	/* this Window is active with its Menus on */

/* --- Other User Flags --------------------------------------------------- */
#define RMBTRAP		0x00010000	/* Catch RMB events for your own */
#define NOCAREREFRESH	0x00020000	/* not to be bothered with REFRESH */


/* --- Other Intuition Flags ---------------------------------------------- */
#define WINDOWREFRESH	0x01000000	/* Window is currently refreshing */
#define WBENCHWINDOW	0x02000000	/* WorkBench tool ONLY Window */
#define WINDOWTICKED	0x04000000	/* only one timer tick at a time */

#define SUPER_UNUSED	0xFCFC0000	/* bits of Flag unused yet */


/* --- see struct IntuiMessage for the IDCMP Flag definitions ------------- */




/* ======================================================================== */
/* === NewWindow ========================================================== */
/* ======================================================================== */
struct NewWindow
{
    SHORT LeftEdge, TopEdge;		/* screen dimensions of window */
    SHORT Width, Height;		/* screen dimensions of window */

    UBYTE DetailPen, BlockPen;		/* for bar/border/gadget rendering */

    ULONG IDCMPFlags;			/* User-selected IDCMP flags */

    ULONG Flags;			/* see Window struct for defines */

    /* You supply a linked-list of Gadgets for your Window.
     *	This list DOES NOT include system Gadgets.  You get the standard
     *	system Window Gadgets by setting flag-bits in the variable Flags (see
     *	the bit definitions under the Window structure definition)
     */
    struct Gadget *FirstGadget;

    /* the CheckMark is a pointer to the imagery that will be used when 
     * rendering MenuItems of this Window that want to be checkmarked
     * if this is equal to NULL, you'll get the default imagery
     */
    struct Image *CheckMark;

    UBYTE *Title;			  /* the title text for this window */
    
    /* the Screen pointer is used only if you've defined a CUSTOMSCREEN and
     * want this Window to open in it.	If so, you pass the address of the
     * Custom Screen structure in this variable.  Otherwise, this variable
     * is ignored and doesn't have to be initialized.
     */
    struct Screen *Screen;
    
    /* SUPER_BITMAP Window?  If so, put the address of your BitMap structure
     * in this variable.  If not, this variable is ignored and doesn't have 
     * to be initialized
     */
    struct BitMap *BitMap;

    /* the values describe the minimum and maximum sizes of your Windows.
     * these matter only if you've chosen the WINDOWSIZING Gadget option,
     * which means that you want to let the User to change the size of 
     * this Window.  You describe the minimum and maximum sizes that the
     * Window can grow by setting these variables.  You can initialize
     * any one these to zero, which will mean that you want to duplicate
     * the setting for that dimension (if MinWidth == 0, MinWidth will be
     * set to the opening Width of the Window).
     * You can change these settings later using SetWindowLimits().
     * If you haven't asked for a SIZING Gadget, you don't have to
     * initialize any of these variables.
     */
    SHORT MinWidth, MinHeight;	     /* minimums */
    USHORT MaxWidth, MaxHeight;	      /* maximums */

    /* the type variable describes the Screen in which you want this Window to
     * open.  The type value can either be CUSTOMSCREEN or one of the
     * system standard Screen Types such as WBENCHSCREEN.  See the
     * type definitions under the Screen structure
     */
    USHORT Type;
};


#ifndef INTUITION_SCREENS_H
#include "intuition/screens.h"
#endif

#ifndef INTUITION_PREFERENCES_H
#include "intuition/preferences.h"
#endif

/* ======================================================================== */
/* === Remember =========================================================== */
/* ======================================================================== */
/* this structure is used for remembering what memory has been allocated to
 * date by a given routine, so that a premature abort or systematic exit
 * can deallocate memory cleanly, easily, and completely
 */
struct Remember
{
    struct Remember *NextRemember;
    ULONG RememberSize;
    UBYTE *Memory;
};





/* ======================================================================== */
/* === Miscellaneous ====================================================== */
/* ======================================================================== */

/* = MACROS ============================================================== */
#define MENUNUM(n) (n & 0x1F)
#define ITEMNUM(n) ((n >> 5) & 0x003F)
#define SUBNUM(n) ((n >> 11) & 0x001F)

#define SHIFTMENU(n) (n & 0x1F)
#define SHIFTITEM(n) ((n & 0x3F) << 5)
#define SHIFTSUB(n) ((n & 0x1F) << 11)


#define SRBNUM(n)    (0x08 - (n >> 4))	/* SerRWBits -> read bits per char */
#define SWBNUM(n)    (0x08 - (n & 0x0F))/* SerRWBits -> write bits per chr */
#define SSBNUM(n)    (0x01 + (n >> 4))	/* SerStopBuf -> stop bits per chr */
#define SPARNUM(n)   (n >> 4)		/* SerParShk -> parity setting	  */
#define SHAKNUM(n)   (n & 0x0F)		/* SerParShk -> handshake mode	  */


/* = MENU STUFF =========================================================== */
#define NOMENU 0x001F
#define NOITEM 0x003F
#define NOSUB  0x001F
#define MENUNULL 0xFFFF


/* = =RJ='s peculiarities ================================================= */
#define FOREVER for(;;)
#define SIGN(x) ( ((x) > 0) - ((x) < 0) )
#define NOT !

/* these defines are for the COMMSEQ and CHECKIT menu stuff.  If CHECKIT,
 * I'll use a generic Width (for all resolutions) for the CheckMark.
 * If COMMSEQ, likewise I'll use this generic stuff
 */
#define CHECKWIDTH	19
#define COMMWIDTH	27
#define LOWCHECKWIDTH	13
#define LOWCOMMWIDTH	16


/* these are the AlertNumber defines.  if you are calling DisplayAlert()
 * the AlertNumber you supply must have the ALERT_TYPE bits set to one
 * of these patterns
 */
#define ALERT_TYPE	0x80000000
#define RECOVERY_ALERT	0x00000000	/* the system can recover from this */
#define DEADEND_ALERT	0x80000000	/* no recovery possible, this is it */


/* When you're defining IntuiText for the Positive and Negative Gadgets 
 * created by a call to AutoRequest(), these defines will get you 
 * reasonable-looking text.  The only field without a define is the IText
 * field; you decide what text goes with the Gadget
 */
#define AUTOFRONTPEN	0
#define AUTOBACKPEN	1
#define AUTODRAWMODE	JAM2
#define AUTOLEFTEDGE	6
#define AUTOTOPEDGE	3
#define AUTOITEXTFONT	NULL
#define AUTONEXTTEXT	NULL


/* --- RAWMOUSE Codes and Qualifiers (Console OR IDCMP) ------------------- */
#define SELECTUP	(IECODE_LBUTTON | IECODE_UP_PREFIX)
#define SELECTDOWN	(IECODE_LBUTTON)
#define MENUUP		(IECODE_RBUTTON | IECODE_UP_PREFIX)
#define MENUDOWN	(IECODE_RBUTTON)
#define ALTLEFT		(IEQUALIFIER_LALT)
#define ALTRIGHT	(IEQUALIFIER_RALT)
#define AMIGALEFT	(IEQUALIFIER_LCOMMAND)
#define AMIGARIGHT	(IEQUALIFIER_RCOMMAND)
#define AMIGAKEYS	(AMIGALEFT | AMIGARIGHT)

#define CURSORUP	0x4C
#define CURSORLEFT	0x4F
#define CURSORRIGHT	0x4E
#define CURSORDOWN	0x4D
#define KEYCODE_Q	0x10
#define KEYCODE_X	0x32
#define KEYCODE_N	0x36
#define KEYCODE_M	0x37
#define KEYCODE_V	0x34
#define KEYCODE_B	0x35

#endif	/* INTUITION_INTUITION_H */
