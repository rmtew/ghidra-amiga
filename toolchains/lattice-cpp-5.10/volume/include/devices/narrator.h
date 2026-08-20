// devices/narrator.h
//
// (C) 1987, MTS aAssociates
//
#ifndef DEVICES_NARRATOR_H
#define DEVICES_NARRATOR_H
#ifndef EXEC_IO_H
#include "exec/io.h"
#endif
#define ND_NoMem     -2
#define ND_NoAudLib  -3
#define ND_MakeBad   -4
#define ND_UnitErr   -5
#define ND_CantAlloc -6
#define ND_Unimpl    -7
#define ND_NoWrite   -8
#define ND_Expunged  -9
#define ND_PhonErr   -20
#define ND_RateErr   -21
#define ND_PitchErr  -22
#define ND_SexErr    -23
#define ND_ModeErr   -24
#define ND_FreqErr   -25
#define ND_VolErr    -26

#define DEFPITCH    110
#define DEFRATE     150
#define DEFVOL      64
#define DEFFREQ     22200
#define MALE        0
#define FEMALE      1
#define NATURALF0   0
#define ROBOTICF0   1
#define DEFSEX      MALE
#define DEFMODE     NATURALF0
#define MINRATE     40
#define MAXRATE     400
#define MINPITCH    65
#define MAXPITCH    320
#define MINFREQ     5000
#define MAXFREQ     28000
#define MINVOL      0
#define MAXVOL      64
struct narrator_rb : public IOStdReq {
 UWORD	rate;
 UWORD	pitch;
 UWORD	mode;
 UWORD	sex;
 UBYTE	*ch_masks;
 UWORD	nm_masks;
 UWORD	volume;
 UWORD	sampfreq;
 UBYTE	mouths;
 UBYTE	chanmask;
 UBYTE	numchan;
 UBYTE	pad;
};

struct mouth_rb : narrator_rb {
 UBYTE   width;
 UBYTE   height;
 UBYTE   shape;
 UBYTE   pad;
 };
#endif DEVICES_NARRATOR_H
