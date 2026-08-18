€ˆLIBRARIES_CONFIGVARS_H€LIBRARIES_CONFIGVARS_HˆEXEC_TYPES_HŒ"exec/types.h"‡ˆµŒ"exec/nodes.h"‡ˆLIBRARIES_CONFIGREGS_HŒ"libraries/configregs.h"‡
ƒConfigDev{
ƒ¬cd_Node;
Šcd_Flags;
Šcd_Pad;
ƒExpansionRom cd_Rom;
”cd_BoardAddr;
—cd_BoardSize;
‰cd_SlotAddr;
‰cd_SlotSize;
”cd_Driver;
ƒConfigDev*cd_NextCD;
—cd_Unused[4];
};€CDB_SHUTUP 0€CDB_CONFIGME 1€CDB_BADMEMORY 2€CDB_PROCESSED 3€CDF_SHUTUP 1€CDF_CONFIGME 2€CDF_BADMEMORY 4€CDF_PROCESSED 8
ƒCurrentBinding{
ƒConfigDev*cb_ConfigDev;
Š*cb_FileName;
Š*cb_ProductString;
Š**cb_ToolTypes;
};‡