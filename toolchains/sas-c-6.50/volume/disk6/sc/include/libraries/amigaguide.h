€ˆLIBRARIES_AMIGAGUIDE_H€LIBRARIES_AMIGAGUIDE_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆ·Œ<exec/lists.h>‡ˆµŒ<exec/nodes.h>‡ˆEXEC_SEMAPHORES_HŒ<exec/semaphores.h>‡ˆINTUITION_INTUITION_HŒ<intuition/intuition.h>‡ˆINTUITION_SCREENS_HŒ<intuition/screens.h>‡ˆINTUITION_CLASSUSR_HŒ<intuition/classusr.h>‡ˆDO_DOS_HŒ<dos/dos.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆAPSH_TOOL_ID€APSH_TOOL_ID 11000L€StartupMsgID (APSH_TOOL_ID+1L)€LoginToolID (APSH_TOOL_ID+2L)€LogoutToolID (APSH_TOOL_ID+3L)€ShutdownMsgID (APSH_TOOL_ID+4L)€ActivateToolID (APSH_TOOL_ID+5L)€DeactivateToolID (APSH_TOOL_ID+6L)€ActiveToolID (APSH_TOOL_ID+7L)€InactiveToolID (APSH_TOOL_ID+8L)€ToolStatusID (APSH_TOOL_ID+9L)€ToolCmdID (APSH_TOOL_ID+10L)€ToolCmdReplyID (APSH_TOOL_ID+11L)€ShutdownToolID (APSH_TOOL_ID+12L)‡€AGA_Dummy (TAG_USER)€AGA_Path (AGA_Dummy+1)€AGA_XRefList (AGA_Dummy+2)€AGA_Activate (AGA_Dummy+3)€AGA_Context (AGA_Dummy+4)€AGA_HelpGroup (AGA_Dummy+5)€AGA_Reserved1 (AGA_Dummy+6)€AGA_Reserved2 (AGA_Dummy+7)€AGA_Reserved3 (AGA_Dummy+8)€AGA_ARexxPort (AGA_Dummy+9)€AGA_ARexxPortName (AGA_Dummy+10)
¥‹*AMIGAGUIDECONTEXT;
ƒAmigaGuideMsg
{
ƒ¯agm_Msg;
—agm_Type;
”agm_Data;
—agm_DSize;
—agm_DType;
—agm_Pri_Ret;
—agm_Sec_Ret;
”agm_System1;
”agm_System2;
};
ƒNewAmigaGuide
{
¡nag_Lock;
STRPTR nag_Name;
ƒScreen*nag_Screen;
STRPTR nag_PubScreen;
STRPTR nag_HostPort;
STRPTR nag_ClientPort;
STRPTR nag_BaseName;
—nag_Flags;
STRPTR*nag_Context;
STRPTR nag_Node;
’nag_Line;
ƒTagItem*nag_Extens;
VOID*nag_Client;
};€HTF_LOAD_INDEX (1L<<0)€HTF_LOAD_ALL (1L<<1)€HTF_CACHE_NODE (1L<<2)€HTF_CACHE_DB (1L<<3)€HTF_UNIQUE (1L<<15)€HTF_NOACTIVATE (1L<<16)€HTFC_SYSGADS 0x80000000€HTH_OPEN 0€HTH_CLOSE 1€HTERR_NOT_ENOUGH_MEMORY 100L€HTERR_CANT_OPEN_DATABASE 101L€HTERR_CANT_FIND_NODE 102L€HTERR_CANT_OPEN_NODE 103L€HTERR_CANT_OPEN_WINDOW 104L€HTERR_INVALID_COMMAND 105L€HTERR_CANT_COMPLETE 106L€HTERR_PORT_CLOSED 107L€HTERR_CANT_CREATE_PORT 108L€HTERR_KEYWORD_NOT_FOUND 113L
¥ƒAmigaGuideHost*AMIGAGUIDEHOST;
ƒXRef
{
ƒ¬xr_Node;
‰xr_Pad;
ƒDocFile*xr_DF;
STRPTR xr_File;
STRPTR xr_Name;
’xr_Line;
};€XRSIZE (sizeof(ƒXRef))€XR_GENERIC 0€XR_FUNCTION 1€XR_COMMAND 2€XR_INCLUDE 3€XR_MACRO 4€XR_STRUCT 5€XR_FIELD 6€XR_TYPEDEF 7€XR_DEFINE 8
ƒAmigaGuideHost
{
ƒHook agh_Dispatcher;
—agh_Reserved;
—agh_Flags;
—agh_UseCnt;
”agh_SystemData;
”agh_UserData;
};€HM_FINDNODE 1€HM_OPENNODE 2€HM_CLOSENODE 3€HM_EXPUNGE 10
ƒopFindHost
{
—MethodID;
ƒTagItem*ofh_Attrs;
STRPTR ofh_Node;
STRPTR ofh_TOC;
STRPTR ofh_Title;
STRPTR ofh_Next;
STRPTR ofh_Prev;
};
ƒopNodeIO
{
—MethodID;
ƒTagItem*onm_Attrs;
STRPTR onm_Node;
STRPTR onm_FileName;
STRPTR onm_DocBuffer;
—onm_BuffLen;
—onm_Flags;
};€HTNF_KEEP (1L<<0)€HTNF_RESERVED1 (1L<<1)€HTNF_RESERVED2 (1L<<2)€HTNF_ASCII (1L<<3)€HTNF_RESERVED3 (1L<<4)€HTNF_CLEAN (1L<<5)€HTNF_DONE (1L<<6)€HTNA_Dummy (TAG_USER)€HTNA_Screen (HTNA_Dummy+1)€HTNA_Pens (HTNA_Dummy+2)€HTNA_Rectangle (HTNA_Dummy+3)€HTNA_HelpGroup (HTNA_Dummy+5)
ƒopExpungeNode
{
—MethodID;
ƒTagItem*oen_Attrs;
};‡