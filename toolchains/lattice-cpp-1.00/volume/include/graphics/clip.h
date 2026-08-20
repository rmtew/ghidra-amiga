// graphics/clip.h
//
// (C) 1977, MTS Associates
//
#ifndef GRAPHICS_CLIP_H
#define GRAPHICS_CLIP_H
#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif
#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif
#ifndef GRAPHICS_RASTPORT_H
#include <graphics/rastport.h>
#endif
#ifndef GRAPHICS_REGIONS_H
#include <graphics/regions.h>
#endif

#define NEWLOCKS

struct Layer;
struct Layer_Info;
struct BitMap;

struct ClipRect
{
    ClipRect *Next;
    ClipRect *Prev;
    Layer   *lobs;
    BitMap  *BitMap;
    Rectangle   bounds;
    ClipRect *_p1,*_p2;
    LONG    reserved;
#ifdef NEWCLIPRECTS_1_1
    LONG    Flags;
#endif
	ClipRect *next() { return Next;}
	ClipRect *prev() { return Prev;}
};

struct Layer
{
    Layer	*front,*back;
    struct ClipRect	*ClipRect;
    RastPort	*rp;
    Rectangle	bounds;
    UBYTE	reserved[4];
    UWORD	priority;
    UWORD	Flags;
    struct BitMap	*SuperBitMap;
    struct ClipRect	*SuperClipRect;
    APTR	Window;
    SHORT	Scroll_X,Scroll_Y;
    struct ClipRect	*cr,*cr2,*crnew;
    struct ClipRect	*SuperSaveClipRects;
    struct ClipRect	*_cliprects;
    Layer_Info  *LayerInfo;
    SignalSemaphore Lock;
    UBYTE	reserved3[8];
    Region	*ClipRegion;
    Region	*saveClipRects;
    UBYTE	reserved2[22];
    Region	*DamageList;
};

#define CR_NEEDS_NO_CONCEALED_RASTERS  1
#define ISLESSX 1
#define ISLESSY 2
#define ISGRTRX 4
#define ISGRTRY 8

#endif
