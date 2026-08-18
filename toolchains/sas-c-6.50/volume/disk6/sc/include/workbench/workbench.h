ÄàWORKBENCH_WORKBENCH_HÄWORKBENCH_WORKBENCH_HàEXEC_TYPES_Hå"exec/types.h"áàµå"exec/nodes.h"áà∑å"exec/lists.h"áàEXEC_TASKS_Hå"exec/tasks.h"áàINTUITION_INTUITION_Hå"intuition/intuition.h"áÄWBDISK 1ÄWBDRAWER 2ÄWBTOOL 3ÄWBPROJECT 4ÄWBGARBAGE 5ÄWBDEVICE 6ÄWBKICK 7ÄWBAPPICON 8
ÉOldDrawerData{
ÉNewWindow dd_NewWindow;
ídd_CurrentX;
ídd_CurrentY;
};ÄOLDDRAWERDATAFILESIZE (sizeof(ÉOldDrawerData))
ÉDrawerData{
ÉNewWindow dd_NewWindow;
ídd_CurrentX;
ídd_CurrentY;
ódd_Flags;
âdd_ViewModes;
};ÄDRAWERDATAFILESIZE (sizeof(ÉDrawerData))
ÉDiskObject{
âdo_Magic;
âdo_Version;
Éªdo_Gadget;
ädo_Type;
Ñ*do_DefaultTool;
Ñ**do_ToolTypes;
ído_CurrentX;
ído_CurrentY;
ÉDrawerData*do_DrawerData;
Ñ*do_ToolWindow;
ído_StackSize;
};ÄWB_DISKMAGIC 0xe310ÄWB_DISKVERSION 1ÄWB_DISKREVISION 1ÄWB_DISKREVISIONMASK 255
ÉFreeList{
òfl_NumFree;
ÉÆfl_MemList;
};ÄGFLG_GADGBACKFILL 1ÄGADGBACKFILL 1ÄNO_ICON_POSITION (0x80000000)ÄWORKBENCH_NAME "workbench.library"ÄAM_VERSION 1
ÉAppMessage{
ÉØam_Message;
âam_Type;
óam_UserData;
óam_ID;
íam_NumArgs;
ÉWBArg*am_ArgList;
âam_Version;
âam_Class;
òam_MouseX;
òam_MouseY;
óam_Seconds;
óam_Micros;
óam_Reserved[8];
};
ÉAppWindow{ã*aw_PRIVATE;};
ÉAppIcon{ã*ai_PRIVATE;};
ÉAppMenuItem{ã*ami_PRIVATE;};á