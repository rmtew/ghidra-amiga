€ˆCLIB_AMIGAGUIDE_PROTOS_H€CLIB_AMIGAGUIDE_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆ¸Œ<exec/ports.h>‡ˆDOS_DOS_HŒ<dos/dos.h>‡ˆLIBRARIES_AMIGAGUIDE_HŒ<libraries/amigaguide.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆUTILITY_HOOKS_HŒ<utility/hooks.h>‡ˆREXX_STORAGE_HŒ<rexx/storage.h>‡
’LockAmigaGuideBase(”handle);
‹UnlockAmigaGuideBase(key);
”OpenAmigaGuideA(ƒNewAmigaGuide*nag,ƒTagItem*);
”OpenAmigaGuide(ƒNewAmigaGuide*nag,Tag tag1,...);
”OpenAmigaGuideAsyncA(ƒNewAmigaGuide*nag,ƒTagItem*attrs);
”OpenAmigaGuideAsync(ƒNewAmigaGuide*nag,Tag tag1,...);
‹CloseAmigaGuide(”cl);
—AmigaGuideSignal(”cl);
ƒAmigaGuideMsg*GetAmigaGuideMsg(”cl);
‹ReplyAmigaGuideMsg(ƒAmigaGuideMsg*amsg);
’SetAmigaGuideContextA(”cl,id,
ƒTagItem*attrs);
’SetAmigaGuideContext(”cl,id,Tag tag1,...);
’SendAmigaGuideContextA(”cl,ƒTagItem*attrs);
’SendAmigaGuideContext(”cl,Tag tag1,...);
’SendAmigaGuideCmdA(”cl,STRPTR cmd,ƒTagItem*attrs);
’SendAmigaGuideCmd(”cl,STRPTR cmd,Tag tag1,...);
’SetAmigaGuideAttrsA(”cl,ƒTagItem*attrs);
’SetAmigaGuideAttrs(”cl,Tag tag1,...);
’GetAmigaGuideAttr(Tag tag,”cl,—*storage);
’LoadXRef(¡lock,STRPTR name);
‹ExpungeXRef(‹);
”AddAmigaGuideHostA(ƒHook*h,STRPTR name,ƒTagItem*attrs);
”AddAmigaGuideHost(ƒHook*h,STRPTR name,Tag tag1,...);
’RemoveAmigaGuideHostA(”hh,ƒTagItem*attrs);
’RemoveAmigaGuideHost(”hh,Tag tag1,...);
STRPTR GetAmigaGuideString(id);‡