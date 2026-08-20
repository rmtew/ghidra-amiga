ÄàCLIB_EXPANSION_PROTOS_HÄCLIB_EXPANSION_PROTOS_HàEXEC_TYPES_Hå<exec/types.h>á
ãAddConfigDev(ÉConfigDev*configDev);
BOOL AddBootNode(çbootPri,éçflags,
ÉDeviceNode*deviceNode,ÉConfigDev*configDev);
ãAllocBoardMem(éçslotSpec);
ÉConfigDev*AllocConfigDev(ã);
îAllocExpansionMem(éçnumSlots,éçslotAlign);
ãConfigBoard(îboard,ÉConfigDev*configDev);
ãConfigChain(îbaseAddr);
ÉConfigDev*FindConfigDev(ÉConfigDev*oldConfigDev,
çmanufacturer,çproduct);
ãFreeBoardMem(éçstartSlot,éçslotSpec);
ãFreeConfigDev(ÉConfigDev*configDev);
ãFreeExpansionMem(éçstartSlot,éçnumSlots);
äReadExpansionByte(îboard,éçoffset);
ãReadExpansionRom(îboard,ÉConfigDev*configDev);
ãRemConfigDev(ÉConfigDev*configDev);
ãWriteExpansionByte(îboard,éçoffset,
éçbyte);
ãObtainConfigBinding(ã);
ãReleaseConfigBinding(ã);
ãSetCurrentBinding(ÉCurrentBinding*currentBinding,
éçbindingSize);
óGetCurrentBinding(ÉCurrentBinding*currentBinding,
éçbindingSize);
ÉDeviceNode*MakeDosNode(îparmPacket);
BOOL AddDosNode(çbootPri,éçflags,
ÉDeviceNode*deviceNode);á