// Intuition/Intuitionbase.h
//
// (C) 1987, MTS Associates
//
// ****** CAUTION *****
//
// Directly modifying data in these areas is STRONGLY discouraged.
// These data areas and their meanings are subject to sudden and drastic
// changes by Commodore-Amiga.  If you play with them frivolously to
// show how clever you are, you may prove just the opposite when
// all your programs start breaking. 
//
//
#ifndef INTUITION_INTUITIONBASE_H
#define INTUITION_INTUITIONBASE_H 1
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
#define DMODECOUNT      0x0002
#define HIRESPICK       0x0000
#define LOWRESPICK      0x0001
#define EVENTMAX 10
#define RESCOUNT        2
#define HIRESGADGET     0
#define LOWRESGADGET    1
#define GADGETCOUNT     8
#define UPFRONTGADGET   0
#define DOWNBACKGADGET  1
#define SIZEGADGET      2
#define CLOSEGADGET     3
#define DRAGGADGET      4
#define SUPFRONTGADGET  5
#define SDOWNBACKGADGET 6
#define SDRAGGADGET     7
#define ISTATELOCK      0
#define LAYERINFOLOCK   1
#define GADGETSLOCK     2
#define LAYERROMLOCK    3
#define VIEWLOCK        4
#define IBASELOCK       5
#define RPLOCK          6
#define NUMILOCKS       7

struct FatIntuiMessage  : IntuiMessage	{
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

struct GListEnv  {
     Screen       *ge_Screen;
     Window       *ge_Window;
     Requester    *ge_Requester;
     RastPort     *ge_RastPort;
     Layer        *ge_Layer;
     Layer        *ge_GZZLayer;
     PenPair       ge_Pens;
     IBox          ge_Domain;
     IBox          ge_GZZdims;
};

struct GadgetInfo  {
     GListEnv     *gi_Environ;
     Gadget       *gi_Gadget;
     IBox         gi_Box;
     IBox         gi_Container;
     Layer        *gi_Layer;
     IBox         gi_NewKnob;
};
#endif

struct IntuitionBase : public Library
{
    View ViewLord;
    Window *ActiveWindow;
    Screen *ActiveScreen;
    Screen *FirstScreen;
    ULONG Flags;
    WORD MouseY, MouseX;
    ULONG Seconds;
    ULONG Micros;
#ifdef INTUITIONPRIVATE
    WORD MinXMouse, MaxXMouse;
    WORD MinYMouse, MaxYMouse;
    ULONG StartSecs, StartMicros;
    APTR        SysBase;
     GfxBase *GfxBase;
    APTR        LayersBase;
    APTR        ConsoleDevice;
    USHORT *APointer;
    BYTE APtrHeight;
    BYTE APtrWidth;
    BYTE AXOffset, AYOffset;
    USHORT MenuDrawn;
    USHORT MenuSelected;
    USHORT OptionList;
     RastPort MenuRPort;
     TmpRas       MenuTmpRas;
     ClipRect ItemCRect;
     ClipRect SubCRect;
     BitMap IBitMap;
     BitMap SBitMap;
     IOStdReq InputRequest;
     Interrupt InputInterrupt;
     Remember  *EventKey;
     InputEvent *IEvents;
#define NUM_IEVENTS 4
    SHORT  EventCount;
     InputEvent IEBuffer[NUM_IEVENTS];
     Gadget *ActiveGadget;
     PropInfo *ActivePInfo;
     Image *ActiveImage;
     GListEnv GadgetEnv;
     GadgetInfo GadgetInfo;
     Point KnobOffset;
     Window *getOKWindow;
     IntuiMessage *getOKMessage;
    USHORT setWExcept, GadgetReturn, StateReturn;
     RastPort     *RP;
     TmpRas       ITmpRas;
     Region *OldClipRegion;
     Point    OldScroll;
     IBox IFrame;
    SHORT hthick, vthick;
    VOID   (*frameChange)();
    VOID   (*sizeDrag)();
     Point FirstPt;
     Point OldPt;
     Gadget *SysGadgets[RESCOUNT][GADGETCOUNT];
     Image *CheckImage[RESCOUNT], *AmigaIcon[RESCOUNT];
#ifdef OLDPATTERN
    USHORT apattern[3], bpattern[4];
#else
    USHORT apattern[8], bpattern[4];
#endif
    USHORT *IPointer;
    BYTE IPtrHeight;
    BYTE IPtrWidth;
    BYTE IXOffset, IYOffset;
    LONG DoubleSeconds, DoubleMicros;
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
    TextAttr SysFont;
    Preferences *Preferences;
    DistantEcho *Echoes;
    WORD ViewInitX, ViewInitY;
    SHORT CursorDX, CursorDY;
    KeyMap *KeyMap;
    SHORT MouseYMinimum;
    SHORT ErrorX, ErrorY;
    timerequest IOExcess;
    SHORT HoldMinYMouse;
    MsgPort *WBPort, *iqd_FNKUHDPort;
    IntuiMessage WBMessage;
    Screen *HitScreen;
    SimpleSprite *SimpleSprite;
    SimpleSprite *AttachedSSprite;
    BOOL        GotSprite1;
    List                 SemaphoreList;
    SignalSemaphore      ISemaphore[NUMILOCKS];
    WORD        MaxDisplayHeight;
    WORD        MaxDisplayRow;
    WORD        MaxDisplayWidth;
    ULONG       Reserved[7];
#endif
};
#endif
