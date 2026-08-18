€ˆDATATYPES_DATATYPESCLASS_H€DATATYPES_DATATYPESCLASS_HˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆDATATYPES_DATATYPES_HŒ<datatypes/datatypes.h>‡ˆINTUITION_INTUITION_HŒ<intuition/intuition.h>‡ˆDEVICES_PRINTER_HŒ<devices/printer.h>‡ˆDEVICES_PRTBASE_HŒ<devices/prtbase.h>‡€DATATYPESCLASS "datatypesclass"€DTA_Dummy (TAG_USER+0x1000)€DTA_TextAttr (DTA_Dummy+10)€DTA_TopVert (DTA_Dummy+11)€DTA_VisibleVert (DTA_Dummy+12)€DTA_TotalVert (DTA_Dummy+13)€DTA_VertUnit (DTA_Dummy+14)€DTA_TopHoriz (DTA_Dummy+15)€DTA_VisibleHoriz (DTA_Dummy+16)€DTA_TotalHoriz (DTA_Dummy+17)€DTA_HorizUnit (DTA_Dummy+18)€DTA_NodeName (DTA_Dummy+19)€DTA_Title (DTA_Dummy+20)€DTA_TriggerMethods (DTA_Dummy+21)€DTA_Data (DTA_Dummy+22)€DTA_TextFont (DTA_Dummy+23)€DTA_Methods (DTA_Dummy+24)€DTA_PrinterStatus (DTA_Dummy+25)€DTA_PrinterProc (DTA_Dummy+26)€DTA_LayoutProc (DTA_Dummy+27)€DTA_Busy (DTA_Dummy+28)€DTA_Sync (DTA_Dummy+29)€DTA_BaseName (DTA_Dummy+30)€DTA_GroupID (DTA_Dummy+31)€DTA_ErrorLevel (DTA_Dummy+32)€DTA_ErrorNumber (DTA_Dummy+33)€DTA_ErrorString (DTA_Dummy+34)€DTA_Conductor (DTA_Dummy+35)€DTA_ControlPanel (DTA_Dummy+36)€DTA_Immediate (DTA_Dummy+37)€DTA_Repeat (DTA_Dummy+38)€DTA_Name (DTA_Dummy+100)€DTA_SourceType (DTA_Dummy+101)€DTA_Handle (DTA_Dummy+102)€DTA_DataType (DTA_Dummy+103)€DTA_Domain (DTA_Dummy+104)€DTA_Left (DTA_Dummy+105)€DTA_Top (DTA_Dummy+106)€DTA_Width (DTA_Dummy+107)€DTA_Height (DTA_Dummy+108)€DTA_ObjName (DTA_Dummy+109)€DTA_ObjAuthor (DTA_Dummy+110)€DTA_ObjAnnotation (DTA_Dummy+111)€DTA_ObjCopyright (DTA_Dummy+112)€DTA_ObjVersion (DTA_Dummy+113)€DTA_ObjectID (DTA_Dummy+114)€DTA_UserData (DTA_Dummy+115)€DTA_FrameInfo (DTA_Dummy+116)€DTA_RelRight (DTA_Dummy+117)€DTA_RelBottom (DTA_Dummy+118)€DTA_RelWidth (DTA_Dummy+119)€DTA_RelHeight (DTA_Dummy+120)€DTA_SelectDomain (DTA_Dummy+121)€DTA_TotalPVert (DTA_Dummy+122)€DTA_TotalPHoriz (DTA_Dummy+123)€DTA_NominalVert (DTA_Dummy+124)€DTA_NominalHoriz (DTA_Dummy+125)€DTA_DestCols (DTA_Dummy+400)€DTA_DestRows (DTA_Dummy+401)€DTA_Special (DTA_Dummy+402)€DTA_RastPort (DTA_Dummy+403)€DTST_RAM 1€DTST_FILE 2€DTST_CLIPBOARD 3€DTST_HOTLINK 4
ƒDTSpecialInfo
{
ƒSignalSemaphore si_Lock;
—si_Flags;
’si_TopVert;
’si_VisVert;
’si_TotVert;
’si_OTopVert;
’si_VertUnit;
’si_TopHoriz;
’si_VisHoriz;
’si_TotHoriz;
’si_OTopHoriz;
’si_HorizUnit;
};€DTSIF_LAYOUT (1L<<0)€DTSIF_NEWSIZE (1L<<1)€DTSIF_DRAGGING (1L<<2)€DTSIF_DRAGSELECT (1L<<3)€DTSIF_HIGHLIGHT (1L<<4)€DTSIF_PRINTING (1L<<5)€DTSIF_LAYOUTPROC (1L<<6)
ƒDTMethod
{
STRPTR dtm_Label;
STRPTR dtm_Command;
—dtm_Method;
};€DTM_Dummy (0x600)€DTM_FRAMEBOX (0x601)€DTM_PROCLAYOUT (0x602)€DTM_ASYNCLAYOUT (0x603)€DTM_REMOVEDTOBJECT (0x604)€DTM_SELECT (0x605)€DTM_CLEARSELECTED (0x606)€DTM_COPY (0x607)€DTM_PRINT (0x608)€DTM_ABORTPRINT (0x609)€DTM_NEWMEMBER (0x610)€DTM_DISPOSEMEMBER (0x611)€DTM_GOTO (0x630)€DTM_TRIGGER (0x631)€DTM_OBTAINDRAWINFO (0x640)€DTM_DRAW (0x641)€DTM_RELEASEDRAWINFO (0x642)€DTM_WRITE (0x650)
ƒFrameInfo
{
—fri_PropertyFlags;
Point fri_Resolution;
Šfri_RedBits;
Šfri_GreenBits;
Šfri_BlueBits;
ƒ
{
—Width;
—Height;
—Depth;
}fri_Dimensions;
ƒScreen*fri_Screen;
ƒColorMap*fri_ColorMap;
—fri_Flags;
};€FIF_SCALABLE 1€FIF_SCROLLABLE 2€FIF_REMAPPABLE 4
ƒdtGeneral
{
—MethodID;
ƒGadgetInfo*dtg_GInfo;
};
ƒdtSelect
{
—MethodID;
ƒGadgetInfo*dts_GInfo;
ƒRectangle dts_Select;
};
ƒdtFrameBox
{
—MethodID;
ƒGadgetInfo*dtf_GInfo;
ƒFrameInfo*dtf_ContentsInfo;
ƒFrameInfo*dtf_FrameInfo;
—dtf_SizeFrameInfo;
—dtf_FrameFlags;
};ˆFRAMEF_SPECIFY€FRAMEF_SPECIFY (1<<0)‡
ƒdtGoto
{
—MethodID;
ƒGadgetInfo*dtg_GInfo;
STRPTR dtg_NodeName;
ƒTagItem*dtg_AttrList;
};
ƒdtTrigger
{
—MethodID;
ƒGadgetInfo*dtt_GInfo;
—dtt_Function;
”dtt_Data;
};€STM_PAUSE 1€STM_PLAY 2€STM_CONTENTS 3€STM_INDEX 4€STM_RETRACE 5€STM_BROWSE_PREV 6€STM_BROWSE_NEXT 7€STM_NEXT_FIELD 8€STM_PREV_FIELD 9€STM_ACTIVATE_FIELD 10€STM_COMMAND 11€STM_REWIND 12€STM_FASTFORWARD 13€STM_STOP 14€STM_RESUME 15€STM_LOCATE 16
«printerIO
{
ƒIOStdReq ios;
ƒIODRPReq iodrp;
ƒIOPrtCmdReq iopc;
};
ƒdtPrint
{
—MethodID;
ƒGadgetInfo*dtp_GInfo;
«printerIO*dtp_PIO;
ƒTagItem*dtp_AttrList;
};
ƒdtDraw
{
—MethodID;
ƒRastPort*dtd_RPort;
’dtd_Left;
’dtd_Top;
’dtd_Width;
’dtd_Height;
’dtd_TopHoriz;
’dtd_TopVert;
ƒTagItem*dtd_AttrList;
};
ƒdtWrite
{
—MethodID;
ƒGadgetInfo*dtw_GInfo;
¡dtw_FileHandle;
—dtw_Mode;
ƒTagItem*dtw_AttrList;
};€DTWM_IFF 0€DTWM_RAW 1‡