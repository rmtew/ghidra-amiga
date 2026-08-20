ÄàDATATYPES_DATATYPES_HÄDATATYPES_DATATYPES_HàEXEC_TYPES_Hå<exec/types.h>áà∑å<exec/lists.h>áàµå<exec/nodes.h>áàEXEC_LIBRARIES_Hå<exec/libraries.h>áàLIBRARIES_IFFPARSE_Hå<libraries/iffparse.h>áàDOS_DOS_Hå<dos/dos.h>áÄID_DTYP MAKE_ID('D','T','Y','P')ÄID_DTHD MAKE_ID('D','T','H','D')
ÉDataTypeHeader
{
STRPTR dth_Name;
STRPTR dth_BaseName;
STRPTR dth_Pattern;
ò*dth_Mask;
ódth_GroupID;
ódth_ID;
òdth_MaskLen;
òdth_Pad;
âdth_Flags;
âdth_Priority;
};ÄDTHSIZE sizeof(ÉDataTypeHeader)ÄDTF_TYPE_MASK 15ÄDTF_BINARY 0ÄDTF_ASCII 1ÄDTF_IFF 2ÄDTF_MISC 3ÄDTF_CASE 16ÄDTF_SYSTEM1 0x1000ÄGID_SYSTEM MAKE_ID('s','y','s','t')ÄGID_TEXT MAKE_ID('t','e','x','t')ÄGID_DOCUMENT MAKE_ID('d','o','c','u')ÄGID_SOUND MAKE_ID('s','o','u','n')ÄGID_INSTRUMENT MAKE_ID('i','n','s','t')ÄGID_MUSIC MAKE_ID('m','u','s','i')ÄGID_PICTURE MAKE_ID('p','i','c','t')ÄGID_ANIMATION MAKE_ID('a','n','i','m')ÄGID_MOVIE MAKE_ID('m','o','v','i')ÄID_CODE MAKE_ID('D','T','C','D')
ÉDTHookContext
{
ÉLibrary*dthc_SysBase;
ÉLibrary*dthc_DOSBase;
ÉLibrary*dthc_IFFParseBase;
ÉLibrary*dthc_UtilityBase;
°dthc_Lock;
ÉFileInfoBlock*dthc_FIB;
°dthc_FileHandle;
ÉIFFHandle*dthc_IFF;
STRPTR dthc_Buffer;
ódthc_BufferLength;
};ÄID_TOOL MAKE_ID('D','T','T','L')
ÉTool
{
âtn_Which;
âtn_Flags;
STRPTR tn_Program;
};ÄTSIZE sizeof(ÉTool)ÄTW_INFO 1ÄTW_BROWSE 2ÄTW_EDIT 3ÄTW_PRINT 4ÄTW_MAIL 5ÄTF_LAUNCH_MASK 15ÄTF_SHELL 1ÄTF_WORKBENCH 2ÄTF_RX 3ÄID_TAGS MAKE_ID('D','T','T','G')àDATATYPEÄDATATYPE
ÉDataType
{
É¨dtn_Node1;
É¨dtn_Node2;
ÉDataTypeHeader*dtn_Header;
ÉÆdtn_ToolList;
STRPTR dtn_FunctionName;
ÉTagItem*dtn_AttrList;
ódtn_Length;
};áÄDTNSIZE sizeof(ÉDataType)
ÉToolNode
{
É¨tn_Node;
ÉTool tn_Tool;
ótn_Length;
};ÄTNSIZE sizeof(ÉToolNode)àID_NAMEÄID_NAME MAKE_ID('N','A','M','E')áÄDTERROR_UNKNOWN_DATATYPE 2000ÄDTERROR_COULDNT_SAVE 2001ÄDTERROR_COULDNT_OPEN 2002ÄDTERROR_COULDNT_SEND_MESSAGE 2003ÄDTERROR_COULDNT_OPEN_CLIPBOARD 2004ÄDTERROR_Reserved 2005ÄDTERROR_UNKNOWN_COMPRESSION 2006ÄDTERROR_NOT_ENOUGH_DATA 2007ÄDTERROR_INVALID_DATA 2008ÄDTMSG_TYPE_OFFSET 2100á