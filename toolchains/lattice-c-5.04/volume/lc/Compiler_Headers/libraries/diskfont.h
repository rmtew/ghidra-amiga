#ifndef LIBRARIES_DISKFONT_H
#define LIBRARIES_DISKFONT_H
/*
**	$Filename: libraries/diskfont.h $
**	$Release: 1.3 $
**
**	diskfont library definitions 
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef	    EXEC_NODES_H
#include "exec/nodes.h"
#endif
#ifndef	    EXEC_LISTS_H
#include "exec/lists.h"
#endif
#ifndef	    GRAPHICS_TEXT_H
#include "graphics/text.h"
#endif

#define	    MAXFONTPATH 256   /* including null terminator */

struct FontContents {
   char	 fc_FileName[MAXFONTPATH];
   UWORD fc_YSize;
   UBYTE fc_Style;
   UBYTE fc_Flags;
};

#define	    FCH_ID	0x0f00

struct FontContentsHeader {
   UWORD fch_FileID; /* FCH_ID */
   UWORD fch_NumEntries;   /* the number of FontContents elements */
    /*	 struct FontContents fch_FC[]; */
};

#define	    DFH_ID	0x0f80
#define	    MAXFONTNAME 32 /* font name including ".font\0" */

struct DiskFontHeader {
    /* the following 8 bytes are not actually considered a part of the */
    /* DiskFontHeader, but immediately preceed it. The NextSegment is  */
    /* supplied by the linker/loader, and the ReturnCode is the code   */
    /* at the beginning of the font in case someone runs it...	       */
    /*	 ULONG dfh_NextSegment;/* actually a BPTR */
    /*	 ULONG dfh_ReturnCode;	 /* MOVEQ #0,D0 : RTS */
    /* here then is the official start of the DiskFontHeader...	     */
   struct   Node dfh_DF;   /* node to link disk fonts */
   UWORD dfh_FileID; /* DFH_ID */
   UWORD dfh_Revision;	/* the font revision */
   LONG	 dfh_Segment;	/* the segment address when loaded */
   char	 dfh_Name[MAXFONTNAME];	 /* the font name (null terminated) */
   struct   TextFont dfh_TF;/* loaded TextFont structure */
};


#define	    AFB_MEMORY	0
#define	    AFF_MEMORY	1
#define	    AFB_DISK 1
#define	    AFF_DISK 2

struct AvailFonts {
   UWORD af_Type;    /* MEMORY or DISK */
   struct   TextAttr af_Attr; /* text attributes for font */
};

struct AvailFontsHeader {
   UWORD afh_NumEntries;      /* number of AvailFonts elements */
    /*	 struct	  AvailFonts afh_AF[]; */
};

#endif	/* LIBRARIES_DISKFONT_H */
