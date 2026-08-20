#ifndef	DEVICES_NARRATOR_H
#define	DEVICES_NARRATOR_H
#ifndef	EXEC_IO_H
#include	"exec/io.h"
#endif
#define	ND_NoMem	-2L
#define	ND_NoAudLib	-3L
#define	ND_MakeBad	-4L
#define	ND_UnitErr	-5L
#define	ND_CantAlloc	-6L
#define	ND_Unimpl	-7L
#define	ND_NoWrite	-8L
#define	ND_Expunged	-9L
#define	ND_PhonErr	-20L
#define	ND_RateErr	-21L
#define	ND_PitchErr	-22L
#define	ND_SexErr	-23L
#define	ND_ModeErr	-24L
#define	ND_FreqErr	-25L
#define	ND_VolErr	-26L
#define	DEFPITCH	110L
#define	DEFRATE	150L
#define	DEFVOL	64L
#define	DEFFREQ	22200L
#define	MALE	0L
#define	FEMALE	1L
#define	NATURALF0	0L
#define	ROBOTICF0	1L
#define	DEFSEX	MALE
#define	DEFMODE	NATURALF0
#define	MINRATE	40L
#define	MAXRATE	400L
#define	MINPITCH	65L
#define	MAXPITCH	320L
#define	MINFREQ	5000L
#define	MAXFREQ	28000L
#define	MINVOL	0L
#define	MAXVOL	64L
struct	narrator_rb	{
struct	IOStdReq	message;
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
struct	mouth_rb	{
struct	narrator_rb	voice;
UBYTE	width;
UBYTE	height;
UBYTE	shape;
UBYTE	pad;
};
#endif
