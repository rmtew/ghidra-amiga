€ˆINTUITION_SCREENS_H€INTUITION_SCREENS_H TRUEˆEXEC_TYPES_HŒ<exec/types.h>‡ˆGRAPHICS_GFX_HŒ<graphics/gfx.h>‡ˆGRAPHICS_CLIP_HŒ<graphics/clip.h>‡ˆGRAPHICS_VIEW_HŒ<graphics/view.h>‡ˆGRAPHICS_RASTPORT_HŒ<graphics/rastport.h>‡ˆGRAPHICS_LAYERS_HŒ<graphics/layers.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡€DRI_VERSION (2)
ƒDrawInfo
{
‰dri_Version;
‰dri_NumPens;
‰*dri_Pens;
ƒTextFont*dri_Font;
‰dri_Depth;
ƒ{
‰X;
‰Y;
}dri_Resolution;
—dri_Flags;
ƒImage*dri_CheckMark;
ƒImage*dri_AmigaKey;
—dri_Reserved[5];
};€DRIF_NEWLOOK 1€DETAILPEN (0x0000)€BLOCKPEN (0x0001)€TEXTPEN (0x0002)€SHINEPEN (0x0003)€SHADOWPEN (0x0004)€FILLPEN (0x0005)€FILLTEXTPEN (0x0006)€BACKGROUNDPEN (0x0007)€HIGHLIGHTTEXTPEN (0x0008)€BARDETAILPEN (0x0009)€BARBLOCKPEN (0x000A)€BARTRIMPEN (0x000B)€NUMDRIPENS (0x000C)€PEN_C3 0xFEFC€PEN_C2 0xFEFD€PEN_C1 0xFEFE€PEN_C0 0xFEFF
ƒScreen
{
ƒScreen*NextScreen;
ƒWindow*FirstWindow;
˜LeftEdge,TopEdge;
˜Width,Height;
˜MouseY,MouseX;
‰¦;
Š*Title;
Š*DefaultTitle;
šBarHeight,BarVBorder,BarHBorder,MenuVBorder,MenuHBorder;
šWBorTop,WBorLeft,WBorRight,WBorBottom;
ƒTextAttr*Font;
ƒViewPort ViewPort;
ƒRastPort RastPort;
ƒBitMap BitMap;
ƒLayer_Info LayerInfo;
ƒ»*FirstGadget;
ŠDetailPen,BlockPen;
‰SaveColor0;
ƒLayer*BarLayer;
Š*ExtData;
Š*UserData;
};€SCREENTYPE 15€WBENCHSCREEN 1€PUBLICSCREEN 2€CUSTOMSCREEN 15€SHOWTITLE 16€BEEPING 32€CUSTOMBITMAP 64€SCREENBEHIND 128€SCREENQUIET 256€SCREENHIRES 512€NS_EXTENDED 0x1000€AUTOSCROLL 0x4000€PENSHARED 0x400€STDSCREENHEIGHT -1€STDSCREENWIDTH -1€SA_Dummy (TAG_USER+32)€SA_Left (SA_Dummy+1)€SA_Top (SA_Dummy+2)€SA_Width (SA_Dummy+3)€SA_Height (SA_Dummy+4)€SA_Depth (SA_Dummy+5)€SA_DetailPen (SA_Dummy+6)€SA_BlockPen (SA_Dummy+7)€SA_Title (SA_Dummy+8)€SA_Colors (SA_Dummy+9)€SA_ErrorCode (SA_Dummy+10)€SA_Font (SA_Dummy+11)€SA_SysFont (SA_Dummy+12)€SA_Type (SA_Dummy+13)€SA_BitMap (SA_Dummy+14)€SA_PubName (SA_Dummy+15)€SA_PubSig (SA_Dummy+16)€SA_PubTask (SA_Dummy+17)€SA_DisplayID (SA_Dummy+18)€SA_DClip (SA_Dummy+19)€SA_Overscan (SA_Dummy+20)€SA_Obsolete1 (SA_Dummy+21)€SA_ShowTitle (SA_Dummy+22)€SA_Behind (SA_Dummy+23)€SA_Quiet (SA_Dummy+24)€SA_AutoScroll (SA_Dummy+25)€SA_Pens (SA_Dummy+26)€SA_FullPalette (SA_Dummy+27)€SA_ColorMapEntries (SA_Dummy+28)€SA_Parent (SA_Dummy+29)€SA_Draggable (SA_Dummy+30)€SA_Exclusive (SA_Dummy+31)€SA_SharePens (SA_Dummy+32)€SA_BackFill (SA_Dummy+33)€SA_Interleaved (SA_Dummy+34)€SA_Colors32 (SA_Dummy+35)€SA_VideoControl (SA_Dummy+36)€SA_FrontChild (SA_Dummy+37)€SA_BackChild (SA_Dummy+38)€SA_LikeWorkbench (SA_Dummy+39)€SA_Reserved (SA_Dummy+40)€SA_MinimizeISG (SA_Dummy+41)ˆNSTAG_EXT_VPMODE€NSTAG_EXT_VPMODE (TAG_USER|1)‡€OSERR_NOMONITOR (1)€OSERR_NOCHIPS (2)€OSERR_NOMEM (3)€OSERR_NOCHIPMEM (4)€OSERR_PUBNOTUNIQUE (5)€OSERR_UNKNOWNMODE (6)€OSERR_TOODEEP (7)€OSERR_ATTACHFAIL (8)€OSERR_NOTAVAILABLE (9)
ƒNewScreen
{
˜LeftEdge,TopEdge,Width,Height,Depth;
ŠDetailPen,BlockPen;
‰ViewModes;
‰Type;
ƒTextAttr*Font;
Š*DefaultTitle;
ƒ»*Gadgets;
ƒBitMap*CustomBitMap;
};
ƒExtNewScreen
{
˜LeftEdge,TopEdge,Width,Height,Depth;
ŠDetailPen,BlockPen;
‰ViewModes;
‰Type;
ƒTextAttr*Font;
Š*DefaultTitle;
ƒ»*Gadgets;
ƒBitMap*CustomBitMap;
ƒTagItem*Extension;
};€OSCAN_TEXT (1)€OSCAN_STANDARD (2)€OSCAN_MAX (3)€OSCAN_VIDEO (4)
ƒPubScreenNode{
ƒ¬psn_Node;
ƒScreen*psn_Screen;
‰psn_Flags;
˜psn_Size;
˜psn_VisitorCount;
ƒTask*psn_SigTask;
Špsn_SigBit;
};€PSNF_PRIVATE (0x0001)€MAXPUBSCREENNAME (139)€SHANGHAI 1€POPPUBSCREEN 2€SDEPTH_TOFRONT (0)€SDEPTH_TOBACK (1)€SDEPTH_INFAMILY (2)€SDEPTH_CHILDONLY SDEPTH_INFAMILY€SPOS_RELATIVE (0)€SPOS_ABSOLUTE (1)€SPOS_MAKEVISIBLE (2)€SPOS_FORCEDRAG (4)
ƒScreenBuffer
{
ƒBitMap*sb_BitMap;
ƒDBufInfo*sb_DBufInfo;
};€SB_SCREEN_BITMAP 1€SB_COPY_BITMAP 2ˆINTUITION_IOBSOLETE_HŒ<intuition/iobsolete.h>‡‡