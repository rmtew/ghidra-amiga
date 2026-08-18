#ifndef	LIBRARIES_DISKFONT_H
#define	LIBRARIES_DISKFONT_H
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif /* !EXEC_TYPES_H */
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif
#ifndef	GRAPHICS_TEXT_H
#include	"graphics/text.h"
#endif
#define	MAXFONTPATH	256
struct	FontContents	{
char	fc_FileName[MAXFONTPATH];
UWORD	fc_YSize;
UBYTE	fc_Style;
UBYTE	fc_Flags;
};
#define	FCH_ID	0x0f00
struct	FontContentsHeader	{
UWORD	fch_FileID;
UWORD	fch_NumEntries;
};
#define	DFH_ID	0x0f80
#define	MAXFONTNAME	32
struct	DiskFontHeader	{
struct	Node	dfh_DF;
UWORD	dfh_FileID;
UWORD	dfh_Revision;
LONG	dfh_Segment;
char	dfh_Name[MAXFONTNAME];
struct	TextFont	dfh_TF;
};
#define	AFB_MEMORY	0
#define	AFF_MEMORY	1
#define	AFB_DISK	1
#define	AFF_DISK	2
struct	AvailFonts	{
UWORD	af_Type;
struct	TextAttr	af_Attr;
};
struct	AvailFontsHeader	{
UWORD	afh_NumEntries;
};
#endif
