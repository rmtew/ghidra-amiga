ÄàCLIB_NONVOLATILE_PROTOS_HÄCLIB_NONVOLATILE_PROTOS_HàEXEC_TYPES_Hå<exec/types.h>áà∑å<exec/lists.h>áàLIBRARIES_NONVOLATILE_Hå<libraries/nonvolatile.h>á
îGetCopyNV(STRPTR appName,STRPTR itemName,çkillRequesters);
ãFreeNVData(îdata);
âStoreNV(STRPTR appName,STRPTR itemName,îdata,
éçlength,çkillRequesters);
BOOL DeleteNV(STRPTR appName,STRPTR itemName,çkillRequesters);
ÉNVInfo*GetNVInfo(çkillRequesters);
ÉMinList*GetNVList(STRPTR appName,çkillRequesters);
BOOL SetNVProtection(STRPTR appName,STRPTR itemName,çmask,
çkillRequesters);á