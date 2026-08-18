€ˆCLIB_UTILITY_PROTOS_H€CLIB_UTILITY_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆ¸Œ<exec/ports.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆUTILITY_DATE_HŒ<utility/date.h>‡ˆUTILITY_HOOKS_HŒ<utility/hooks.h>‡ˆUTILITY_NAME_HŒ<utility/name.h>‡
ƒTagItem*FindTagItem(Tag tagVal,ƒTagItem*tagList);
—GetTagData(Tag tagValue,defaultVal,
ƒTagItem*tagList);
—PackBoolTags(initialFlags,ƒTagItem*tagList,
ƒTagItem*boolMap);
ƒTagItem*NextTagItem(ƒTagItem**tagListPtr);
‹FilterTagChanges(ƒTagItem*changeList,
ƒTagItem*originalList,apply);
‹MapTags(ƒTagItem*tagList,ƒTagItem*mapList,
mapType);
ƒTagItem*AllocateTagItems(numTags);
ƒTagItem*CloneTagItems(ƒTagItem*tagList);
‹FreeTagItems(ƒTagItem*tagList);
‹RefreshTagItemClones(ƒTagItem*clone,ƒTagItem*original);
BOOL TagInArray(Tag tagValue,Tag*tagArray);
—FilterTagItems(ƒTagItem*tagList,Tag*filterArray,
logic);
—CallHookPkt(ƒHook*hook,”object,”paramPacket);
‹Amiga2Date(seconds,ƒClockData*result);
—Date2Amiga(ƒClockData*date);
—CheckDate(ƒClockData*date);
’SMult32(arg1,arg2);
—UMult32(arg1,arg2);
’SDivMod32(dividend,divisor);
—UDivMod32(dividend,divisor);
’Stricmp(STRPTR string1,STRPTR string2);
’Strnicmp(STRPTR string1,STRPTR string2,length);
ŠToUpper(character);
ŠToLower(character);
‹ApplyTagChanges(ƒTagItem*list,ƒTagItem*changeList);
’SMult64(arg1,arg2);
—UMult64(arg1,arg2);
—PackStructureTags(”pack,—*packTable,
ƒTagItem*tagList);
—UnpackStructureTags(”pack,—*packTable,
ƒTagItem*tagList);
BOOL AddNamedObject(ƒNamedObject*nameSpace,
ƒNamedObject*object);
ƒNamedObject*AllocNamedObjectA(STRPTR name,ƒTagItem*tagList);
ƒNamedObject*AllocNamedObject(STRPTR name,Tag tag1,...);
’AttemptRemNamedObject(ƒNamedObject*object);
ƒNamedObject*FindNamedObject(ƒNamedObject*nameSpace,
STRPTR name,ƒNamedObject*lastObject);
‹FreeNamedObject(ƒNamedObject*object);
STRPTR NamedObjectName(ƒNamedObject*object);
‹ReleaseNamedObject(ƒNamedObject*object);
‹RemNamedObject(ƒNamedObject*object,ƒ¯*message);
—GetUniqueID(‹);‡