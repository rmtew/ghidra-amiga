ÄàCLIB_ICON_PROTOS_HÄCLIB_ICON_PROTOS_HàEXEC_TYPES_Hå<exec/types.h>áàWORKBENCH_WORKBENCH_Hå<workbench/workbench.h>á
ãFreeFreeList(ÉFreeList*freelist);
BOOL AddFreeList(ÉFreeList*freelist,îmem,éçsize);
ÉDiskObject*GetDiskObject(ä*name);
BOOL PutDiskObject(ä*name,ÉDiskObject*diskobj);
ãFreeDiskObject(ÉDiskObject*diskobj);
ä*FindToolType(ä**toolTypeArray,ä*typeName);
BOOL MatchToolValue(ä*typeString,ä*value);
ä*BumpRevision(ä*newname,ä*oldname);
ÉDiskObject*GetDefDiskObject(çtype);
BOOL PutDefDiskObject(ÉDiskObject*diskObject);
ÉDiskObject*GetDiskObjectNew(ä*name);
BOOL DeleteDiskObject(ä*name);á