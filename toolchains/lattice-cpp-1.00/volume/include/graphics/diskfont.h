// Graphics/diskfont.h
//
// (C) 1987, MTS Associates
//
#ifndef  GRAPHICS_DISKFONT_H
#define  GRAPHICS_DISKFONT_H
#ifndef  GRAPHICS_TEXT_H
#include "graphics/text.h"
#endif

#ifndef DISKFONT_REV
#define DISKFONT_REV 33
#endif

TextFont *OpenDiskFont(const TextAttr *t);
int AvailFonts(APTR buffer, int bufbytes, int fonttypes);

#define AFF_MEMORY 1
#define AFF_DISK 2

struct AvailFontsDesc {
	UWORD af_FontLoc;	// as above
	TextAttr af_Attributes;
};


struct AvailFontsHeader {
	UWORD afh_NumFonts;
};	// Immediately followed by >= 1 AvailFontsDesc's

#endif
