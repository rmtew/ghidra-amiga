€ˆCLIB_REALTIME_PROTOS_H€CLIB_REALTIME_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆLIBRARIES_REALTIME_HŒ<libraries/realtime.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡
”LockRealTime(lockType);
‹UnlockRealTime(”lock);
ƒPlayer*CreatePlayerA(ƒTagItem*tagList);
ƒPlayer*CreatePlayer(Tag tag1,...);
‹DeletePlayer(ƒPlayer*player);
BOOL SetPlayerAttrsA(ƒPlayer*player,ƒTagItem*tagList);
BOOL SetPlayerAttrs(ƒPlayer*player,Tag tag1,...);
’SetConductorState(ƒPlayer*player,state,
time);
BOOL ExternalSync(ƒPlayer*player,minTime,maxTime);
ƒConductor*NextConductor(ƒConductor*previousConductor);
ƒConductor*FindConductor(STRPTR name);
—GetPlayerAttrsA(ƒPlayer*player,ƒTagItem*tagList);
—GetPlayerAttrs(ƒPlayer*player,Tag tag1,...);‡