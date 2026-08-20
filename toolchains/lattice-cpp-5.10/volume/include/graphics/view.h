#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H
#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif

struct cprlist;
struct CopList;
struct UCopList;

struct RastPort;
struct RasInfo;

struct ViewPort;

struct ColorMap
{
   UBYTE	Flags;
   UBYTE	Type;
   UWORD	Count;
   APTR		ColorTable;
   friend void LoadRGB4(ViewPort *v, short *m, const int n);
};

struct ViewPort;

struct View
{
   ViewPort *ViewPort;
   cprlist *LOFCprList;
   cprlist *SHFCprList;
   short DyOffset,DxOffset;
   UWORD   Modes;

friend void InitView(const View *v);
View(UWORD mode) { InitView(this); Modes = mode; }
View() {}	// for Intuition's benefit
};

#define PFBA            0x40
#define DUALPF          0x400
#define HIRES           0x8000
#define LORES		0
#define LACE            4
#define HAM             0x800
#define SPRITES         0x4000
#define VP_HIDE         0x2000
#define GENLOCK_AUDIO   0x100
#define GENLOCK_VIDEO   2
#define EXTRA_HALFBRITE 0x80

struct ViewPort
{
   ViewPort	*Next;
   ColorMap	*ColorMap;
   CopList	*DspIns;
   CopList	*SprIns;
   CopList	*ClrIns;
   UCopList	*UCopIns;
   SHORT	DWidth,DHeight;
   SHORT	DxOffset,DyOffset;
   UWORD	Modes;
   UBYTE        SpritePriorities;
   UBYTE        reserved;
   RasInfo	*RasInfo;

friend void InitVPort(const ViewPort *vp);
   ViewPort(View *v) { InitVPort(this); Modes = v->Modes & ~HAM; }
   ViewPort() {}
   ViewPort *next() { return Next; }
   friend void ScrollVPort(const ViewPort *vp, const int x, const int y);
   void scroll (short x, short y) {ScrollVPort(this, x, y); }
};

struct RasInfo
{
   RasInfo *Next;
   BitMap *BitMap;
   SHORT    RxOffset,RyOffset;

   RasInfo *next() { return Next; }
};


void LoadView(const View *v);
void MakeVPort(const View *v,  ViewPort *vp);
void MrgCop(const View * v);

void SetRGB4(const ViewPort *vp, const SHORT a,
	const UBYTE b, const UBYTE c, const UBYTE d);
void SetRGB4CM(const ColorMap *cm, const SHORT a,
	const UBYTE b, const UBYTE c, const UBYTE d);
void WaitBOVP(const ViewPort *vp);
void DrawGList( const RastPort *r, const ViewPort *vp);

#endif
