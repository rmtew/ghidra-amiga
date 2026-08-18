#ifndef INTUITION_INTUITIONBASE_H
#define INTUITION_INTUITIONBASE_H
/*
**	$Filename: intuition/intuitionbase.h $
**	$Release: 1.3 $
**
**	the IntuitionBase structure and supporting structures 
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif

#ifndef EXEC_INTERRUPTS_H
#include <exec/interrupts.h>
#endif

#ifdef INTUITIONPRIVATE
/* these types and constants are used in the forbidden part of IntuitionBase.
 * see below for an explanation that these are NOT supported for your use.
 * They will certainly change in subsequent releases, and are provided
 * for education, debugging, and information.
 */

/* these are the display modes for which we have corresponding parameter
 *  settings in the config arrays
 */
#define DMODECOUNT	0x0002	/* how many modes there are */
#define HIRESPICK	0x0000
#define LOWRESPICK	0x0001

#define EVENTMAX 10		/* size of event array */

/* these are the system Gadget defines */
#define RESCOUNT	2
#define HIRESGADGET	0
#define LOWRESGADGET	1

#define GADGETCOUNT	8
#define UPFRONTGADGET	0
#define DOWNBACKGADGET	1
#define SIZEGADGET	2
#define CLOSEGADGET	3
#define DRAGGADGET	4
#define SUPFRONTGADGET	5
#define SDOWNBACKGADGET 6
#define SDRAGGADGET	7

/* jimm: 1/10/86: Intuition Locking */
/* Let me say it again: don't even think about using this information
 * in a program.
 */
#define ISTATELOCK	0	/* Intuition() not re-entrant		*/
#define LAYERINFOLOCK	1	/* dummy lock used to check protocol	*/
#define GADGETSLOCK	2	/* gadget lists, refresh, flags		*/
#define LAYERROMLOCK	3	/* (dummy) for lock layerrom		*/
#define VIEWLOCK	4	/* access to ViewLord			*/
#define IBASELOCK	5	/* protexts IBase pointers and lists	*/
#define RPLOCK		6	/* use of IBase->RP			*/
#define NUMILOCKS	7

/* ======================================================================== */
/* === Intuition Geometric Primitives ===================================== */
/* ======================================================================== */

struct FatIntuiMessage {
    struct IntuiMessage;
    ULONG  PrevKeys;
    };

struct IBox {
    SHORT Left;
    SHORT Top;
    SHORT Width;
    SHORT Height;
    };

struct Point {
    SHORT X;
    SHORT Y;
    };

struct PenPair {
    UBYTE DetailPen;
    UBYTE BlockPen;
    };

/* ======================================================================== */
/* === Gadget Environments ================================================ */
/* ======================================================================== */

/* environment for a whole list of gadgets. note that information for both
 * layers of a G00 window are included.
 */
struct GListEnv	 {
    struct Screen	*ge_Screen;
    struct Window	*ge_Window;
    struct Requester	*ge_Requester;
    struct RastPort	*ge_RastPort;	/* rastport used to render gadget   */
    struct Layer	*ge_Layer;	/* layer for gadget (border, if G00)*/
    struct Layer	*ge_GZZLayer;	/* interior layer for G00 windows   */
    struct PenPair	 ge_Pens;	/* pens for rendering gadget	    */
    struct IBox		 ge_Domain;
			/* window, screen, requester, rel. to window/screen */
    struct IBox		 ge_GZZdims;	/* interior window area		    */
    };

/* information for a gadget in its environment. includes relatively
 * correct size, container for propgadgets, correct layer for this gadget,
 * and back pointers to the environment and gadget itself
 */
struct GadgetInfo  {
    struct GListEnv	*gi_Environ;	/* environment for this gadget	    */
    struct Gadget	*gi_Gadget;	/* gadget this info is for	    */
    struct IBox		gi_Box;		/* actual dimensions of gadget	    */
    struct IBox		gi_Container;	/* inner container dimensions	    */
    struct Layer	*gi_Layer;	/* correct layer for this gadget    */
    struct IBox		gi_NewKnob;	/* place to draw new slider knob    */
    };
#endif	/* PRIVATE VALUES */

/* ======================================================================== */
/* === IntuitionBase ====================================================== */
/* ======================================================================== */
/*
 * Be sure to protect yourself against someone modifying these data as
 * you look at them.  This is done by calling:
 *
 * lock = LockIBase(0), which returns a ULONG.	When done call
 * UnlockIBase(lock) where lock is what LockIBase() returned.
 */

struct IntuitionBase
/* IntuitionBase should never be directly modified by programs	*/
/* even a little bit, guys/gals; do you hear me?	*/
{
    struct Library LibNode;

    struct View ViewLord;

    struct Window *ActiveWindow;
    struct Screen *ActiveScreen;

    /* the FirstScreen variable points to the frontmost Screen.	 Screens are
     * then maintained in a front to back order using Screen.NextScreen
     */
    struct Screen *FirstScreen; /* for linked list of all screens */

    ULONG Flags;	/* see definitions below */
    WORD MouseY, MouseX;	/* mouse position relative to View */

    ULONG Seconds;	/* timestamp of most current input event */
    ULONG Micros;	/* timestamp of most current input event */

#ifdef INTUITIONPRIVATE

    /* The following is a snapshot of the "private" part of 
     * Intuition's library data.  It is included for educational
     * use and your debugging only.  It is absolutely guaranteed
     * that this structure will change from release to release.
     *
     * So: don't count on any of the values you find here
     *	   don't even think about changing any of these fields
     *	     (that goes for the "supported" fields above, too).
     *
     * Some work has been done to find the include files
     * that these fields depend on.
     *
     *			jimm: 9/10/86.
     */

    WORD MinXMouse, MaxXMouse;	/* bounded X position for the mouse */
    WORD MinYMouse, MaxYMouse;	/* bounded Y position for the mouse */

    ULONG StartSecs, StartMicros;	/* measure double clicks */

    /* --------------- base vectors ----------------------------------- */
    /* DO MOVE THESE OFFSETS WITHOUT ADJUSTING EQUATES IN IWORK.ASM 
     * this is automatically handled by standalone program offsets.c
     */
    APTR	SysBase;
    struct GfxBase *GfxBase;
    APTR	LayersBase;
    APTR	ConsoleDevice;

    /* --------------- Sprite Pointer --------------------------------- */
    USHORT *APointer;	/* the ActiveGroup pointer sprite definition	*/
    BYTE APtrHeight;	/* height of the pointer */
    BYTE APtrWidth;	/* width in pixels of the pointer (<= 16!)	*/
    BYTE AXOffset, AYOffset; /* sprite offsets */


    /* ---------------- Menu Rendering and Operation ------------------ */
    USHORT MenuDrawn;	/* menu/item/sub number of current display */
    USHORT MenuSelected;/* menu/item/sub number of selected (and highlights)*/
    USHORT OptionList;			/* menu selection	*/

    /* this is the work RastPort used for building item and subitem displays
     *	you can never count on it being stable, other than that it is
     *	mostly a copy of the active screen's RastPort.
     */
    struct RastPort MenuRPort;
    struct TmpRas	MenuTmpRas;
    struct ClipRect ItemCRect; /* for the item's screen display */
    struct ClipRect SubCRect; /* for the subitem's screen display */
    struct BitMap IBitMap; /* for the item's planes */
    struct BitMap SBitMap; /* for the subitem's planes */
			
    /* ---------------- Input Device Interface ------------------------ */
    struct IOStdReq InputRequest;
    struct Interrupt InputInterrupt;

    /* for dynamically allocated input events */
    struct Remember  *EventKey;
    struct InputEvent *IEvents;

    /* for statically "allocated" input events */
#define NUM_IEVENTS 4
    SHORT  EventCount;
    struct InputEvent IEBuffer[NUM_IEVENTS];

    /* ---------------- Active Gadget Information --------------------- */
    struct Gadget *ActiveGadget;
    struct PropInfo *ActivePInfo;
    struct Image *ActiveImage;
    struct GListEnv GadgetEnv;	/* environment of the active gadget	*/
    struct GadgetInfo GadgetInfo;/* specific information for active gadget*/
    struct Point KnobOffset;	/* position in knob of mouse when selected*/

    /* ---------------- Verify Functions Support ---------------------- */
    /* hold information about getOK wait(), used for breakVerify() */
    struct Window *getOKWindow;
    struct IntuiMessage *getOKMessage;

    /* ---------------- State Machine --------------------------------- */
    USHORT setWExcept, GadgetReturn, StateReturn;

    /* ----------- Intuition's Rendering for Gadgets, Titles, ... ----- */
    /* This will be allocated on init */
    struct RastPort	*RP;
    struct TmpRas	ITmpRas;
    struct Region *OldClipRegion;	/* locks with RPort */
    struct Point    OldScroll;		/* user's Scroll_X/Y*/

    /* ----------- Frame Rendering for Window Size/Drag --------------- */
    struct IBox IFrame; /* window frame for sizing/dragging	*/
    SHORT hthick, vthick;	/* IFrame thickness		*/
    VOID   (*frameChange)();	/* function to change IFrame	*/
    VOID   (*sizeDrag)();	/* either ISizeWindow or IMoveWindow */
    struct Point FirstPt;	/* point from which s/d started */
    struct Point OldPt;		/* previous point for s/d	*/

    /* ---------------- System Gadget Templates ------------------------ */
    struct Gadget *SysGadgets[RESCOUNT][GADGETCOUNT];
    struct Image *CheckImage[RESCOUNT], *AmigaIcon[RESCOUNT];

    /* ---------------- Window Drag Rendering ------------------------- */
#ifdef OLDPATTERN
    USHORT apattern[3], bpattern[4];
#else
    USHORT apattern[8], bpattern[4];
#endif

    /* --- Preferences Section ---------------------------------------------- */
    USHORT *IPointer;	/* the INTUITION pointer default sprite definition */
    BYTE IPtrHeight;	/* height of the pointer */
    BYTE IPtrWidth;	/* width in pixels of the pointer (<= 16!) */
    BYTE IXOffset, IYOffset; /* sprite offsets */

    LONG DoubleSeconds, DoubleMicros; /* for testing double-click timeout */

    /* ---------------- Border Widths --------------------------------- */
    BYTE WBorLeft[DMODECOUNT];
    BYTE WBorTop[DMODECOUNT];
    BYTE WBorRight[DMODECOUNT];
    BYTE WBorBottom[DMODECOUNT];

    BYTE BarVBorder[DMODECOUNT];
    BYTE BarHBorder[DMODECOUNT];
    BYTE MenuVBorder[DMODECOUNT];
    BYTE MenuHBorder[DMODECOUNT];

    USHORT color0;
    USHORT color1;
    USHORT color2;
    USHORT color3;
    USHORT color17;
    USHORT color18;
    USHORT color19;

    struct TextAttr SysFont;

    /* WARNING:	 you can easily wipe out Intuition by modifying this pointer 
     * or the Preference data pointed to by this!
     */
    struct Preferences *Preferences;

    /* ----------------- Deferred action queue ---------------------- */
    struct DistantEcho *Echoes;

    WORD ViewInitX, ViewInitY;	/* View initial offsets at startup   */

    SHORT CursorDX, CursorDY;	/* for accelerating pointer movement */

    struct KeyMap *KeyMap;	/* for the active String Gadget */
    
    SHORT MouseYMinimum;	/* magic	*/

    SHORT ErrorX, ErrorY;	/* for retaining mouse movement round-off */
    
    struct timerequest IOExcess;

    SHORT HoldMinYMouse;

    struct MsgPort *WBPort, *iqd_FNKUHDPort;
    struct IntuiMessage WBMessage;
    struct Screen *HitScreen;	/* set by hitUpfront() routine */

    /** jimm:dale: 11/25/85, thought we'd take a chance for glory **/
    struct SimpleSprite *SimpleSprite;
    struct SimpleSprite *AttachedSSprite;
    BOOL	GotSprite1;

    /** jimm: 1/6/86: Intuition contention **/
    struct List			SemaphoreList;	/* chain of the below */
    struct SignalSemaphore	ISemaphore[NUMILOCKS];

    WORD	MaxDisplayHeight;	/* in interlaced mode: 400 or 512 */
    WORD	MaxDisplayRow;		/* MaxDisplayHeight - 1		  */
    WORD	MaxDisplayWidth;	/* copy of GfxBase's NormalDisplayCol */

    ULONG	Reserved[7];	/* cause one never know, do one? */
#endif	/* PRIVATE VALUES */
};

#endif	/* INTUITION_INTUITIONBASE_H */
