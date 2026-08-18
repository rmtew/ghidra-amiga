ÄàCLIB_DISKFONT_PROTOS_HÄCLIB_DISKFONT_PROTOS_HàDOS_DOS_Hå<dos/dos.h>áàDISKFONT_DISKFONT_Hå<diskfont/diskfont.h>á
ÉTextFont*OpenDiskFont(ÉTextAttr*textAttr);
íAvailFonts(STRPTR buffer,çbufBytes,çflags);
ÉFontContentsHeader*NewFontContents(°fontsLock,STRPTR fontName);
ãDisposeFontContents(ÉFontContentsHeader*fontContentsHeader);
ÉDiskFont*NewScaledDiskFont(ÉTextFont*sourceFont,
ÉTextAttr*destTextAttr);á