// workbench/icon.h
//
// (C) 1987, MTS Associates
//
#ifndef LIBRARIES_ICON_H
#define LIBRARIES_ICON_H
#define ICONNAME "icon.library"

#ifndef WORKBENCH_WORKBENCH_H
#include <workbench/workbench.h>
#endif

WBObject *GetWBObject(const char *filename);
WBObject *AllocWBObject();
DiskObject *GetDiskObject(const char *filename);
BOOL PutWBObject(const char *filename, WBObject *w);
BOOL PutIcon(const char *filename, DiskObject *d);
BOOL GetIcon(const char *filename, DiskObject *d, FreeList *f);
BOOL MatchToolValue(const char *string, const char *substring);
VOID FreeFreeList(FreeList *f);
VOID FreeWBObject(WBObject *w);
BOOL AddFreeList(FreeList *f, APTR m, const LONG numbytes);
char *FindToolType(const char *tooltype, const char *typename);
BOOL BumpRevision(char *newname, const char *oldname);
#endif !LIBRARIES_ICON_H
