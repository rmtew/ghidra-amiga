// Intuition/Preferences.h
// (C) 1988, MTS Associates, Inc.  Portions (C) Commodore-Amiga, Inc.
#ifndef INTUITION_PREFERENCES_H
#define INTUITION_PREFERENCES_H

// for Intuition version 1.2

#ifndef DEVICES_TIMER_H
#include <devices/timer.h>
#endif

// Printer stuff:
const int FILENAME_SIZE = 30;
const int POINTERSIZE = (1 + 16 + 1) * 2;
#define TOPAZ_EIGHTY 8
#define TOPAZ_SIXTY 9

struct Preferences
{
    BYTE FontHeight;			/* height for system default font  */
    UBYTE PrinterPort;
    USHORT BaudRate;			/* baud rate for the serial port   */

    timeval KeyRptSpeed;
    timeval KeyRptDelay;
    timeval DoubleClick;

    /* Intuition Pointer data */
    USHORT PointerMatrix[POINTERSIZE];
    BYTE XOffset;			/* X-Offset for active 'bit'       */
    BYTE YOffset;			/* Y-Offset for active 'bit'       */
    USHORT color17;			/***********************************/
    USHORT color18;			/* Colours for sprite pointer      */
    USHORT color19;			/***********************************/
    USHORT PointerTicks;		/* Sensitivity of the pointer	   */

    /* Workbench Screen colors */
    USHORT color0;
    USHORT color1;
    USHORT color2;
    USHORT color3;

    /* positioning data for the Intuition View */
    BYTE ViewXOffset;
    BYTE ViewYOffset;
    WORD ViewInitX, ViewInitY;

    BOOL EnableCLI;

    USHORT PrinterType;
    UBYTE PrinterFilename[FILENAME_SIZE];

    /* print format and quality configurations */
    USHORT PrintPitch;
    USHORT PrintQuality;
    USHORT PrintSpacing;
    UWORD PrintLeftMargin;
    UWORD PrintRightMargin;
    USHORT PrintImage;
    USHORT PrintAspect;
    USHORT PrintShade;
    WORD PrintThreshold;

    /* print paper descriptors */
    USHORT PaperSize;
    UWORD PaperLength;
    USHORT PaperType;

    /* Serial device settings: These are six nibble-fields in three bytes */
    /* (these look a little strange so the defaults will map out to zero) */ 
    UBYTE   SerRWBits;       /* upper nibble = (8-number of read bits)    */
                             /* lower nibble = (8-number of write bits)   */
    UBYTE   SerStopBuf;      /* upper nibble = (number of stop bits - 1)  */
                             /* lower nibble = (table value for BufSize)  */
    UBYTE   SerParShk;       /* upper nibble = (value for Parity setting) */
                             /* lower nibble = (value for Handshake mode) */
    UBYTE   LaceWB;	     /* if workbench is to be interlaced	  */

    UBYTE   WorkName[FILENAME_SIZE];  /* temp file for printer          */
    BYTE	RowSizeChange;
    BYTE	ColumnSizeChange;
    BYTE    padding[14];
};


/* Workbench Interlace (use one bit) */
#define LACEWB 0x01

/* PrinterPort */
#define PARALLEL_PRINTER 0x00
#define SERIAL_PRINTER	0x01

/* BaudRate */
#define BAUD_110	0x00
#define BAUD_300	0x01
#define BAUD_1200	0x02
#define BAUD_2400	0x03
#define BAUD_4800	0x04
#define BAUD_9600	0x05
#define BAUD_19200	0x06
#define BAUD_MIDI	0x07

/* PaperType */
#define FANFOLD 	0x00
#define SINGLE		0x80

/* PrintPitch */
#define PICA		0x000
#define ELITE		0x400
#define FINE		0x800

/* PrintQuality */
#define DRAFT		0x000
#define LETTER		0x100

/* PrintSpacing */
#define SIX_LPI		0x000
#define EIGHT_LPI	0x200

/* Print Image */
#define IMAGE_POSITIVE	0x00
#define IMAGE_NEGATIVE	0x01

/* PrintAspect */
#define ASPECT_HORIZ	0x00
#define ASPECT_VERT	0x01

/* PrintShade */
#define SHADE_BW	0x00
#define SHADE_GREYSCALE	0x01
#define SHADE_COLOR	0x02

/* PaperSize */
#define US_LETTER	0x00
#define US_LEGAL	0x10
#define N_TRACTOR	0x20
#define W_TRACTOR	0x30
#define CUSTOM		0x40

/* PrinterType */
#define CUSTOM_NAME		0x00
#define	ALPHA_P_101		0x01
#define BROTHER_15XL		0x02
#define CBM_MPS1000		0x03
#define DIAB_630		0x04
#define DIAB_ADV_D25		0x05
#define DIAB_C_150		0x06
#define EPSON			0x07
#define EPSON_JX_80		0x08
#define OKIMATE_20		0x09
#define QUME_LP_20		0x0A
/* new printer entries, 3 October 1985 */
#define HP_LASERJET		0x0B
#define HP_LASERJET_PLUS	0x0C

/* Serial Input Buffer Sizes */
#define SBUF_512        0x00
#define SBUF_1024       0x01
#define SBUF_2048       0x02
#define SBUF_4096       0x03
#define SBUF_8000       0x04
#define SBUF_16000      0x05

/* Serial Bit Masks */
#define	SREAD_BITS	0xF0 /* for SerRWBits	*/
#define	SWRITE_BITS	0x0F

#define	SSTOP_BITS	0xF0 /* for SerStopBuf	*/
#define	SBUFSIZE_BITS	0x0F

#define	SPARITY_BITS    0xF0 /* for SerParShk */
#define SHSHAKE_BITS    0x0F

/* Serial Parity (upper nibble, after being shifted by
 * macro SPARNUM() )
 */
#define SPARITY_NONE     0
#define SPARITY_EVEN     1
#define SPARITY_ODD      2

/* Serial Handshake Mode (lower nibble, after masking using 
 * macro SHANKNUM() )
 */
#define SHSHAKE_XON      0
#define SHSHAKE_RTS      1
#define SHSHAKE_NONE     2

/* === Miscellaneous ===================================================== */

#define SRBNUM(n)    (0x08 - (n >> 4))  /* SerRWBits -> read bits per char */
#define SWBNUM(n)    (0x08 - (n & 0x0F))/* SerRWBits -> write bits per chr */
#define SSBNUM(n)    (0x01 + (n >> 4))  /* SerStopBuf -> stop bits per chr */
#define SPARNUM(n)   (n >> 4)           /* SerParShk -> parity setting    */
#define SHAKNUM(n)   (n & 0x0F)         /* SerParShk -> handshake mode    */


struct Preferences * GetDefPrefs (Preferences *p, SHORT numbytes);
struct Preferences * GetPrefs (Preferences *p, SHORT numbytes);
#endif INTUITION_PREFERENCES_H
