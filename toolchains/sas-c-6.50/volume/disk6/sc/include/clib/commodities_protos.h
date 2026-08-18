€ˆCLIB_COMMODITIES_PROTOS_H€CLIB_COMMODITIES_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆµŒ<exec/nodes.h>‡ˆLIBRARIES_COMMODITIES_HŒ<libraries/commodities.h>‡ˆDEVICES_INPUTEVENT_HŒ<devices/inputevent.h>‡ˆDEVICES_KEYMAP_HŒ<devices/keymap.h>‡
CxObj*CreateCxObj(type,arg1,arg2);
CxObj*CxBroker(ƒNewBroker*nb,’*error);
’ActivateCxObj(CxObj*co,true);
‹DeleteCxObj(CxObj*co);
‹DeleteCxObjAll(CxObj*co);
—CxObjType(CxObj*co);
’CxObjError(CxObj*co);
‹ClearCxObjError(CxObj*co);
’SetCxObjPri(CxObj*co,pri);
‹AttachCxObj(CxObj*headObj,CxObj*co);
‹EnqueueCxObj(CxObj*headObj,CxObj*co);
‹InsertCxObj(CxObj*headObj,CxObj*co,CxObj*pred);
‹RemoveCxObj(CxObj*co);
‹SetTranslate(CxObj*translator,ƒInputEvent*events);
‹SetFilter(CxObj*filter,STRPTR text);
‹SetFilterIX(CxObj*filter,IX*ix);
’ParseIX(STRPTR description,IX*ix);
—CxMsgType(CxMsg*cxm);
”CxMsgData(CxMsg*cxm);
’CxMsgID(CxMsg*cxm);
‹DivertCxMsg(CxMsg*cxm,CxObj*headObj,CxObj*returnObj);
‹RouteCxMsg(CxMsg*cxm,CxObj*co);
‹DisposeCxMsg(CxMsg*cxm);
BOOL InvertKeyMap(ansiCode,ƒInputEvent*event,
ƒKeyMap*km);
‹AddIEvents(ƒInputEvent*events);
BOOL MatchIX(ƒInputEvent*event,IX*ix);‡