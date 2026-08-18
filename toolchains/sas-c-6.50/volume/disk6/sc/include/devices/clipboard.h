€ˆDEVICES_CLIPBOARD_H€DEVICES_CLIPBOARD_HˆEXEC_TYPES_HŒ"exec/types.h"‡ˆµŒ"exec/nodes.h"‡ˆ·Œ"exec/lists.h"‡ˆ¸Œ"exec/ports.h"‡€CBD_POST (CMD_NONSTD+0)€CBD_CURRENTREADID (CMD_NONSTD+1)€CBD_CURRENTWRITEID (CMD_NONSTD+2)€CBD_CHANGEHOOK (CMD_NONSTD+3)€CBERR_OBSOLETEID 1
ƒClipboardUnitPartial{
ƒ¬cu_Node;
—cu_UnitNum;
};
ƒIOClipReq{
ƒ¯io_Message;
ƒDevice*io_Device;
ƒClipboardUnitPartial*io_Unit;
‰io_Command;
Šio_Flags;
šio_Error;
—io_Actual;
—io_Length;
STRPTR io_Data;
—io_Offset;
’io_ClipID;
};€PRIMARY_CLIP 0
ƒSatisfyMsg{
ƒ¯sm_Msg;
‰sm_Unit;
’sm_ClipID;
};
ƒClipHookMsg{
—chm_Type;
’chm_ChangeCmd;
’chm_ClipID;
};‡