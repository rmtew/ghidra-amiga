€ˆCLIB_ALIB_PROTOS_H€CLIB_ALIB_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆDEVICES_TIMER_HŒ<devices/timer.h>‡ˆDEVICES_KEYMAP_HŒ<devices/keymap.h>‡ˆLIBRARIES_COMMODITIES_HŒ<libraries/commodities.h>‡ˆUTILITY_HOOKS_HŒ<utility/hooks.h>‡ˆINTUITION_CLASSES_HŒ<intuition/classes.h>‡ˆINTUITION_CLASSUSR_HŒ<intuition/classusr.h>‡ˆGRAPHICS_GRAPHINT_HŒ<graphics/graphint.h>‡
‹BeginIO(ƒIORequest*ioReq);
ƒIORequest*CreateExtIO(ƒ©*port,ioSize);
ƒ©*CreatePort(STRPTR name,pri);
ƒIOStdReq*CreateStdIO(ƒ©*port);
ƒTask*CreateTask(STRPTR name,pri,”initPC,
stackSize);
‹DeleteExtIO(ƒIORequest*ioReq);
‹DeletePort(ƒ©*ioReq);
‹DeleteStdIO(ƒIOStdReq*ioReq);
‹DeleteTask(ƒTask*task);
‹NewList(ƒ®*list);
”LibAllocPooled(”poolHeader,memSize);
”LibCreatePool(memFlags,puddleSize,
threshSize);
‹LibDeletePool(”poolHeader);
‹LibFreePooled(”poolHeader,”memory,memSize);
—FastRand(seed);
‰RangeRand(maxValue);
‹AddTOF(ƒIsrvstr*i,(*p)(),a);
‹RemTOF(ƒIsrvstr*i);
‹waitbeam(b);
¤afp(š*string);
‹arnd(place,exp,š*string);
¤dbf(exp,mant);
’fpa(¤fnum,š*string);
‹fpbcd(¤fnum,š*string);
’TimeDelay(unit,secs,microsecs);
’DoTimer(ƒtimeval*,unit,command);
‹ArgArrayDone(‹);
Š**ArgArrayInit(argc,Š**argv);
’ArgInt(Š**tt,STRPTR entry,defaultval);
STRPTR ArgString(Š**tt,STRPTR entry,STRPTR defaulstring);
CxObj*HotKey(STRPTR description,ƒ©*port,id);
ƒInputEvent*InvertString(STRPTR str,ƒKeyMap*km);
‹FreeIEvents(ƒInputEvent*events);
BOOL CheckRexxMsg(ƒ¯*rexxmsg);
’GetRexxVar(ƒ¯*rexxmsg,Š*name,Š**result);
’SetRexxVar(ƒ¯*rexxmsg,Š*name,Š*value,
length);
—CallHookA(ƒHook*hookPtr,Object*obj,”message);
—CallHook(ƒHook*hookPtr,Object*obj,...);
—DoMethodA(Object*obj,Msg message);
—DoMethod(Object*obj,MethodID,...);
—DoSuperMethodA(ƒIClass*cl,Object*obj,Msg message);
—DoSuperMethod(ƒIClass*cl,Object*obj,MethodID,
...);
—CoerceMethodA(ƒIClass*cl,Object*obj,Msg message);
—CoerceMethod(ƒIClass*cl,Object*obj,MethodID,
...);
—SetSuperAttrs(ƒIClass*cl,Object*obj,Tag1,
...);
STRPTR ACrypt(STRPTR buffer,STRPTR password,STRPTR username);‡