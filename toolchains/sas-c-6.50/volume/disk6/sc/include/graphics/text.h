€ˆGRAPHICS_TEXT_H€GRAPHICS_TEXT_Hˆ¸Œ"exec/ports.h"‡ˆGRAPHICS_GFX_HŒ"graphics/gfx.h"‡ˆUTILITY_TAGITEM_HŒ"utility/tagitem.h"‡€FS_NORMAL 0€FSB_UNDERLINED 0€FSF_UNDERLINED 1€FSB_BOLD 1€FSF_BOLD 2€FSB_ITALIC 2€FSF_ITALIC 4€FSB_EXTENDED 3€FSF_EXTENDED 8€FSB_COLORFONT 6€FSF_COLORFONT 64€FSB_TAGGED 7€FSF_TAGGED 128€FPB_ROMFONT 0€FPF_ROMFONT 1€FPB_DISKFONT 1€FPF_DISKFONT 2€FPB_REVPATH 2€FPF_REVPATH 4€FPB_TALLDOT 3€FPF_TALLDOT 8€FPB_WIDEDOT 4€FPF_WIDEDOT 16€FPB_PROPORTIONAL 5€FPF_PROPORTIONAL 32€FPB_DESIGNED 6€FPF_DESIGNED 64€FPB_REMOVED 7€FPF_REMOVED (1<<7)
ƒTextAttr{
STRPTR ta_Name;
‰ta_YSize;
Šta_Style;
Šta_Flags;
};
ƒTTextAttr{
STRPTR tta_Name;
‰tta_YSize;
Štta_Style;
Štta_Flags;
ƒTagItem*tta_Tags;
};€TA_DeviceDPI (1|TAG_USER)€MAXFONTMATCHWEIGHT 32767
ƒTextFont{
ƒ¯tf_Message;
‰tf_YSize;
Štf_Style;
Štf_Flags;
‰tf_XSize;
‰tf_Baseline;
‰tf_BoldSmear;
‰tf_Accessors;
Štf_LoChar;
Štf_HiChar;
”tf_CharData;
‰tf_Modulo;
”tf_CharLoc;
”tf_CharSpace;
”tf_CharKern;
};€tf_Extension tf_Message.mn_ReplyPort€TE0B_NOREMFONT 0€TE0F_NOREMFONT 1
ƒTextFontExtension{
‰tfe_MatchWord;
Štfe_Flags0;
Štfe_Flags1;
ƒTextFont*tfe_BackPtr;
ƒ©*tfe_OrigReplyPort;
ƒTagItem*tfe_Tags;
‰*tfe_OFontPatchS;
‰*tfe_OFontPatchK;
};€CT_COLORMASK 15€CT_COLORFONT 1€CT_GREYFONT 2€CT_ANTIALIAS 4€CTB_MAPCOLOR 0€CTF_MAPCOLOR 1
ƒColorFontColors{
‰cfc_Reserved;
‰cfc_Count;
‰*cfc_ColorTable;
};
ƒColorTextFont{
ƒTextFont ctf_TF;
‰ctf_Flags;
Šctf_Depth;
Šctf_FgColor;
Šctf_Low;
Šctf_High;
Šctf_PlanePick;
Šctf_PlaneOnOff;
ƒColorFontColors*ctf_ColorFontColors;
”ctf_CharData[8];
};
ƒTextExtent{
‰te_Width;
‰te_Height;
ƒRectangle te_Extent;
};‡