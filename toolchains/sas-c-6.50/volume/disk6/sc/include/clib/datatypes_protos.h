€ˆCLIB_DATATYPES_PROTOS_H€CLIB_DATATYPES_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆ·Œ<exec/lists.h>‡ˆINTUITION_INTUITION_HŒ<intuition/intuition.h>‡ˆINTUITION_CLASSES_HŒ<intuition/classes.h>‡ˆINTUITION_CLASSUSR_HŒ<intuition/classusr.h>‡ˆINTUITION_GADGETCLASS_HŒ<intuition/gadgetclass.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆDATATYPES_DATATYPES_HŒ<datatypes/datatypes.h>‡ˆREXX_STORAGE_HŒ<rexx/storage.h>‡
ƒDataType*ObtainDataTypeA(type,”handle,
ƒTagItem*attrs);
ƒDataType*ObtainDataType(type,”handle,Tag Tag1,
...);
‹ReleaseDataType(ƒDataType*dt);
Object*NewDTObjectA(”name,ƒTagItem*attrs);
Object*NewDTObject(”name,Tag Tag1,...);
‹DisposeDTObject(Object*o);
—SetDTAttrsA(Object*o,ƒWindow*win,ƒRequester*req,
ƒTagItem*attrs);
—SetDTAttrs(Object*o,ƒWindow*win,ƒRequester*req,
Tag Tag1,...);
—GetDTAttrsA(Object*o,ƒTagItem*attrs);
—GetDTAttrs(Object*o,Tag Tag1,...);
’AddDTObject(ƒWindow*win,ƒRequester*req,Object*o,
pos);
‹RefreshDTObjectA(Object*o,ƒWindow*win,ƒRequester*req,
ƒTagItem*attrs);
‹RefreshDTObjects(Object*o,ƒWindow*win,ƒRequester*req,
Tag Tag1,...);
—DoAsyncLayout(Object*o,ƒgpLayout*gpl);
—DoDTMethodA(Object*o,ƒWindow*win,ƒRequester*req,
Msg msg);
—DoDTMethod(Object*o,ƒWindow*win,ƒRequester*req,
data,...);
’RemoveDTObject(ƒWindow*win,Object*o);
—*GetDTMethods(Object*object);
ƒDTMethods*GetDTTriggerMethods(Object*object);
—PrintDTObjectA(Object*o,ƒWindow*w,ƒRequester*r,
ƒdtPrint*msg);
—PrintDTObject(Object*o,ƒWindow*w,ƒRequester*r,
data,...);
STRPTR GetDTString(id);‡