€ˆINTUITION_GADGETCLASS_H€INTUITION_GADGETCLASS_H 1ˆEXEC_TYPES_HŒ<exec/types.h>‡ˆINTUITION_INTUITION_HŒ<intuition/intuition.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡€GA_Dummy (TAG_USER+0x30000)€GA_Left (GA_Dummy+1)€GA_RelRight (GA_Dummy+2)€GA_Top (GA_Dummy+3)€GA_RelBottom (GA_Dummy+4)€GA_Width (GA_Dummy+5)€GA_RelWidth (GA_Dummy+6)€GA_Height (GA_Dummy+7)€GA_RelHeight (GA_Dummy+8)€GA_Text (GA_Dummy+9)€GA_Image (GA_Dummy+10)€GA_Border (GA_Dummy+11)€GA_SelectRender (GA_Dummy+12)€GA_Highlight (GA_Dummy+13)€GA_Disabled (GA_Dummy+14)€GA_GZZGadget (GA_Dummy+15)€GA_ID (GA_Dummy+16)€GA_UserData (GA_Dummy+17)€GA_SpecialInfo (GA_Dummy+18)€GA_Selected (GA_Dummy+19)€GA_EndGadget (GA_Dummy+20)€GA_Immediate (GA_Dummy+21)€GA_RelVerify (GA_Dummy+22)€GA_FollowMouse (GA_Dummy+23)€GA_RightBorder (GA_Dummy+24)€GA_LeftBorder (GA_Dummy+25)€GA_TopBorder (GA_Dummy+26)€GA_BottomBorder (GA_Dummy+27)€GA_ToggleSelect (GA_Dummy+28)€GA_SysGadget (GA_Dummy+29)€GA_SysGType (GA_Dummy+30)€GA_Previous (GA_Dummy+31)€GA_Next (GA_Dummy+32)€GA_DrawInfo (GA_Dummy+33)€GA_IntuiText (GA_Dummy+34)€GA_LabelImage (GA_Dummy+35)€GA_TabCycle (GA_Dummy+36)€GA_GadgetHelp (GA_Dummy+37)€GA_Bounds (GA_Dummy+38)€GA_RelSpecial (GA_Dummy+39)€PGA_Dummy (TAG_USER+0x31000)€PGA_Freedom (PGA_Dummy+1)€PGA_Borderless (PGA_Dummy+2)€PGA_HorizPot (PGA_Dummy+3)€PGA_HorizBody (PGA_Dummy+4)€PGA_VertPot (PGA_Dummy+5)€PGA_VertBody (PGA_Dummy+6)€PGA_Total (PGA_Dummy+7)€PGA_Visible (PGA_Dummy+8)€PGA_Top (PGA_Dummy+9)€PGA_NewLook (PGA_Dummy+10)€STRINGA_Dummy (TAG_USER+0x32000)€STRINGA_MaxChars (STRINGA_Dummy+1)€STRINGA_Buffer (STRINGA_Dummy+2)€STRINGA_UndoBuffer (STRINGA_Dummy+3)€STRINGA_WorkBuffer (STRINGA_Dummy+4)€STRINGA_BufferPos (STRINGA_Dummy+5)€STRINGA_DispPos (STRINGA_Dummy+6)€STRINGA_AltKeyMap (STRINGA_Dummy+7)€STRINGA_Font (STRINGA_Dummy+8)€STRINGA_Pens (STRINGA_Dummy+9)€STRINGA_ActivePens (STRINGA_Dummy+10)€STRINGA_EditHook (STRINGA_Dummy+11)€STRINGA_EditModes (STRINGA_Dummy+12)€STRINGA_ReplaceMode (STRINGA_Dummy+13)€STRINGA_FixedFieldMode (STRINGA_Dummy+14)€STRINGA_NoFilterMode (STRINGA_Dummy+15)€STRINGA_Justification (STRINGA_Dummy+16)€STRINGA_LongVal (STRINGA_Dummy+17)€STRINGA_TextVal (STRINGA_Dummy+18)€STRINGA_ExitHelp (STRINGA_Dummy+19)€SG_DEFAULTMAXCHARS (128)€LAYOUTA_Dummy (TAG_USER+0x38000)€LAYOUTA_LayoutObj (LAYOUTA_Dummy+1)€LAYOUTA_Spacing (LAYOUTA_Dummy+2)€LAYOUTA_Orientation (LAYOUTA_Dummy+3)€LORIENT_NONE 0€LORIENT_HORIZ 1€LORIENT_VERT 2€GM_Dummy (-1)€GM_HITTEST (0)€GM_RENDER (1)€GM_GOACTIVE (2)€GM_HANDLEINPUT (3)€GM_GOINACTIVE (4)€GM_HELPTEST (5)€GM_LAYOUT (6)
ƒgpHitTest
{
—MethodID;
ƒGadgetInfo*gpht_GInfo;
ƒ
{
˜X;
˜Y;
}gpht_Mouse;
};€GMR_GADGETHIT (0x00000004)€GMR_NOHELPHIT (0x00000000)€GMR_HELPHIT (0xFFFFFFFF)€GMR_HELPCODE (0x00010000)
ƒgpRender
{
—MethodID;
ƒGadgetInfo*gpr_GInfo;
ƒRastPort*gpr_RPort;
’gpr_Redraw;
};€GREDRAW_UPDATE (2)€GREDRAW_REDRAW (1)€GREDRAW_TOGGLE (0)
ƒgpInput
{
—MethodID;
ƒGadgetInfo*gpi_GInfo;
ƒInputEvent*gpi_IEvent;
’*gpi_Termination;
ƒ
{
˜X;
˜Y;
}gpi_Mouse;
ƒTabletData*gpi_TabletData;
};€GMR_MEACTIVE (0)€GMR_NOREUSE (1<<1)€GMR_REUSE (1<<2)€GMR_VERIFY (1<<3)€GMR_NEXTACTIVE (1<<4)€GMR_PREVACTIVE (1<<5)
ƒgpGoInactive
{
—MethodID;
ƒGadgetInfo*gpgi_GInfo;
—gpgi_Abort;
};
ƒgpLayout
{
—MethodID;
ƒGadgetInfo*gpl_GInfo;
—gpl_Initial;
};ˆINTUITION_IOBSOLETE_HŒ<intuition/iobsolete.h>‡‡