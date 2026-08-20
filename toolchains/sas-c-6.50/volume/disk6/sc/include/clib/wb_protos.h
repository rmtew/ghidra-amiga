ÄàCLIB_WB_PROTOS_HÄCLIB_WB_PROTOS_HàEXEC_TYPES_Hå<exec/types.h>áàDOS_DOS_Hå<dos/dos.h>áàWORKBENCH_WORKBENCH_Hå<workbench/workbench.h>áàINTUITION_INTUITION_Hå<intuition/intuition.h>áàUTILITY_TAGITEM_Hå<utility/tagitem.h>á
ÉAppWindow*AddAppWindowA(éçid,éçuserdata,
ÉWindow*window,É©*msgport,
ÉTagItem*taglist);
ÉAppWindow*AddAppWindow(éçid,éçuserdata,
ÉWindow*window,É©*msgport,Tag tag1,...);
BOOL RemoveAppWindow(ÉAppWindow*appWindow);
ÉAppIcon*AddAppIconA(éçid,éçuserdata,
ä*text,É©*msgport,ÉFileLock*lock,
ÉDiskObject*diskobj,ÉTagItem*taglist);
ÉAppIcon*AddAppIcon(éçid,éçuserdata,
ä*text,É©*msgport,ÉFileLock*lock,
ÉDiskObject*diskobj,Tag tag1,...);
BOOL RemoveAppIcon(ÉAppIcon*appIcon);
ÉAppMenuItem*AddAppMenuItemA(éçid,éçuserdata,
ä*text,É©*msgport,ÉTagItem*taglist);
ÉAppMenuItem*AddAppMenuItem(éçid,éçuserdata,
ä*text,É©*msgport,Tag tag1,...);
BOOL RemoveAppMenuItem(ÉAppMenuItem*appMenuItem);
ãWBInfo(°lock,STRPTR name,ÉScreen*screen);á