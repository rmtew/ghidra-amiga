#ifndef DEVICES_NARRATOR_H
#define DEVICES_NARRATOR_H
/*
**	$Filename: devices/narrator.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_IO_H
#include "exec/io.h"
#endif

		/*	    Error Codes		*/

#define ND_NoMem     -2		/* Can't allocate memory		*/
#define ND_NoAudLib  -3		/* Can't open audio device		*/
#define ND_MakeBad   -4		/* Error in MakeLibrary call		*/
#define ND_UnitErr   -5		/* Unit other than 0			*/
#define ND_CantAlloc -6		/* Can't allocate audio channel(s)	*/
#define ND_Unimpl    -7		/* Unimplemented command		*/
#define ND_NoWrite   -8		/* Read for mouth without write first	*/
#define ND_Expunged  -9		/* Can't open, deferred expunge bit set */
#define ND_PhonErr   -20	/* Phoneme code spelling error		*/
#define ND_RateErr   -21	/* Rate out of bounds			*/
#define ND_PitchErr  -22	/* Pitch out of bounds			*/
#define ND_SexErr    -23	/* Sex not valid			*/
#define ND_ModeErr   -24	/* Mode not valid			*/
#define ND_FreqErr   -25	/* Sampling frequency out of bounds	*/
#define ND_VolErr    -26	/* Volume out of bounds			*/
    


		/* Input parameters and defaults */

#define DEFPITCH    110		/* Default pitch			*/
#define DEFRATE	    150		/* Default speaking rate (wpm)		*/
#define DEFVOL	    64		/* Default volume (full)		*/
#define DEFFREQ	    22200	/* Default sampling frequency (Hz)	*/
#define MALE	    0		/* Male vocal tract			*/
#define FEMALE	    1		/* Female vocal tract			*/
#define NATURALF0   0		/* Natural pitch contours		*/
#define ROBOTICF0   1		/* Monotone				*/
#define DEFSEX	    MALE	/* Default sex				*/
#define DEFMODE	    NATURALF0	/* Default mode				*/



		/*	Parameter bounds	*/

#define MINRATE	    40		/* Minimum speaking rate		*/
#define MAXRATE	    400		/* Maximum speaking rate		*/
#define MINPITCH    65		/* Minimum pitch			*/
#define MAXPITCH    320		/* Maximum pitch			*/
#define MINFREQ	    5000	/* Minimum sampling frequency		*/
#define MAXFREQ	    28000	/* Maximum sampling frequency		*/
#define MINVOL	    0		/* Minimum volume			*/
#define MAXVOL	    64		/* Maximum volume			*/



		/*    Standard Write request	*/

struct narrator_rb {
	struct IOStdReq	 message;	/* Standard IORB		*/
	UWORD	rate;			/* Speaking rate (words/minute) */
	UWORD	pitch;			/* Baseline pitch in Hertz	*/
	UWORD	mode;			/* Pitch mode			*/
	UWORD	sex;			/* Sex of voice			*/
	UBYTE	*ch_masks;		/* Pointer to audio alloc maps	*/
	UWORD	nm_masks;		/* Number of audio alloc maps	*/
	UWORD	volume;			/* Volume. 0 (off) thru 64	*/
	UWORD	sampfreq;		/* Audio sampling freq		*/
	UBYTE	mouths;			/* If non-zero, generate mouths */
	UBYTE	chanmask;		/* Which ch mask used (internal)*/
	UBYTE	numchan;		/* Num ch masks used (internal) */
	UBYTE	pad;			/* For alignment		*/
    };



		/*    Standard Read request	*/

struct mouth_rb {
	struct	narrator_rb voice;	/* Speech IORB			*/
	UBYTE	width;			/* Width (returned value)	*/
	UBYTE	height;			/* Height (returned value)	*/
	UBYTE	shape;			/* Internal use, do not modify	*/
	UBYTE	pad;			/* For alignment		*/
	};

#endif	/* DEVICES_NARRATOR_H */
