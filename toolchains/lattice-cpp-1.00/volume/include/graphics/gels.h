//
// (C) 1986, MTS Associates
//
#ifndef GRAPHICS_GELS_H
#define GRAPHICS_GELS_H

#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif

#ifndef GRAPHICS_VIEW_H
#include <graphics/view.h>
#endif

#ifndef GRAPHICS_RASTPORT_H
#include <graphics/rastport.h>
#endif

#define SUSERFLAGS  0x00FF
#define VSPRITE     0x0001
#define SAVEBACK    0x0002
#define OVERLAY     0x0004
#define MUSTDRAW    0x0008
#define BACKSAVED   0x0100
#define BOBUPDATE   0x0200
#define GELGONE     0x0400
#define VSOVERFLOW  0x0800
#define BUSERFLAGS  0x00FF
#define SAVEBOB     0x0001
#define BOBISCOMP   0x0002
#define BWAITING    0x0100
#define BDRAWN      0x0200
#define BOBSAWAY    0x0400
#define BOBNIX      0x0800
#define SAVEPRESERVE 0x1000
#define OUTSTEP     0x2000
#define ANFRACSIZE  6
#define ANIMHALF    0x0020
#define RINGTRIGGER 0x0001
#ifndef VUserStuff
#define VUserStuff SHORT
#endif
#ifndef BUserStuff
#define BUserStuff SHORT
#endif
#ifndef AUserStuff
#define AUserStuff SHORT
#endif

struct Bob;
struct AnimOb;
struct DBufPacket;

struct VSprite
{
    VSprite   *NextVSprite;
    VSprite   *PrevVSprite;
    VSprite   *DrawPath;
    VSprite   *ClearPath;
    WORD OldY, OldX;
    WORD Flags;
    WORD Y, X;
    WORD Height;
    WORD Width;
    WORD Depth;
    WORD MeMask;
    WORD HitMask;
    WORD *ImageData;
    WORD *BorderLine;
    WORD *CollMask;
    WORD *SprColors;
    Bob  *VSBob;
    BYTE PlanePick;
    BYTE PlaneOnOff;
    VUserStuff VUserExt;

    VSprite *next() { return NextVSprite; }
    VSprite *prev() { return PrevVSprite; }
};

struct AnimComp
{
    WORD Flags;
    WORD Timer;
    WORD TimeSet;
    AnimComp  *NextComp;
    AnimComp  *PrevComp;
    AnimComp  *NextSeq;
    AnimComp  *PrevSeq;
    WORD (*AnimCRoutine)();
    WORD YTrans;
    WORD XTrans;
    AnimOb    *HeadOb;
    Bob       *AnimBob;

    AnimComp *next() { return NextComp; }
    AnimComp *prev() { return PrevComp; }
};

struct AnimOb
{
    AnimOb	*NextOb;
    AnimOb	*PrevOb;
    LONG Clock;
    WORD AnOldY, AnOldX;
    WORD AnY, AnX;
    WORD YVel, XVel;
    WORD YAccel, XAccel;
    WORD RingYTrans, RingXTrans;
    WORD (*AnimORoutine)();
    AnimComp  *HeadComp;
    AUserStuff AUserExt;

    AnimOb *next() { return NextOb; }
    AnimOb *prev() { return PrevOb; }
};

struct Bob
{
    WORD Flags;
    WORD *SaveBuffer;
    WORD *ImageShadow;
    Bob *Before;
    Bob *After;
    VSprite   *BobVSprite;
    AnimComp  *BobComp;
    DBufPacket *DBuffer;
    BUserStuff BUserExt;
};

struct DBufPacket
{
    WORD BufY, BufX;
    VSprite   *BufPath;
    WORD *BufBuffer;
};

#define InitAnimate(animKey) {*(animKey) = NULL;}
inline void RemBob(Bob *b) {(b)->Flags |= BOBSAWAY;}
#define B2NORM      0
#define B2SWAP      1
#define B2BOBBER    2

struct collTable
{
    int (*collPtrs[16])();
};

void AddAnimOb( AnimOb *a,  AnimOb **key,  RastPort *rp);
void AddBob( AnimOb *a,  RastPort *rp);

void AddVSprite( VSprite *v,  RastPort *p);
void FreeSprite(const SHORT num);

void InitGels(VSprite *first, VSprite *last,  GelsInfo *gi);
void InitMasks( VSprite *s);
void RemVSprite( VSprite *s);

void Animate( AnimOb **starter,  RastPort *r);
void FreeGBuffers( AnimOb *z,  RastPort *r, const BOOL q);

void RemBob( Bob *b);
void RemIBob( Bob *b,  RastPort *r,  ViewPort *v);
void SortGList( RastPort *r);
void DoCollision( RastPort *r);

#endif
