extern struct Library *IconBase;
/*------ normal functions ---------------------------------------------*/
struct WBObject *GetWBObject(char *);
long PutWBObject(char *, struct WBObject *);
long GetIcon(char *, struct DiskObject *, struct FreeList *);
long PutIcon(char *, struct DiskObject *);
void FreeFreeList(struct FreeList *);
void FreeWBObject(struct WBObject *);
struct WBObject AllocWBObject(void);
long AddFreeList(struct FreeList *, char *, long);
struct DiskObject *GetDiskObject(char *);
long PutDiskObject(char *, struct DiskObject *);
void FreeDiskObject(struct DiskObject *);
char * FindToolType(char **, char *);
long MatchToolValue(char **, char *);
long BumpRevision(char *, char *);
#ifndef  NO_PRAGMAS
/*------ normal functions ---------------------------------------------*/
#pragma libcall IconBase GetWBObject 1e 801
#pragma libcall IconBase PutWBObject 24 9802
#pragma libcall IconBase GetIcon 2a a9803
#pragma libcall IconBase PutIcon 30 9802
#pragma libcall IconBase FreeFreeList 36 801
#pragma libcall IconBase FreeWBObject 3c 801
#pragma libcall IconBase AllocWBObject 42 0
#pragma libcall IconBase AddFreeList 48 a9803
#pragma libcall IconBase GetDiskObject 4e 801
#pragma libcall IconBase PutDiskObject 54 9802
#pragma libcall IconBase FreeDiskObject 5a 801
#pragma libcall IconBase FindToolType 60 9802
#pragma libcall IconBase MatchToolValue 66 9802
#pragma libcall IconBase BumpRevision 6c 9802
#endif
