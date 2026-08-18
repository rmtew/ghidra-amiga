#ifndef INTUITION_PREFERENCES_H
#define INTUITION_PREFERENCES_H
/*
**	$Filename: intuition/preferences.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif

#ifndef DEVICES_TIMER_H
#include "devices/timer.h"
#endif

/* ======================================================================== */
/* === Preferences ======================================================== */
/* ======================================================================== */

/* these are the definitions for the printer configurations */
#define FILENAME_SIZE	30	    /* Filename size */

#define POINTERSIZE (1 + 16 + 1) * 2	/* Size of Pointer data buffer */

/* These defines are for the default font size.	 These actually describe the
 * height of the defaults fonts.  The default font type is the topaz
 * font, which is a fixed width font that can be used in either 
 * eighty-column or sixty-column mode.	The Preferences structure reflects
 * which is currently selected by the value found in the variable FontSize,
 * which may have either of the values defined below.  These values actually
 * are used to select the height of the default font.  By changing the
 * height, the resolution of the font changes as well.
 */
#define TOPAZ_EIGHTY 8	
#define TOPAZ_SIXTY 9

struct Preferences
{
    /* the default font height */
    BYTE FontHeight;		/* height for system default font  */

    /* constant describing what's hooked up to the port */
    UBYTE PrinterPort;		/* printer port connection     */

    /* the baud rate of the port */
    USHORT BaudRate;		/* baud rate for the serial port   */
    
    /* various timing rates */
    struct timeval KeyRptSpeed;	    /* repeat speed for keyboard       */
    struct timeval KeyRptDelay;	    /* Delay before keys repeat	       */
    struct timeval DoubleClick;	    /* Interval allowed between clicks */

    /* Intuition Pointer data */
    USHORT PointerMatrix[POINTERSIZE];	/* Definition of pointer sprite	   */
    BYTE XOffset;	    /* X-Offset for active 'bit'       */
    BYTE YOffset;	    /* Y-Offset for active 'bit'       */
    USHORT color17;	    /***********************************/
    USHORT color18;	    /* Colours for sprite pointer      */
    USHORT color19;	    /***********************************/
    USHORT PointerTicks;	/* Sensitivity of the pointer	   */

    /* Workbench Screen colors */
    USHORT color0;	    /***********************************/
    USHORT color1;	    /*	Standard default colours       */
    USHORT color2;	    /*	 Used in the Workbench	       */
    USHORT color3;	    /***********************************/

    /* positioning data for the Intuition View */
    BYTE ViewXOffset;		/* Offset for top lefthand corner  */
    BYTE ViewYOffset;		/* X and Y dimensions	       */
    WORD ViewInitX, ViewInitY;	    /* View initial offset values      */

    BOOL EnableCLI;	    /* CLI availability switch */

    /* printer configurations */
    USHORT PrinterType;		/* printer type			   */
    UBYTE PrinterFilename[FILENAME_SIZE];/* file for printer	   */

    /* print format and quality configurations */
    USHORT PrintPitch;		/* print pitch		   */
    USHORT PrintQuality;	/* print quality	   */
    USHORT PrintSpacing;	/* number of lines per inch    */
    UWORD PrintLeftMargin;	/* left margin in characters	   */
    UWORD PrintRightMargin;	/* right margin in characters	   */
    USHORT PrintImage;		/* positive or negative	       */
    USHORT PrintAspect;		/* horizontal or vertical      */
    USHORT PrintShade;		/* b&w, half-tone, or color    */
    WORD PrintThreshold;	/* darkness ctrl for b/w dumps	   */

    /* print paper descriptors */
    USHORT PaperSize;		/* paper size		   */
    UWORD PaperLength;		/* paper length in number of lines */
    USHORT PaperType;		/* continuous or single sheet	   */

    /* Serial device settings: These are six nibble-fields in three bytes */
    /* (these look a little strange so the defaults will map out to zero) */ 
    UBYTE   SerRWBits;	     /* upper nibble = (8-number of read bits)	  */
			     /* lower nibble = (8-number of write bits)	  */
    UBYTE   SerStopBuf;	     /* upper nibble = (number of stop bits - 1)  */
			     /* lower nibble = (table value for BufSize)  */
    UBYTE   SerParShk;	     /* upper nibble = (value for Parity setting) */
			     /* lower nibble = (value for Handshake mode) */
    UBYTE   LaceWB;	     /* if workbench is to be interlaced      */

    UBYTE   WorkName[FILENAME_SIZE];  /* temp file for printer		*/

    BYTE    RowSizeChange; 
    BYTE    ColumnSizeChange;  

    UWORD   PrintFlags;	    /* user preference flags */
    UWORD   PrintMaxWidth;  /* max width of printed picture in 10ths/inch */
    UWORD   PrintMaxHeight; /* max height of printed picture in 10ths/inch */
    UBYTE   PrintDensity;   /* print density */
    UBYTE   PrintXOffset;   /* offset of printed picture in 10ths/inch */

    UWORD   wb_Width;	    /* override default workbench width	 */
    UWORD   wb_Height;	    /* override default workbench height */
    UBYTE   wb_Depth;	    /* override default workbench depth	 */

    UBYTE   ext_size;	    /* extension information -- do not touch! */
			    /* extension size in blocks of 64 bytes */
};


/* Workbench Interlace (use one bit) */
#define LACEWB		0x01
#define LW_RESERVED	1	 /* internal use only */

/* PrinterPort */
#define PARALLEL_PRINTER 0x00
#define SERIAL_PRINTER	0x01

/* BaudRate */
#define BAUD_110    0x00
#define BAUD_300    0x01
#define BAUD_1200   0x02
#define BAUD_2400   0x03
#define BAUD_4800   0x04
#define BAUD_9600   0x05
#define BAUD_19200  0x06
#define BAUD_MIDI   0x07

/* PaperType */
#define FANFOLD	    0x00
#define SINGLE	    0x80

/* PrintPitch */
#define PICA	    0x000
#define ELITE	    0x400
#define FINE	    0x800

/* PrintQuality */
#define DRAFT	    0x000
#define LETTER	    0x100

/* PrintSpacing */
#define SIX_LPI	    0x000
#define EIGHT_LPI   0x200

/* Print Image */
#define IMAGE_POSITIVE	0x00
#define IMAGE_NEGATIVE	0x01

/* PrintAspect */
#define ASPECT_HORIZ	0x00
#define ASPECT_VERT 0x01

/* PrintShade */
#define SHADE_BW    0x00
#define SHADE_GREYSCALE 0x01
#define SHADE_COLOR 0x02

/* PaperSize */
#define US_LETTER   0x00
#define US_LEGAL    0x10
#define N_TRACTOR   0x20
#define W_TRACTOR   0x30
#define CUSTOM	    0x40

/* PrinterType */
#define CUSTOM_NAME	0x00
#define ALPHA_P_101	0x01
#define BROTHER_15XL	    0x02
#define CBM_MPS1000	0x03
#define DIAB_630	0x04
#define DIAB_ADV_D25	    0x05
#define DIAB_C_150	0x06
#define EPSON		0x07
#define EPSON_JX_80	0x08
#define OKIMATE_20	0x09
#define QUME_LP_20	0x0A
/* new printer entries, 3 October 1985 */
#define HP_LASERJET	0x0B
#define HP_LASERJET_PLUS    0x0C

/* Serial Input Buffer Sizes */
#define SBUF_512	0x00
#define SBUF_1024	0x01
#define SBUF_2048	0x02
#define SBUF_4096	0x03
#define SBUF_8000	0x04
#define SBUF_16000	0x05

/* Serial Bit Masks */
#define SREAD_BITS  0xF0 /* for SerRWBits   */
#define SWRITE_BITS 0x0F

#define SSTOP_BITS  0xF0 /* for SerStopBuf  */
#define SBUFSIZE_BITS	0x0F

#define SPARITY_BITS	0xF0 /* for SerParShk
#define SHSHAKE_BITS	0x0F

/* Serial Parity (upper nibble, after being shifted by
 * macro SPARNUM() )
 */
#define SPARITY_NONE	 0
#define SPARITY_EVEN	 1
#define SPARITY_ODD	 2

/* Serial Handshake Mode (lower nibble, after masking using 
 * macro SHANKNUM() )
 */
#define SHSHAKE_XON	 0
#define SHSHAKE_RTS	 1
#define SHSHAKE_NONE	 2

/* new defines for PrintFlags */

#define CORRECT_RED	    0x0001  /* color correct red shades */
#define CORRECT_GREEN	    0x0002  /* color correct green shades */
#define CORRECT_BLUE	    0x0004  /* color correct blue shades */

#define CENTER_IMAGE	    0x0008  /* center image on paper */

#define IGNORE_DIMENSIONS   0x0000 /* ignore max width/height settings */
#define BOUNDED_DIMENSIONS  0x0010  /* use max width/height as boundaries */
#define ABSOLUTE_DIMENSIONS 0x0020  /* use max width/height as absolutes */
#define PIXEL_DIMENSIONS    0x0040  /* use max width/height as prt pixels */
#define MULTIPLY_DIMENSIONS 0x0080 /* use max width/height as multipliers */

#define INTEGER_SCALING	    0x0100  /* force integer scaling */

#define ORDERED_DITHERING   0x0000 /* ordered dithering */
#define HALFTONE_DITHERING  0x0200  /* halftone dithering */
#define FLOYD_DITHERING	    0x0400 /* Floyd-Steinberg dithering */

#define ANTI_ALIAS	    0x0800 /* anti-alias image */
#define GREY_SCALE2	    0x1000 /* for use with hi-res monitor */

/* masks used for checking bits */

#define CORRECT_RGB_MASK    (CORRECT_RED|CORRECT_GREEN|CORRECT_BLUE)
#define DIMENSIONS_MASK	    (BOUNDED_DIMENSIONS|ABSOLUTE_DIMENSIONS|PIXEL_DIMENSIONS|MULTIPLY_DIMENSIONS)
#define DITHERING_MASK	    (HALFTONE_DITHERING|FLOYD_DITHERING)

#endif	/* INTUITION_PREFERENCES_H */
