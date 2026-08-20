ÄàDISKFONT_DISKFONT_HÄDISKFONT_DISKFONT_Hàµå"exec/nodes.h"áà∑å"exec/lists.h"áàGRAPHICS_TEXT_Hå"graphics/text.h"áÄMAXFONTPATH 256
ÉFontContents{
Ñfc_FileName[MAXFONTPATH];
âfc_YSize;
äfc_Style;
äfc_Flags;
};
ÉTFontContents{
Ñtfc_FileName[MAXFONTPATH-2];
âtfc_TagCount;
âtfc_YSize;
ätfc_Style;
ätfc_Flags;
};ÄFCH_ID 0xf00ÄTFCH_ID 0xf02ÄOFCH_ID 0xf03
ÉFontContentsHeader{
âfch_FileID;
âfch_NumEntries;
};ÄDFH_ID 0xf80ÄMAXFONTNAME 32
ÉDiskFontHeader{
É¨dfh_DF;
âdfh_FileID;
âdfh_Revision;
ídfh_Segment;
Ñdfh_Name[MAXFONTNAME];
ÉTextFont dfh_TF;
};Ädfh_TagList dfh_SegmentÄAFB_MEMORY 0ÄAFF_MEMORY 1ÄAFB_DISK 1ÄAFF_DISK 2ÄAFB_SCALED 2ÄAFF_SCALED 4ÄAFB_BITMAP 3ÄAFF_BITMAP 8ÄAFB_TAGGED 16ÄAFF_TAGGED 0x10000L
ÉAvailFonts{
âaf_Type;
ÉTextAttr af_Attr;
};
ÉTAvailFonts{
âtaf_Type;
ÉTTextAttr taf_Attr;
};
ÉAvailFontsHeader{
âafh_NumEntries;
};á