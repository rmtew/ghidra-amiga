// Intuition/Intuition.h for C++
#ifndef INTUITION_INTUITION_H
#define INTUITION_INTUITION_H TRUE

#ifndef INTUITION_REV
#define INTUITION_REV 33	/* Current version of Intuition (1.2) */
#endif

#ifndef STRING_H
   #include <string.h>
#endif

/*** intuition.h *************************************************************
 *
 *  intuition.h main include for c programmers
 *
 *  MTS C++ version 1.2 Copyright (C) 1986 by MTS Associates, Inc.
 *  Portions Copyright (c) Commodore-Amiga Computer, Inc.
 *				Modification History
 *      date    :   author :    Comments
 *     ------       ------      -------------------------------------
 *     1-30-85	    -=RJ=-	created this file!
 *     1-12-87	     TFH	Made it work with C++
 *
 ****************************************************************************
 * CONFIDENTIAL and PROPRIETARY
 * Copyright (C) 1985, COMMODORE-AMIGA, INC.
 * All Rights Reserved
 ****************************************************************************/

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

#ifndef EXEC_ALERTS_H
#include <exec/alerts.h>
#endif

#ifndef EXEC_PORTS_H
#include <exec/ports.h>
#endif

#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif

#ifndef GRAPHICS_RASTPORT_H
#include <graphics/rastport.h>
#endif

#ifndef GRAPHICS_VIEW_H
#include <graphics/view.h>
#endif

#ifndef GRAPHICS_LAYERS_H
#include <graphics/layers.h>
#endif

#ifndef DEVICES_TIMER_H
#include <devices/timer.h>
#endif

#ifndef DEVICES_INPUTEVENT_H
#include <devices/inputevent.h>
#endif

class Window;		typedef Window *PWindow;
class Requester;	typedef Requester *PRequester;
class IntuiText;	typedef IntuiText *PIntuiText;
class Gadget;		typedef Gadget *PGadget;
class BoolInfo;
class StringInfo;
class PropInfo;
class Layer;
class Layer_Info;
class KeyMap;

#define AT_END 65535

/* === PIntuition ========================================================= */

class pIntuition {
	class IntuitionBase *ib;
	short open_count;
public:
    pIntuition(int version)
    {
	if (open_count++ == 0)
	{
	    ib = (IntuitionBase *) OpenLibrary("intuition.library", version);
	    if (ib == NULL)
	    {
		Alert(AT_Recovery|AG_OpenLib|AO_Intuition);
		exit(20);
	    }
	}
    }

    ~pIntuition()
    { 
	if (--open_count == 0) CloseLibrary( (Library *) ib);
    }
    IntuitionBase *IntuitionBase() { return ib; }
};

pIntuition IntuitionBase(INTUITION_REV);

/* === IntuiText ========================================================= */

class TextAttr;

struct IntuiText
{
    UBYTE FrontPen, BackPen;
    UBYTE DrawMode;
    SHORT LeftEdge;
    SHORT TopEdge;
    TextAttr *ITextFont;
    TEXT  *IText;
    PIntuiText NextText;

	PIntuiText next() { return NextText; }

	friend int IntuiTextLength(PIntuiText);
	friend void PrintIText(RastPort *r, PIntuiText,
		SHORT left, SHORT top);
	friend RastPort& operator<<(RastPort& rp, IntuiText& itx)
	{ PrintIText(&rp, &itx, 0, 0); return rp; }
	friend IntuiText& operator<<(IntuiText& itx, char *str)
	{ itx.IText = str; return itx; }	// Assign POINTER only!
};

enum MenuFlags {
// FLAGS SET BY THE APPLIPROG:
 CHECKIT	= 0x0001, 	// whether to check this item if selected
 ITEMTEXT	= 0x0002, 	// set if textual, clear if graphical item
 COMMSEQ	= 0x0004, 	// set if there's an command sequence
 MENUTOGGLE	= 0x0008, 	// set to toggle the check of a menu item
 ITEMENABLED	= 0x0010, 	// set if this item is enabled
 
// these are the SPECIAL HIGHLIGHT FLAG state meanings:
 HIGHFLAGS	= 0x00C0,
 HIGHIMAGE	= 0x0000,	// use the user's "select image"
 HIGHCOMP	= 0x0040,	// highlight by complementing the selectbox
 HIGHBOX	= 0x0080,	// highlight by "boxing" the selectbox
 HIGHNONE	= 0x00C0,	// don't highlight

/* FLAGS SET BY BOTH APPLIPROG AND INTUITION */ 
 CHECKED	= 0x0100,	// if CHECKIT, then set this when selected
 
// FLAGS SET BY INTUITION:
 ISDRAWN	= 0x1000,	// this item's subs are currently drawn
 HIGHITEM	= 0x2000,	// this item is currently highlighted
 MENUTOGGLED	= 0x4000	// this item was already toggled
};

/* === MenuItem ========================================================== */

struct MenuItem 
{
    MenuItem *NextItem;
    SHORT LeftEdge, TopEdge;
    SHORT Width, Height;
    USHORT Flags;
    LONG MutualExclude;
    APTR ItemFill;
    APTR SelectFill;
    BYTE Command;		// only if appliprog sets the COMMSEQ flag
    MenuItem *SubItem;		// if non-zero, DrawMenu shows "->"

// The NextSelect field represents the menu number of next selected 
// item (when user has drag-selected several items).

    USHORT NextSelect;

	MenuItem *next() { return NextItem; }
};

/* === Menu ============================================================== */

const NOMENU = 0x001F;
const NOITEM = 0x003F;
const NOSUB  = 0x001F;
const MENUNULL = 0xFFFF;

/* these defines are for the COMMSEQ and CHECKIT menu stuff.  If CHECKIT,
 * I'll use a generic Width (for all resolutions) for the CheckMark.
 * If COMMSEQ, likewise I'll use this generic stuff
 */
const CHECKWIDTH = 19;
const COMMWIDTH 	= 27;
const LOWCHECKWIDTH	= 13;
const LOWCOMMWIDTH	= 16;

struct Menu
{
    Menu *NextMenu;
    SHORT LeftEdge, TopEdge;
    SHORT Width, Height;
    USHORT Flags;
    BYTE *MenuName;
    MenuItem *FirstItem;

    SHORT JazzX, JazzY, BeatX, BeatY;	// internal use only

	Menu *next() { return NextMenu; }
	
friend MenuItem *ItemAddress(Menu&, USHORT);
};

inline MENUNUM(int n) { return (n & 0x1F); }
inline ITEMNUM(int n) { return ((n >> 5) & 0x003F); }
inline SUBNUM(int n)  { return ((n >> 11) & 0x001F); }

inline SHIFTMENU(int n) { return (n & 0x1F); }
inline SHIFTITEM(int n) { return ((n & 0x3F) << 5); }
inline SHIFTSUB(int n) { return ((n & 0x1F) << 11); }

/* FLAGS SET BY BOTH THE APPLIPROG AND INTUITION */
#define MENUENABLED 0x0001	/* whether or not this menu is enabled */

/* FLAGS SET BY INTUITION */
#define MIDRAWN 0x0100		/* this menu's items are currently drawn */

/* === Gadget ============================================================ */

struct Gadget
{
    PGadget NextGadget;
    SHORT LeftEdge, TopEdge;
    SHORT Width, Height;
    USHORT Flags;
    USHORT Activation;
    USHORT GadgetType;
    APTR GadgetRender;		// Border, Image, or NULL
    APTR SelectRender;
    PIntuiText GadgetText;
    LONG MutualExclude;

    // pointer to a structure of special data required by Proportional,
    // String and Integer Gadgets

    APTR SpecialInfo;
    // tame the wild type-miscast!
	BoolInfo   *Bool() { return (BoolInfo *) SpecialInfo; }
	StringInfo *String() { return (StringInfo *) SpecialInfo; }
	PropInfo   *Prop() { return (PropInfo *) SpecialInfo; }

    USHORT GadgetID;	/* user-definable ID field */
    APTR UserData;	// ptr to general purpose User data (ignored by In)

	PGadget next() { return NextGadget; }

friend	USHORT AddGadget(PWindow, PGadget,
	SHORT pos=AT_END);

friend	USHORT AddGList(PWindow,  PGadget,
	SHORT pos=AT_END, USHORT number=1,
	PRequester r=NULL);

friend	BOOL ActivateGadget(PGadget, PWindow,
	PRequester r=NULL);

friend	void ModifyProp(PGadget, PWindow, PRequester r,
	USHORT flags,
	USHORT Hpot, USHORT Vpot,
	USHORT Hbody, USHORT Vbody);

friend	void NewModifyProp(PGadget, PWindow, PRequester,
	USHORT flags,
	USHORT Hpot, USHORT Vpot,
	USHORT Hbody, USHORT Vbody,
	int i);

friend	void OffGadget(PGadget, PWindow, PRequester r=NULL);

friend	void OnGadget(PGadget, PWindow,
	PRequester r=NULL);

friend	void RefreshGadgets( PGadget, PWindow,
	PRequester r=NULL);

friend	void RefreshGList(PGadget, PWindow,
	PRequester, SHORT num=1);

friend	USHORT RemoveGadget(PWindow, PGadget);

friend	USHORT RemoveGList(PWindow, PGadget,
	USHORT numgads);

	void operator<<(char *);
};


/* --- FLAGS SET BY THE APPLIPROG ----------------------------------------- */
/* combinations in these bits describe the highlight technique to be used */
#define GADGHIGHBITS	0x0003
#define GADGHCOMP	0x0000	/* Complement the select box */
#define GADGHBOX	0x0001	/* Draw a box around the image */
#define GADGHIMAGE	0x0002	/* Blast in this alternate image */
#define GADGHNONE	0x0003	/* don't highlight */

#define GADGIMAGE 0x0004 

#define GRELBOTTOM	0x0008
#define GRELRIGHT	0x0010
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


// These are the Activation flag bits:
#define RELVERIFY	0x0001
#define GADGIMMEDIATE	0x0002
#define ENDGADGET	0x0004
#define FOLLOWMOUSE	0x0008

// if any of the BORDER flags are set in a Gadget that's included in the
// Gadget list when a Window is opened, the corresponding Border will
// be adjusted to make room for the Gadget.

#define RIGHTBORDER	0x0010
#define LEFTBORDER	0x0020
#define TOPBORDER	0x0040
#define BOTTOMBORDER	0x0080

#define TOGGLESELECT	0x0100

#define STRINGCENTER	0x0200
#define STRINGRIGHT	0x0400

#define LONGINT		0x0800

#define ALTKEYMAP	0x1000

#define BOOLEXTEND	0x2000

/* --- GADGET TYPES ------------------------------------------------------- */

#define GADGETTYPE	0xFC00	/* all Gadget Global Type flags (padded) */
#define SYSGADGET	0x8000	/* 1 = SysGadget, 0 = AppliPGadget */
#define SCRGADGET	0x4000  /* 1 = ScreenGadget, 0 = WindowGadget */
#define GZZGADGET	0x2000  /* 1 = Gadget for GIMMEZEROZERO borders */
#define REQGADGET	0x1000  /* 1 = this is a Requester Gadget */
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



/* === BoolInfo======================================================= */

struct BoolInfo
{
    USHORT Flags;
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
#define BOOLMASK	0x0001	// extension is for masked Gadget

/* === PropInfo =========================================================== */

struct PropInfo
{
    USHORT Flags;
    USHORT HorizPot;
    USHORT VertPot;
    USHORT HorizBody;
    USHORT VertBody;

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

#define KNOBHMIN 	6	/* minimum horizontal size of the Knob */
#define KNOBVMIN 	4	/* minimum vertical size of the Knob */
#define MAXBODY		0xFFFF	/* maximum body value */
#define MAXPOT 		0xFFFF	/* maximum pot value */

/* === StringInfo ========================================================= */

struct StringInfo
{
    /* you initialize these variables, and then Intuition maintains them */
    UBYTE *Buffer;
    UBYTE *UndoBuffer;
    SHORT BufferPos;
    SHORT MaxChars;
    SHORT DispPos;	/* Buffer position of first displayed character */

    /* Intuition initializes and maintains these variables for you */
    SHORT UndoPos;
    SHORT NumChars;
    SHORT DispCount;
    SHORT CLeft, CTop;
    Layer *LayerPtr;

    LONG LongInt;
    KeyMap *AltKeyMap;
};

inline void
Gadget::operator<<(char *str)
{
	strcpy(String()->Buffer, str);
}

/* === Border ============================================================ */

struct Border
{
    SHORT LeftEdge, TopEdge;
    UBYTE FrontPen, BackPen;
    UBYTE DrawMode;
    BYTE Count;			/* number of XY pairs */
    SHORT *XY;
    Border *NextBorder;

	Border *next() { return NextBorder; }

friend void DrawBorder (RastPort *rp, Border *b,
	SHORT left, SHORT top);
};

/* === Image ============================================================== */

struct Image
{
    WORD LeftEdge;
    WORD TopEdge;
    UWORD Width, Height, Depth;
    UWORD *ImageData;
    UBYTE PlanePick, PlaneOnOff;
    Image *NextImage;

    Image *next() { return NextImage; }
};

void DrawImage (RastPort *r, Image *i,
	SHORT left, SHORT top);

/************************************************************************/

// --- IDCMP Classes

enum IDCMPclass {
     SIZEVERIFY		= 0x00000001,
     NEWSIZE		= 0x00000002,
     REFRESHWINDOW	= 0x00000004,
     MOUSEBUTTONS	= 0x00000008,
     MOUSEMOVE		= 0x00000010,
     GADGETDOWN		= 0x00000020,
     GADGETUP		= 0x00000040,
     REQSET		= 0x00000080,
     MENUPICK		= 0x00000100,
     CLOSEWINDOW	= 0x00000200,
     RAWKEY		= 0x00000400,
     REQVERIFY		= 0x00000800,
     REQCLEAR		= 0x00001000,
     MENUVERIFY		= 0x00002000,
     NEWPREFS		= 0x00004000,
     DISKINSERTED	= 0x00008000,
     DISKREMOVED	= 0x00010000,
     WBENCHMESSAGE	= 0x00020000,
     ACTIVEWINDOW	= 0x00040000,
     INACTIVEWINDOW	= 0x00080000,
     DELTAMOVE		= 0x00100000,
     VANILLAKEY		= 0x00200000,
     INTUITICKS		= 0x00400000,
    LONELYMESSAGE	= 0x80000000	// internal use only!
};

/* --- IDCMP Codes -------------------------------------------------------- */
/* This group of codes is for the MENUVERIFY function */
#define MENUHOT		0x0001	/* IntuiWants verification or MENUCANCEL    */
#define MENUCANCEL	0x0002	/* HOT Reply of this cancels Menu operation */
#define MENUWAITING	0x0003	/* Intuition simply wants a ReplyMsg() ASAP */

/* These are internal tokens to represent state of verification attempts
 * shown here as a clue.
 */
#define OKOK		MENUHOT	/* guy didn't care			*/
#define OKABORT		0x0004	/* window rendered question moot	*/
#define OKCANCEL 	MENUCANCEL /* window sent cancel reply		*/

/* This group of codes is for the WBENCHMESSAGE messages */
#define WBENCHOPEN	0x0001
#define WBENCHCLOSE	0x0002

typedef LONGBITS IDCMPFlags;

//class IDCMPFlags {
//	LONGBITS flags;
//public:
//
//	IDCMPFlags() { flags = 0; }
//	IDCMPFlags(LONGBITS i) { flags = i; }
//	operator LONGBITS() { return flags; }
//
//	friend IDCMPFlags operator|(IDCMPFlags a, IDCMPFlags b)
//		{ return a.flags | b.flags; }
//
//};

typedef ULONG IntuitionLock;


/* ======================================================================= */
/* === IntuiMessage ======================================================= */
/* ======================================================================== */

class IntuiMessage : public Message
{
// the Class bits correspond directly with the IDCMP Flags, except for the
// special bit LONELYMESSAGE (defined above).
public:
    const ULONG Class;
    const USHORT Code;
    const USHORT Qualifier;
    const APTR IAddress;
    const SHORT MouseX, MouseY; // relative to upper left corner of window.
    const ULONG Seconds, Micros; // timeval - C compiler bug
    PWindow IDCMPWindow;
private:
    /* system-use variable */
    IntuiMessage *SpecialLink;
};

/* === NewScreen ========================================================= */

struct NewScreen
{
    SHORT LeftEdge, TopEdge, Width, Height, Depth;  // screen dimensions
    UBYTE DetailPen, BlockPen;	// for bar/border/gadget rendering
    USHORT ViewModes;		// the Modes for the ViewPort (and View)
    USHORT Type;		//the Screen type (see defines above)
    TextAttr *Font;
    UBYTE *DefaultTitle;
    PGadget Gadgets;
    BitMap *CustomBitMap;
};


/* === Screen ============================================================= */

// extern int strlen(const char *s); - in string.h

class Screen
{
// Hidden read-only definitions.  Use screen->next() to chase screens
    Screen *NextScreen;		// linked list of screens
public:
    PWindow FirstWindow;	// linked list Screen's Windows

    SHORT LeftEdge, TopEdge;	// parameters of the screen
    SHORT Width, Height;

    SHORT MouseY, MouseX;

    USHORT Flags;		// see definitions below

    UBYTE *Title;		// null-terminated Title text
    UBYTE *DefaultTitle;	// for Windows without ScreenTitle
// Bar sizes for this Screen and all Windows in this Screen:
    BYTE BarHeight, BarVBorder, BarHBorder, MenuVBorder, MenuHBorder;
    BYTE WBorTop, WBorLeft, WBorRight, WBorBottom;

    TextAttr *Font;		// this screen's default font

    /* the display data structures for this Screen */
    ViewPort ViewPort;		// describing the Screen's display
    RastPort RastPort;		// describing Screen rendering
    BitMap BitMap;		// extra copy of RastPort BitMap
    Layer_Info LayerInfo;
    PGadget FirstGadget;

    UBYTE DetailPen, BlockPen;
    USHORT SaveColor0;		// used by DisplayBeep
    Layer *BarLayer;    // This layer is for the Screen and Menu bars
    UBYTE *ExtData;

    UBYTE *UserData;	// general-purpose pointer to User data extension

//  C++ goodies:
public:
	friend Screen *OpenScreen(NewScreen *ns);

	Screen (NewScreen *s)		// create a new screen
	{
		this = OpenScreen(s);
	}

	friend void CloseScreen (Screen *s);

	~Screen()			// make it go away
	{
		CloseScreen(this);
		this = NULL;
	}

	Screen *next() { return NextScreen; }
	class RastPort *graphic() { return &RastPort; }

friend void DisplayBeep(Screen *s);
friend BOOL GetScreenData(void *x, USHORT a, USHORT b,
	 Screen *s);
friend void MakeScreen(Screen *s);
friend void MoveScreen(Screen *s, SHORT dx, SHORT dy);
friend Screen * OpenWorkBench();
friend void ScreenToBack(Screen *s);
friend void ScreenToFront(Screen *s);
friend void ShowTitle(Screen *s, BOOL offon);
};


/* --- FLAGS SET BY INTUITION --------------------------------------------- */
#define SCREENTYPE	0x000F	/* all the screens types available	*/
/* --- the definitions for the Screen Type ------------------------------- */
enum ScreenType {
	WBENCHSCREEN	= 0x0001,
	CUSTOMSCREEN	= 0x000F,
	SHOWTITLE	= 0x0010,
	BEEPING		= 0x0020,
	CUSTOMBITMAP	= 0x0040,
	SCREENBEHIND	= 0x0080,
	SCREENQUIET 	= 0x0100
};
#define STDSCREENHEIGHT -1	/* supply in NewScreen.Height		*/

/* === NewWindow ========================================================= */

struct NewWindow
{
    SHORT LeftEdge, TopEdge;
    SHORT Width, Height;
    UBYTE DetailPen, BlockPen;
    IDCMPFlags IDCMPFlags;
    ULONG Flags;
    PGadget FirstGadget;
    Image *CheckMark;		// image used for checkmarking menus
    UBYTE *Title;
    Screen *Screen;
    BitMap *BitMap;
    USHORT MinWidth, MinHeight;
    USHORT MaxWidth, MaxHeight;
    USHORT Type;		// Type of screen to open window on
};

/* === Window ============================================================ */

class Window
{
// Use window->next() for this one.
    PWindow NextWindow;	// for the linked list in a screen 
public:
    SHORT LeftEdge, TopEdge;
    SHORT Width, Height;
    SHORT MouseY, MouseX;
    SHORT MinWidth, MinHeight;
    USHORT MaxWidth, MaxHeight;
    ULONG Flags;
    Menu *MenuStrip;
    UBYTE *Title;

    PRequester	FirstRequest;
    PRequester	DMRequest;
    SHORT	ReqCount;		/* count of reqs blocking Window */
    Screen	*WScreen;
    RastPort	*RPort;
    BYTE BorderLeft, BorderTop, BorderRight, BorderBottom;
    RastPort	*BorderRPort;
    Gadget	*FirstGadget;

    /* these are for opening/closing the windows */
    PWindow Parent, *Descendant;

    /* sprite data information for your own Pointer
     * set these AFTER you Open the Window by calling SetPointer()
     */
    USHORT *Pointer;	/* sprite data */
    BYTE PtrHeight;
    BYTE PtrWidth;
    BYTE XOffset, YOffset;

    /* the IDCMP Flags and User's and Intuition's Message Ports */
    IDCMPFlags IDCMPFlags;
    MsgPort	*UserPort, *WindowPort;
    IntuiMessage *MessageKey;

    UBYTE DetailPen, BlockPen; /* for bar/border/gadget rendering */
    Image *CheckMark;

    UBYTE *ScreenTitle;	/* if non-null, Screen title when Window is active */
    SHORT GZZMouseX;
    SHORT GZZMouseY;
    /* these variables contain the width and height of the inner-Window of
     * GIMMEZEROZERO Windows
     */
    SHORT GZZWidth;
    SHORT GZZHeight;

    UBYTE *ExtData;

    void *UserData;	/* general-purpose pointer to User data extension */

    /** jimm: NEW: 11/18/85: this pointer keeps a duplicate of what 
     * Window.RPort->Layer is _supposed_ to be pointing at
     */
    Layer *WLayer;

//  C++ goodies:
public:
	friend PWindow OpenWindow (NewWindow *nw);
	Window (NewWindow* nw)		// create a new window
	{
		this = OpenWindow(nw);
	}
	Window (NewWindow* nw, Screen *s)	// create a new window
	{
		nw->Screen = s;
		nw->Type = CUSTOMSCREEN;
		this = OpenWindow(nw);
	}

	friend void CloseWindow(PWindow);
	~Window()			// make it go away
	{
		CloseWindow(this);
		this = NULL;
	}

	PWindow next() { return NextWindow; }
	Screen *screen() { return WScreen; }
	PGadget gadget() { return FirstGadget; }
	PRequester requester() { return FirstRequest; }
	Menu *menu() { return MenuStrip; }
//	SigBits SigBits(PWindow w) { return SigBits::SigBits(1L << w->UserPort->mp_SigBit); }
	IntuiMessage *message() { return (IntuiMessage*) GetMsg(UserPort); }
	RastPort *graphic() { return RPort; }

//$$$$$ Gadgets

 USHORT add(PGadget g,
		SHORT pos=AT_END, USHORT num=1)
	{return AddGList (this, g, pos, num, NULL);}
 USHORT activate(PGadget g)
	{return ActivateGadget (g, this, NULL);}
 void turn_off(PGadget g) {OffGadget (g, this, NULL);}
 void turn_on(PGadget g) {OnGadget (g, this, NULL);}
 void refresh(PGadget g, USHORT num=65535)
	{ RefreshGList(g, this, NULL, num);}
 USHORT remove(PGadget g, USHORT num)
        { return RemoveGList(this, g, num);}
};

void ActivateWindow(PWindow);

BOOL AutoRequest(PWindow,
	PIntuiText body,
	PIntuiText sayyes,
	PIntuiText sayno,
	IDCMPFlags yesflags,
	IDCMPFlags noflags,
	SHORT w, SHORT h);

PWindow BuildSysRequest(PWindow,
	PIntuiText body,
	PIntuiText sayyes,
	PIntuiText sayno,
	IDCMPFlags f,
	SHORT w, SHORT h);


void ClearMenuStrip(PWindow);
void SetPointer(PWindow, USHORT * a,
	SHORT b, SHORT c,
	SHORT d, SHORT e);

void ClearPointer(PWindow);
void BeginRefresh(PWindow);
void EndRefresh(PWindow, BOOL ok);
void FreeSysRequest(PWindow);
void ModifyIDCMP(PWindow, IDCMPFlags ff);
void MoveWindow(PWindow, SHORT dx, SHORT dy);
void RefreshWindowFrame(PWindow);
void ReportMouse(BOOL offon, PWindow);


BOOL ClearDMRequest(PWindow);
void SetWindowTitles(PWindow, TEXT * win, TEXT * screen);
void SizeWindow(PWindow, SHORT dx, SHORT dy);
ViewPort *ViewPortAddress(PWindow);
BOOL WindowLimits(PWindow,
	SHORT, SHORT,	/* min width, height */
	SHORT, SHORT);	/* max width, height */
void WindowToBack(PWindow);
void WindowToFront(PWindow);

void OffMenu(PWindow, USHORT item);
void OnMenu(PWindow, USHORT item);
BOOL SetMenuStrip(PWindow, Menu *m);

enum WindowFlags {
/* --- FLAGS REQUESTED (NOT DIRECTLY SET THOUGH) BY THE APPLIPROG -------- */
	WINDOWSIZING	= 0x0001,	/* system gadget options */
	WINDOWDRAG	= 0x0002,
	WINDOWDEPTH	= 0x0004,
	WINDOWCLOSE	= 0x0008,

	SIZEBRIGHT	= 0x0010,	/* size gadget uses right border */
	SIZEBBOTTOM	= 0x0020,

/* --- refresh modes ----------------------------------------------------- */
/* combinations of the REFRESHBITS select the refresh type */
	REFRESHBITS	= 0x00C0,
	SMART_REFRESH	= 0x0000,
	SIMPLE_REFRESH	= 0x0040,
	SUPER_BITMAP	= 0x0080,
	OTHER_REFRESH	= 0x00C0,

	BACKDROP	= 0x0100,
	REPORTMOUSE	= 0x0200,
	GIMMEZEROZERO	= 0x0400,
	BORDERLESS	= 0x0800,
	ACTIVATE	= 0x1000,

/* FLAGS SET BY INTUITION */
	WINDOWACTIVE	= 0x2000,
	INREQUEST	= 0x4000,
	MENUSTATE	= 0x8000,

/* --- Other User Flags --------------------------------------------------- */
	RMBTRAP		= 0x00010000,
	NOCAREREFRESH	= 0x00020000,


/* --- Other Intuition Flags ---------------------------------------------- */
	WINDOWREFRESH	= 0x01000000,
	WBENCHWINDOW	= 0x02000000,
	WINDOWTICKED	= 0x04000000,
	SUPER_UNUSED	= 0xFCFC0000
};

/* --- see IntuiMessage for the IDCMP Flag definitions ------------- */

/* === Requester ========================================================= */

struct Requester
{
    PRequester OlderRequest;
    SHORT LeftEdge, TopEdge;
    SHORT Width, Height;
    SHORT RelLeft, RelTop;

    PGadget ReqGadget;
    Border *ReqBorder;
    PIntuiText ReqText;
    USHORT Flags;

    /* pen number for back-plane fill before draws */
    UBYTE BackFill;
    /* Layer in place of clip rect	*/
    Layer *ReqLayer;

    UBYTE ReqPad1[32];

    /* If the BitMap plane pointers are non-zero, this tells the system 
     * that the image comes pre-drawn (if the appliprog wants to define 
     * it's own box, in any shape or size it wants!);  this is OK by 
     * Intuition as long as there's a good correspondence between 
     * the image and the specified Gadgets
     */
    BitMap *ImageBMap;	/* points to the BitMap of PREDRAWN imagery */
    PWindow RWindow;	/* added.  points back to Window */
    UBYTE ReqPad2[36];

friend void EndRequest(PRequester, PWindow);
friend void InitRequester(PRequester);
//$$$$$ Gadgets

 USHORT add(PGadget g,
		USHORT pos=AT_END, USHORT num=1)
	{ return AddGList (this->RWindow, g, pos, num, this);}
 USHORT activate(PGadget g)
	{ return ActivateGadget (g, this->RWindow, this);}
 void turn_off(PGadget g) {OffGadget (g, this->RWindow, this);}
 void turn_on(PGadget g) {OffGadget (g, this->RWindow, this);}
 void refresh(PGadget g, USHORT num=65535)
	{ RefreshGList(g, this->RWindow, this, num);}
 USHORT remove(PGadget g, USHORT num)
        { return RemoveGList((PWindow)this, g, num);}

 RastPort *RastPort() { return ReqLayer->rp; }
 Window   *Window() { return RWindow; }
 PRequester prev()  { return OlderRequest; }
};

BOOL Request(PRequester, PWindow);
void SetDMRequest(PWindow, PRequester);

/* FLAGS SET BY THE APPLIPROG */
#define POINTREL 0x0001  /* if POINTREL set, TopLeft is relative to pointer*/
#define PREDRAWN 0x0002  /* if ReqBMap points to predrawn Requester imagery */
#define NOISYREQ 0x0004	 /* if you don't want requester to filter input	   */
/* FLAGS SET BY BOTH THE APPLIPROG AND INTUITION */

/* FLAGS SET BY INTUITION */
#define REQOFFWINDOW	0x1000	// part of one of the Gadgets was off Window
#define REQACTIVE	0x2000
#define SYSREQUEST	0x4000
#define DEFERREFRESH	0x8000	/* this Requester stops a Refresh broadcast */

BOOL CloseWorkBench();
void CurrentTime(ULONG * secs, ULONG * microsecs);
BOOL DisplayAlert(ULONG alertnum, TEXT *t, SHORT num);
BOOL DoubleClick(LONG t1sec, LONG t1mics,
	 LONG t2secs, LONG t2mics);

IntuitionLock LockIBase(ULONG l);

void RemakeDisplay();
void RethinkDisplay();

void UnlockIBase(IntuitionLock l);
View * ViewAddress();

BOOL WBenchToBack();
BOOL WBenchToFront();



/* = =RJ='s peculiarities ================================================= */
#define FOREVER for(;;)
inline SIGN(int x) { return ( ((x) > 0) - ((x) < 0) ); }
#define NOT !

#define ALERT_TYPE	0x80000000
#define RECOVERY_ALERT	0x00000000
#define DEADEND_ALERT 	0x80000000

const AUTOFRONTPEN	= 0;
const AUTOBACKPEN	= 1;
const AUTODRAWMODE	= JAM2;
const AUTOLEFTEDGE	= 6;
const AUTOTOPEDGE	= 3;
const AUTOITEXTFONT	= NULL;
const AUTONEXTTEXT	= NULL;


/* --- RAWMOUSE Codes and Qualifiers (Console OR IDCMP) ------------------- */
const SELECTUP =	(IECODE_LBUTTON | IECODE_UP_PREFIX);
const SELECTDOWN =	(IECODE_LBUTTON);
const MENUUP =		(IECODE_RBUTTON | IECODE_UP_PREFIX);
const MENUDOWN =	(IECODE_RBUTTON);
const ALTLEFT =		(IEQUALIFIER_LALT);
const ALTRIGHT =	(IEQUALIFIER_RALT);
const AMIGALEFT	=	(IEQUALIFIER_LCOMMAND);
const AMIGARIGHT =	(IEQUALIFIER_RCOMMAND);
const AMIGAKEYS	=	(AMIGALEFT | AMIGARIGHT);

const HELPKEY = 	0x5F;
const CURSORUP =	0x4C;
const CURSORLEFT =	0x4F;
const CURSORRIGHT =	0x4E;
const CURSORDOWN =	0x4D;
const KEYCODE_Q	= 0x10;
const KEYCODE_X	= 0x32;
const KEYCODE_N	= 0x36;
const KEYCODE_M	= 0x37;
const KEYCODE_V	= 0x34;
const KEYCODE_B = 0x35;


/* === Remember ========================================================== */

struct Remember
{
    Remember *NextRemember;
    ULONG RememberSize;
    UBYTE *Memory;

	Remember *next() { return NextRemember; }
friend void *AllocRemember(Remember **, ULONG size, ULONG type);
friend void FreeRemember(Remember **, BOOL reallyfree = TRUE);
};

#endif
