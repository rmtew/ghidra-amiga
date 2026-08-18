#ifndef     LIBRARIES_DISKFONT_H
#include <libraries/diskfont.h>
#endif
extern struct Library *DiskfontBase;
struct TextFont *OpenDiskFont(struct TextAttr *);
long AvailFonts(char *, long, long);
#ifndef  NO_PRAGMAS
#pragma libcall DiskfontBase OpenDiskFont 1e 801
#pragma libcall DiskfontBase AvailFonts 24 10803
#endif
