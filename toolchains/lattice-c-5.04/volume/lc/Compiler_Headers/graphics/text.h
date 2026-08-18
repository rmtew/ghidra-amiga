#ifndef GRAPHICS_TEXT_H
#define GRAPHICS_TEXT_H
/*
**	$Filename: graphics/text.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef	    EXEC_PORTS_H
#include "exec/ports.h"
#endif

/*------ Font Styles ------------------------------------------------*/
#define FS_NORMAL    0	      /* normal text (no style bits set) */
#define FSB_EXTENDED 3	      /* extended face (wider than normal) */
#define FSF_EXTENDED (1<<3)
#define FSB_ITALIC   2	      /* italic (slanted 1:2 right) */
#define FSF_ITALIC   (1<<2)
#define FSB_BOLD     1	      /* bold face text (ORed w/ shifted) */
#define FSF_BOLD     (1<<1)
#define FSB_UNDERLINED	0     /* underlined (under baseline) */
#define FSF_UNDERLINED	(1<<0)

/*------ Font Flags -------------------------------------------------*/
#define FPB_ROMFONT  0	      /* font is in rom */
#define FPF_ROMFONT  (1<<0)
#define FPB_DISKFONT 1	      /* font is from diskfont.library */
#define FPF_DISKFONT (1<<1)
#define FPB_REVPATH  2	      /* designed path is reversed (e.g. left) */
#define FPF_REVPATH  (1<<2)
#define	 FPB_TALLDOT 3	      /* designed for hires non-interlaced */
#define	 FPF_TALLDOT (1<<3)
#define	 FPB_WIDEDOT 4	      /* designed for lores interlaced */
#define	 FPF_WIDEDOT (1<<4)
#define FPB_PROPORTIONAL   5  /* character sizes can vary from nominal */
#define FPF_PROPORTIONAL   (1<<5)
#define FPB_DESIGNED 6	      /* size is "designed", not constructed */
#define FPF_DESIGNED (1<<6)
#define FPB_REMOVED  7	      /* the font has been removed */
#define FPF_REMOVED  (1<<7)

/****** TextAttr node, matches text attributes in RastPort **********/
struct TextAttr {
   STRPTR   ta_Name;	      /* name of the font */
   UWORD    ta_YSize;	      /* height of the font */
   UBYTE    ta_Style;	      /* intrinsic font style */
   UBYTE    ta_Flags;	      /* font preferences and flags */
};


/****** TextFonts node **********************************************/
struct TextFont {
   struct Message tf_Message;	    /* reply message for font removal */
			      /* font name in LN	\    used in this */
   UWORD    tf_YSize;	      /* font height		|    order to best */
   UBYTE    tf_Style;	      /* font style		|    match a font */
   UBYTE    tf_Flags;	      /* preferences and flags	/    request. */
   UWORD    tf_XSize;	      /* nominal font width */
   UWORD    tf_Baseline;      /* distance from the top of char to baseline */
   UWORD    tf_BoldSmear;     /* smear to affect a bold enhancement */

   UWORD    tf_Accessors;     /* access count */

   UBYTE    tf_LoChar;	      /* the first character described here */
   UBYTE    tf_HiChar;	      /* the last character described here */
   APTR	    tf_CharData;      /* the bit character data */

   UWORD    tf_Modulo;	      /* the row modulo for the strike font data */
   APTR	    tf_CharLoc;	      /* ptr to location data for the strike font */
	     /*	   2 words: bit offset then size */
   APTR	    tf_CharSpace;     /* ptr to words of proportional spacing data */
   APTR	    tf_CharKern;      /* ptr to words of kerning data */
};

#endif	/* GRAPHICS_TEXT_H */
