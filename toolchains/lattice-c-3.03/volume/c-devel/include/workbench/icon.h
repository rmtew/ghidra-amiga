
#ifndef LIBRARIES_ICON_H
#define LIBRARIES_ICON_H












#define  ICONNAME "icon.library"



struct WBObject *GetWBObject(), *AllocWBObject();
LONG   PutWBObject(), PutIcon(), GetIcon(), MatchToolValue();
VOID   FreeFreeList(), FreeWBObject(), AddFreeList();
char   *ToolTypeArray();


#endif !LIBRARIES_ICON_H
