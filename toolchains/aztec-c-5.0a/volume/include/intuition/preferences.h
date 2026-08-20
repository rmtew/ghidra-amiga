#ifndef	INTUITION_PREFERENCES_H
#define	INTUITION_PREFERENCES_H
#ifndef	EXEC_TYPES_H
#include	"exec/types.h"
#endif
#ifndef	DEVICES_TIMER_H
#include	"devices/timer.h"
#endif
#define	FILENAME_SIZE	30
#define	POINTERSIZE	(1	+	16	+	1)	*	2
#define	TOPAZ_EIGHTY	8
#define	TOPAZ_SIXTY	9
struct	Preferences
{
BYTE	FontHeight;
UBYTE	PrinterPort;
USHORT	BaudRate;
struct	timeval	KeyRptSpeed;
struct	timeval	KeyRptDelay;
struct	timeval	DoubleClick;
USHORT	PointerMatrix[POINTERSIZE];
BYTE	XOffset;
BYTE	YOffset;
USHORT	color17;
USHORT	color18;
USHORT	color19;
USHORT	PointerTicks;
USHORT	color0;
USHORT	color1;
USHORT	color2;
USHORT	color3;
BYTE	ViewXOffset;
BYTE	ViewYOffset;
WORD	ViewInitX,	ViewInitY;
BOOL	EnableCLI;
USHORT	PrinterType;
UBYTE	PrinterFilename[FILENAME_SIZE];
USHORT	PrintPitch;
USHORT	PrintQuality;
USHORT	PrintSpacing;
UWORD	PrintLeftMargin;
UWORD	PrintRightMargin;
USHORT	PrintImage;
USHORT	PrintAspect;
USHORT	PrintShade;
WORD	PrintThreshold;
USHORT	PaperSize;
UWORD	PaperLength;
USHORT	PaperType;
UBYTE	SerRWBits;
UBYTE	SerStopBuf;
UBYTE	SerParShk;
UBYTE	LaceWB;
UBYTE	WorkName[FILENAME_SIZE];
BYTE	RowSizeChange;
BYTE	ColumnSizeChange;
UWORD	PrintFlags;
UWORD	PrintMaxWidth;
UWORD	PrintMaxHeight;
UBYTE	PrintDensity;
UBYTE	PrintXOffset;
UWORD	wb_Width;
UWORD	wb_Height;
UBYTE	wb_Depth;
UBYTE	ext_size;
};
#define	LACEWB	0x01
#define	LW_RESERVED	1
#define	PARALLEL_PRINTER	0x00
#define	SERIAL_PRINTER	0x01
#define	BAUD_110	0x00
#define	BAUD_300	0x01
#define	BAUD_1200	0x02
#define	BAUD_2400	0x03
#define	BAUD_4800	0x04
#define	BAUD_9600	0x05
#define	BAUD_19200	0x06
#define	BAUD_MIDI	0x07
#define	FANFOLD	0x00
#define	SINGLE	0x80
#define	PICA	0x000
#define	ELITE	0x400
#define	FINE	0x800
#define	DRAFT	0x000
#define	LETTER	0x100
#define	SIX_LPI	0x000
#define	EIGHT_LPI	0x200
#define	IMAGE_POSITIVE	0x00
#define	IMAGE_NEGATIVE	0x01
#define	ASPECT_HORIZ	0x00
#define	ASPECT_VERT	0x01
#define	SHADE_BW	0x00
#define	SHADE_GREYSCALE	0x01
#define	SHADE_COLOR	0x02
#define	US_LETTER	0x00
#define	US_LEGAL	0x10
#define	N_TRACTOR	0x20
#define	W_TRACTOR	0x30
#define	CUSTOM	0x40
#define	CUSTOM_NAME	0x00
#define	ALPHA_P_101	0x01
#define	BROTHER_15XL	0x02
#define	CBM_MPS1000	0x03
#define	DIAB_630	0x04
#define	DIAB_ADV_D25	0x05
#define	DIAB_C_150	0x06
#define	EPSON	0x07
#define	EPSON_JX_80	0x08
#define	OKIMATE_20	0x09
#define	QUME_LP_20	0x0A
#define	HP_LASERJET	0x0B
#define	HP_LASERJET_PLUS	0x0C
#define	SBUF_512	0x00
#define	SBUF_1024	0x01
#define	SBUF_2048	0x02
#define	SBUF_4096	0x03
#define	SBUF_8000	0x04
#define	SBUF_16000	0x05
#define	SREAD_BITS	0xF0
#define	SWRITE_BITS	0x0F
#define	SSTOP_BITS	0xF0
#define	SBUFSIZE_BITS	0x0F
#define	SPARITY_BITS	0xF0
#define	SPARITY_NONE	0
#define	SPARITY_EVEN	1
#define	SPARITY_ODD	2
#define	SHSHAKE_XON	0
#define	SHSHAKE_RTS	1
#define	SHSHAKE_NONE	2
#define	CORRECT_RED	0x0001
#define	CORRECT_GREEN	0x0002
#define	CORRECT_BLUE	0x0004
#define	CENTER_IMAGE	0x0008
#define	IGNORE_DIMENSIONS	0x0000
#define	BOUNDED_DIMENSIONS	0x0010
#define	ABSOLUTE_DIMENSIONS	0x0020
#define	PIXEL_DIMENSIONS	0x0040
#define	MULTIPLY_DIMENSIONS	0x0080
#define	INTEGER_SCALING	0x0100
#define	ORDERED_DITHERING	0x0000
#define	HALFTONE_DITHERING	0x0200
#define	FLOYD_DITHERING	0x0400
#define	ANTI_ALIAS	0x0800
#define	GREY_SCALE2	0x1000
#define	CORRECT_RGB_MASK	(CORRECT_RED|CORRECT_GREEN|CORRECT_BLUE)
#define	DIMENSIONS_MASK	(BOUNDED_DIMENSIONS|ABSOLUTE_DIMENSIONS|PIXEL_DIMENSIONS|MULTIPLY_DIMENSIONS)
#define	DITHERING_MASK	(HALFTONE_DITHERING|FLOYD_DITHERING)
#endif
