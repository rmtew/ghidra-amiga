#ifndef	INTUITION_INTUITIONBASE_H
#define	INTUITION_INTUITIONBASE_H
#ifndef	EXEC_TYPES_H
#include	<exec/types.h>
#endif
#ifndef	EXEC_LIBRARIES_H
#include	<exec/libraries.h>
#endif
#ifndef	INTUITION_INTUITION_H
#include	<intuition/intuition.h>
#endif
#ifndef	EXEC_INTERRUPTS_H
#include	<exec/interrupts.h>
#endif
#ifdef	INTUITIONPRIVATE
#define	DMODECOUNT	0x0002
#define	HIRESPICK	0x0000
#define	LOWRESPICK	0x0001
#define	EVENTMAX	10
#define	RESCOUNT	2
#define	HIRESGADGET	0
#define	LOWRESGADGET	1
#define	GADGETCOUNT	8
#define	UPFRONTGADGET	0
#define	DOWNBACKGADGET	1
#define	SIZEGADGET	2
#define	CLOSEGADGET	3
#define	DRAGGADGET	4
#define	SUPFRONTGADGET	5
#define	SDOWNBACKGADGET	6
#define	SDRAGGADGET	7
#define	ISTATELOCK	0
#define	LAYERINFOLOCK	1
#define	GADGETSLOCK	2
#define	LAYERROMLOCK	3
#define	VIEWLOCK	4
#define	IBASELOCK	5
#define	RPLOCK	6
#define	NUMILOCKS	7
struct	FatIntuiMessage	{
struct	IntuiMessage;
ULONG	PrevKeys;
};
struct	IBox	{
SHORT	Left;
SHORT	Top;
SHORT	Width;
SHORT	Height;
};
struct	Point	{
SHORT	X;
SHORT	Y;
};
struct	PenPair	{
UBYTE	DetailPen;
UBYTE	BlockPen;
};
struct	GListEnv	{
struct	Screen	*ge_Screen;
struct	Window	*ge_Window;
struct	Requester	*ge_Requester;
struct	RastPort	*ge_RastPort;
struct	Layer	*ge_Layer;
struct	Layer	*ge_GZZLayer;
struct	PenPair	ge_Pens;
struct	IBox	ge_Domain;
struct	IBox	ge_GZZdims;
};
struct	GadgetInfo	{
struct	GListEnv	*gi_Environ;
struct	Gadget	*gi_Gadget;
struct	IBox	gi_Box;
struct	IBox	gi_Container;
struct	Layer	*gi_Layer;
struct	IBox	gi_NewKnob;
};
#endif
struct	IntuitionBase
{
struct	Library	LibNode;
struct	View	ViewLord;
struct	Window	*ActiveWindow;
struct	Screen	*ActiveScreen;
struct	Screen	*FirstScreen;
ULONG	Flags;
WORD	MouseY,	MouseX;
ULONG	Seconds;
ULONG	Micros;
#ifdef	INTUITIONPRIVATE
WORD	MinXMouse,	MaxXMouse;
WORD	MinYMouse,	MaxYMouse;
ULONG	StartSecs,	StartMicros;
APTR	SysBase;
struct	GfxBase	*GfxBase;
APTR	LayersBase;
APTR	ConsoleDevice;
USHORT	*APointer;
BYTE	APtrHeight;
BYTE	APtrWidth;
BYTE	AXOffset,	AYOffset;
USHORT	MenuDrawn;
USHORT	MenuSelected;
USHORT	OptionList;
struct	RastPort	MenuRPort;
struct	TmpRas	MenuTmpRas;
struct	ClipRect	ItemCRect;
struct	ClipRect	SubCRect;
struct	BitMap	IBitMap;
struct	BitMap	SBitMap;
struct	IOStdReq	InputRequest;
struct	Interrupt	InputInterrupt;
struct	Remember	*EventKey;
struct	InputEvent	*IEvents;
#define	NUM_IEVENTS	4
SHORT	EventCount;
struct	InputEvent	IEBuffer[NUM_IEVENTS];
struct	Gadget	*ActiveGadget;
struct	PropInfo	*ActivePInfo;
struct	Image	*ActiveImage;
struct	GListEnv	GadgetEnv;
struct	GadgetInfo	GadgetInfo;
struct	Point	KnobOffset;
struct	Window	*getOKWindow;
struct	IntuiMessage	*getOKMessage;
USHORT	setWExcept,	GadgetReturn,	StateReturn;
struct	RastPort	*RP;
struct	TmpRas	ITmpRas;
struct	Region	*OldClipRegion;
struct	Point	OldScroll;
struct	IBox	IFrame;
SHORT	hthick,	vthick;
VOID	(*frameChange)();
VOID	(*sizeDrag)();
struct	Point	FirstPt;
struct	Point	OldPt;
struct	Gadget	*SysGadgets[RESCOUNT][GADGETCOUNT];
struct	Image	*CheckImage[RESCOUNT],	*AmigaIcon[RESCOUNT];
#ifdef	OLDPATTERN
USHORT	apattern[3],	bpattern[4];
#else
USHORT	apattern[8],	bpattern[4];
#endif
USHORT	*IPointer;
BYTE	IPtrHeight;
BYTE	IPtrWidth;
BYTE	IXOffset,	IYOffset;
LONG	DoubleSeconds,	DoubleMicros;
BYTE	WBorLeft[DMODECOUNT];
BYTE	WBorTop[DMODECOUNT];
BYTE	WBorRight[DMODECOUNT];
BYTE	WBorBottom[DMODECOUNT];
BYTE	BarVBorder[DMODECOUNT];
BYTE	BarHBorder[DMODECOUNT];
BYTE	MenuVBorder[DMODECOUNT];
BYTE	MenuHBorder[DMODECOUNT];
USHORT	color0;
USHORT	color1;
USHORT	color2;
USHORT	color3;
USHORT	color17;
USHORT	color18;
USHORT	color19;
struct	TextAttr	SysFont;
struct	Preferences	*Preferences;
struct	DistantEcho	*Echoes;
WORD	ViewInitX,	ViewInitY;
SHORT	CursorDX,	CursorDY;
struct	KeyMap	*KeyMap;
SHORT	MouseYMinimum;
SHORT	ErrorX,	ErrorY;
struct	timerequest	IOExcess;
SHORT	HoldMinYMouse;
struct	MsgPort	*WBPort,	*iqd_FNKUHDPort;
struct	IntuiMessage	WBMessage;
struct	Screen	*HitScreen;
struct	SimpleSprite	*SimpleSprite;
struct	SimpleSprite	*AttachedSSprite;
BOOL	GotSprite1;
struct	List	SemaphoreList;
struct	SignalSemaphore	ISemaphore[NUMILOCKS];
WORD	MaxDisplayHeight;
WORD	MaxDisplayRow;
WORD	MaxDisplayWidth;
ULONG	Reserved[7];
#endif
};
#endif
