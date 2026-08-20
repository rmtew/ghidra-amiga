#ifndef	INTUITION_SCREENS_H
#define	INTUITION_SCREENS_H
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
struct	Screen
{
struct	Screen	*NextScreen;
struct	Window	*FirstWindow;
SHORT	LeftEdge,	TopEdge;
SHORT	Width,	Height;
SHORT	MouseY,	MouseX;
USHORT	Flags;
UBYTE	*Title;
UBYTE	*DefaultTitle;
BYTE	BarHeight,	BarVBorder,	BarHBorder,	MenuVBorder,	MenuHBorder;
BYTE	WBorTop,	WBorLeft,	WBorRight,	WBorBottom;
struct	TextAttr	*Font;
struct	ViewPort	ViewPort;
struct	RastPort	RastPort;
struct	BitMap	BitMap;
struct	Layer_Info	LayerInfo;
struct	Gadget	*FirstGadget;
UBYTE	DetailPen,	BlockPen;
USHORT	SaveColor0;
struct	Layer	*BarLayer;
UBYTE	*ExtData;
UBYTE	*UserData;
};
#define	SCREENTYPE	0x000F
#define	WBENCHSCREEN	0x0001
#define	CUSTOMSCREEN	0x000F
#define	SHOWTITLE	0x0010
#define	BEEPING	0x0020
#define	CUSTOMBITMAP	0x0040
#define	SCREENBEHIND	0x0080
#define	SCREENQUIET	0x0100
#define	STDSCREENHEIGHT	-1
struct	NewScreen
{
SHORT	LeftEdge,	TopEdge,	Width,	Height,	Depth;
UBYTE	DetailPen,	BlockPen;
USHORT	ViewModes;
USHORT	Type;
struct	TextAttr	*Font;
UBYTE	*DefaultTitle;
struct	Gadget	*Gadgets;
struct	BitMap	*CustomBitMap;
};
#endif
