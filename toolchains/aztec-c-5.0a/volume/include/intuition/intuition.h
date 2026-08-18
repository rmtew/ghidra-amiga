#ifndef	INTUITION_INTUITION_H
#define	INTUITION_INTUITION_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif
#ifndef	GRAPHICS_GFX_H
#include	"graphics/gfx.h"
#endif
#ifndef	GRAPHICS_CLIP_H
#include	"graphics/clip.h"
#endif
#ifndef	GRAPHICS_VIEW_H
#include	"graphics/view.h"
#endif
#ifndef	GRAPHICS_RASTPORT_H
#include	"graphics/rastport.h"
#endif
#ifndef	GRAPHICS_LAYERS_H
#include	"graphics/layers.h"
#endif
#ifndef	GRAPHICS_TEXT_H
#include	"graphics/text.h"
#endif
#ifndef	EXEC_PORTS_H
#include	"exec/ports.h"
#endif
#ifndef	DEVICES_TIMER_H
#include	"devices/timer.h"
#endif
#ifndef	DEVICES_INPUTEVENT_H
#include	"devices/inputevent.h"
#endif
struct	Menu
{
struct	Menu	*NextMenu;
SHORT	LeftEdge,	TopEdge;
SHORT	Width,	Height;
USHORT	Flags;
BYTE	*MenuName;
struct	MenuItem	*FirstItem;
SHORT	JazzX,	JazzY,	BeatX,	BeatY;
};
#define	MENUENABLED	0x0001
#define	MIDRAWN	0x0100
struct	MenuItem
{
struct	MenuItem	*NextItem;
SHORT	LeftEdge,	TopEdge;
SHORT	Width,	Height;
USHORT	Flags;
LONG	MutualExclude;
APTR	ItemFill;
APTR	SelectFill;
BYTE	Command;
struct	MenuItem	*SubItem;
USHORT	NextSelect;
};
#define	CHECKIT	0x0001
#define	ITEMTEXT	0x0002
#define	COMMSEQ	0x0004
#define	MENUTOGGLE	0x0008
#define	ITEMENABLED	0x0010
#define	HIGHFLAGS	0x00C0
#define	HIGHIMAGE	0x0000
#define	HIGHCOMP	0x0040
#define	HIGHBOX	0x0080
#define	HIGHNONE	0x00C0
#define	CHECKED	0x0100
#define	ISDRAWN	0x1000
#define	HIGHITEM	0x2000
#define	MENUTOGGLED	0x4000
struct	Requester
{
struct	Requester	*OlderRequest;
SHORT	LeftEdge,	TopEdge;
SHORT	Width,	Height;
SHORT	RelLeft,	RelTop;
struct	Gadget	*ReqGadget;
struct	Border	*ReqBorder;
struct	IntuiText	*ReqText;
USHORT	Flags;
UBYTE	BackFill;
struct	Layer	*ReqLayer;
UBYTE	ReqPad1[32];
struct	BitMap	*ImageBMap;
struct	Window	*RWindow;
UBYTE	ReqPad2[36];
};
#define	POINTREL	0x0001
#define	PREDRAWN	0x0002
#define	NOISYREQ	0x0004
#define	REQOFFWINDOW	0x1000
#define	REQACTIVE	0x2000
#define	SYSREQUEST	0x4000
#define	DEFERREFRESH	0x8000
struct	Gadget
{
struct	Gadget	*NextGadget;
SHORT	LeftEdge,	TopEdge;
SHORT	Width,	Height;
USHORT	Flags;
USHORT	Activation;
USHORT	GadgetType;
APTR	GadgetRender;
APTR	SelectRender;
struct	IntuiText	*GadgetText;
LONG	MutualExclude;
APTR	SpecialInfo;
USHORT	GadgetID;
APTR	UserData;
};
#define	GADGHIGHBITS	0x0003
#define	GADGHCOMP	0x0000
#define	GADGHBOX	0x0001
#define	GADGHIMAGE	0x0002
#define	GADGHNONE	0x0003
#define	GADGIMAGE	0x0004
#define	GRELBOTTOM	0x0008
#define	GRELRIGHT	0x0010
#define	GRELWIDTH	0x0020
#define	GRELHEIGHT	0x0040
#define	SELECTED	0x0080
#define	GADGDISABLED	0x0100
#define	RELVERIFY	0x0001
#define	GADGIMMEDIATE	0x0002
#define	ENDGADGET	0x0004
#define	FOLLOWMOUSE	0x0008
#define	RIGHTBORDER	0x0010
#define	LEFTBORDER	0x0020
#define	TOPBORDER	0x0040
#define	BOTTOMBORDER	0x0080
#define	TOGGLESELECT	0x0100
#define	STRINGCENTER	0x0200
#define	STRINGRIGHT	0x0400
#define	LONGINT	0x0800
#define	ALTKEYMAP	0x1000
#define	BOOLEXTEND	0x2000
#define	GADGETTYPE	0xFC00
#define	SYSGADGET	0x8000
#define	SCRGADGET	0x4000
#define	GZZGADGET	0x2000
#define	REQGADGET	0x1000
#define	SIZING	0x0010
#define	WDRAGGING	0x0020
#define	SDRAGGING	0x0030
#define	WUPFRONT	0x0040
#define	SUPFRONT	0x0050
#define	WDOWNBACK	0x0060
#define	SDOWNBACK	0x0070
#define	CLOSE	0x0080
#define	BOOLGADGET	0x0001
#define	GADGET0002	0x0002
#define	PROPGADGET	0x0003
#define	STRGADGET	0x0004
struct	BoolInfo
{
USHORT	Flags;
UWORD	*Mask;
ULONG	Reserved;
};
#define	BOOLMASK	0x0001
struct	PropInfo
{
USHORT	Flags;
USHORT	HorizPot;
USHORT	VertPot;
USHORT	HorizBody;
USHORT	VertBody;
USHORT	CWidth;
USHORT	CHeight;
USHORT	HPotRes,	VPotRes;
USHORT	LeftBorder;
USHORT	TopBorder;
};
#define	AUTOKNOB	0x0001
#define	FREEHORIZ	0x0002
#define	FREEVERT	0x0004
#define	PROPBORDERLESS	0x0008
#define	KNOBHIT	0x0100
#define	KNOBHMIN	6
#define	KNOBVMIN	4
#define	MAXBODY	0xFFFF
#define	MAXPOT	0xFFFF
struct	StringInfo
{
UBYTE	*Buffer;
UBYTE	*UndoBuffer;
SHORT	BufferPos;
SHORT	MaxChars;
SHORT	DispPos;
SHORT	UndoPos;
SHORT	NumChars;
SHORT	DispCount;
SHORT	CLeft,	CTop;
struct	Layer	*LayerPtr;
LONG	LongInt;
struct	KeyMap	*AltKeyMap;
};
struct	IntuiText
{
UBYTE	FrontPen,	BackPen;
UBYTE	DrawMode;
SHORT	LeftEdge;
SHORT	TopEdge;
struct	TextAttr	*ITextFont;
UBYTE	*IText;
struct	IntuiText	*NextText;
};
struct	Border
{
SHORT	LeftEdge,	TopEdge;
UBYTE	FrontPen,	BackPen;
UBYTE	DrawMode;
BYTE	Count;
SHORT	*XY;
struct	Border	*NextBorder;
};
struct	Image
{
SHORT	LeftEdge;
SHORT	TopEdge;
SHORT	Width;
SHORT	Height,	Depth;
USHORT	*ImageData;
UBYTE	PlanePick,	PlaneOnOff;
struct	Image	*NextImage;
};
struct	IntuiMessage
{
struct	Message	ExecMessage;
ULONG	Class;
USHORT	Code;
USHORT	Qualifier;
APTR	IAddress;
SHORT	MouseX,	MouseY;
ULONG	Seconds,	Micros;
struct	Window	*IDCMPWindow;
struct	IntuiMessage	*SpecialLink;
};
#define	SIZEVERIFY	0x00000001
#define	NEWSIZE	0x00000002
#define	REFRESHWINDOW	0x00000004
#define	MOUSEBUTTONS	0x00000008
#define	MOUSEMOVE	0x00000010
#define	GADGETDOWN	0x00000020
#define	GADGETUP	0x00000040
#define	REQSET	0x00000080
#define	MENUPICK	0x00000100
#define	CLOSEWINDOW	0x00000200
#define	RAWKEY	0x00000400
#define	REQVERIFY	0x00000800
#define	REQCLEAR	0x00001000
#define	MENUVERIFY	0x00002000
#define	NEWPREFS	0x00004000
#define	DISKINSERTED	0x00008000
#define	DISKREMOVED	0x00010000
#define	WBENCHMESSAGE	0x00020000
#define	ACTIVEWINDOW	0x00040000
#define	INACTIVEWINDOW	0x00080000
#define	DELTAMOVE	0x00100000
#define	VANILLAKEY	0x00200000
#define	INTUITICKS	0x00400000
#define	LONELYMESSAGE	0x80000000
#define	MENUHOT	0x0001
#define	MENUCANCEL	0x0002
#define	MENUWAITING	0x0003
#define	OKOK	MENUHOT
#define	OKABORT	0x0004
#define	OKCANCEL	MENUCANCEL
#define	WBENCHOPEN	0x0001
#define	WBENCHCLOSE	0x0002
struct	Window
{
struct	Window	*NextWindow;
SHORT	LeftEdge,	TopEdge;
SHORT	Width,	Height;
SHORT	MouseY,	MouseX;
SHORT	MinWidth,	MinHeight;
USHORT	MaxWidth,	MaxHeight;
ULONG	Flags;
struct	Menu	*MenuStrip;
UBYTE	*Title;
struct	Requester	*FirstRequest;
struct	Requester	*DMRequest;
SHORT	ReqCount;
struct	Screen	*WScreen;
struct	RastPort	*RPort;
BYTE	BorderLeft,	BorderTop,	BorderRight,	BorderBottom;
struct	RastPort	*BorderRPort;
struct	Gadget	*FirstGadget;
struct	Window	*Parent,	*Descendant;
USHORT	*Pointer;
BYTE	PtrHeight;
BYTE	PtrWidth;
BYTE	XOffset,	YOffset;
ULONG	IDCMPFlags;
struct	MsgPort	*UserPort,	*WindowPort;
struct	IntuiMessage	*MessageKey;
UBYTE	DetailPen,	BlockPen;
struct	Image	*CheckMark;
UBYTE	*ScreenTitle;
SHORT	GZZMouseX;
SHORT	GZZMouseY;
SHORT	GZZWidth;
SHORT	GZZHeight;
UBYTE	*ExtData;
BYTE	*UserData;
struct	Layer	*WLayer;
struct	TextFont	*IFont;
};
#define	WINDOWSIZING	0x0001
#define	WINDOWDRAG	0x0002
#define	WINDOWDEPTH	0x0004
#define	WINDOWCLOSE	0x0008
#define	SIZEBRIGHT	0x0010
#define	SIZEBBOTTOM	0x0020
#define	REFRESHBITS	0x00C0
#define	SMART_REFRESH	0x0000
#define	SIMPLE_REFRESH	0x0040
#define	SUPER_BITMAP	0x0080
#define	OTHER_REFRESH	0x00C0
#define	BACKDROP	0x0100
#define	REPORTMOUSE	0x0200
#define	GIMMEZEROZERO	0x0400
#define	BORDERLESS	0x0800
#define	ACTIVATE	0x1000
#define	WINDOWACTIVE	0x2000
#define	INREQUEST	0x4000
#define	MENUSTATE	0x8000
#define	RMBTRAP	0x00010000
#define	NOCAREREFRESH	0x00020000
#define	WINDOWREFRESH	0x01000000
#define	WBENCHWINDOW	0x02000000
#define	WINDOWTICKED	0x04000000
#define	SUPER_UNUSED	0xFCFC0000
struct	NewWindow
{
SHORT	LeftEdge,	TopEdge;
SHORT	Width,	Height;
UBYTE	DetailPen,	BlockPen;
ULONG	IDCMPFlags;
ULONG	Flags;
struct	Gadget	*FirstGadget;
struct	Image	*CheckMark;
UBYTE	*Title;
struct	Screen	*Screen;
struct	BitMap	*BitMap;
SHORT	MinWidth,	MinHeight;
USHORT	MaxWidth,	MaxHeight;
USHORT	Type;
};
#ifndef	INTUITION_SCREENS_H
#include	"intuition/screens.h"
#endif
#ifndef	INTUITION_PREFERENCES_H
#include	"intuition/preferences.h"
#endif
struct	Remember
{
struct	Remember	*NextRemember;
ULONG	RememberSize;
UBYTE	*Memory;
};
#define	MENUNUM(n)	(n	&	0x1F)
#define	ITEMNUM(n)	((n	>>	5)	&	0x003F)
#define	SUBNUM(n)	((n	>>	11)	&	0x001F)
#define	SHIFTMENU(n)	(n	&	0x1F)
#define	SHIFTITEM(n)	((n	&	0x3F)	<<	5)
#define	SHIFTSUB(n)	((n	&	0x1F)	<<	11)
#define	SRBNUM(n)	(0x08	-	(n	>>	4))
#define	SWBNUM(n)	(0x08	-	(n	&	0x0F))
#define	SSBNUM(n)	(0x01	+	(n	>>	4))
#define	SPARNUM(n)	(n	>>	4)
#define	SHAKNUM(n)	(n	&	0x0F)
#define	NOMENU	0x001F
#define	NOITEM	0x003F
#define	NOSUB	0x001F
#define	MENUNULL	0xFFFF
#define	FOREVER	for(;;)
#define	SIGN(x)	(	((x)	>	0)	-	((x)	<	0)	)
#define	NOT	!
#define	CHECKWIDTH	19
#define	COMMWIDTH	27
#define	LOWCHECKWIDTH	13
#define	LOWCOMMWIDTH	16
#define	ALERT_TYPE	0x80000000
#define	RECOVERY_ALERT	0x00000000
#define	DEADEND_ALERT	0x80000000
#define	AUTOFRONTPEN	0
#define	AUTOBACKPEN	1
#define	AUTODRAWMODE	JAM2
#define	AUTOLEFTEDGE	6
#define	AUTOTOPEDGE	3
#define	AUTOITEXTFONT	NULL
#define	AUTONEXTTEXT	NULL
#define	SELECTUP	(IECODE_LBUTTON	|	IECODE_UP_PREFIX)
#define	SELECTDOWN	(IECODE_LBUTTON)
#define	MENUUP	(IECODE_RBUTTON	|	IECODE_UP_PREFIX)
#define	MENUDOWN	(IECODE_RBUTTON)
#define	ALTLEFT	(IEQUALIFIER_LALT)
#define	ALTRIGHT	(IEQUALIFIER_RALT)
#define	AMIGALEFT	(IEQUALIFIER_LCOMMAND)
#define	AMIGARIGHT	(IEQUALIFIER_RCOMMAND)
#define	AMIGAKEYS	(AMIGALEFT	|	AMIGARIGHT)
#define	CURSORUP	0x4C
#define	CURSORLEFT	0x4F
#define	CURSORRIGHT	0x4E
#define	CURSORDOWN	0x4D
#define	KEYCODE_Q	0x10
#define	KEYCODE_X	0x32
#define	KEYCODE_N	0x36
#define	KEYCODE_M	0x37
#define	KEYCODE_V	0x34
#define	KEYCODE_B	0x35
#endif
