€ˆGRAPHICS_DISPLAYINFO_H€GRAPHICS_DISPLAYINFO_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆGRAPHICS_GFX_HŒ<graphics/gfx.h>‡ˆGRAPHICS_MONITOR_HŒ<graphics/monitor.h>‡ˆGRAPHICS_MODEID_HŒ<graphics/modeid.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡
¥”DisplayInfoHandle;€DTAG_DISP 0x80000000€DTAG_DIMS 0x80001000€DTAG_MNTR 0x80002000€DTAG_NAME 0x80003000€DTAG_VEC 0x80004000
ƒQueryHeader
{
—StructID;
—DisplayID;
—SkipID;
—Length;
};
ƒDisplayInfo
{
ƒQueryHeader Header;
‰NotAvailable;
—PropertyFlags;
Point Resolution;
‰PixelSpeed;
‰NumStdSprites;
‰PaletteRange;
Point SpriteResolution;
Špad[4];
ŠRedBits;
ŠGreenBits;
ŠBlueBits;
Špad2[5];
—reserved[2];
};€DI_AVAIL_NOCHIPS 1€DI_AVAIL_NOMONITOR 2€DI_AVAIL_NOTWITHGENLOCK 4€DIPF_IS_LACE 1€DIPF_IS_DUALPF 2€DIPF_IS_PF2PRI 4€DIPF_IS_HAM 8€DIPF_IS_ECS 16€DIPF_IS_AA 0x10000€DIPF_IS_PAL 32€DIPF_IS_SPRITES 64€DIPF_IS_GENLOCK 128€DIPF_IS_WB 256€DIPF_IS_DRAGGABLE 512€DIPF_IS_PANELLED 0x400€DIPF_IS_BEAMSYNC 0x800€DIPF_IS_EXTRAHALFBRITE 0x1000€DIPF_IS_SPRITES_ATT 0x2000€DIPF_IS_SPRITES_CHNG_RES 0x4000€DIPF_IS_SPRITES_BORDER 0x8000€DIPF_IS_SCANDBL 0x20000€DIPF_IS_SPRITES_CHNG_BASE 0x40000€DIPF_IS_SPRITES_CHNG_PRI 0x80000€DIPF_IS_DBUFFER 0x100000€DIPF_IS_PROGBEAM 0x200000€DIPF_IS_FOREIGN 0x80000000
ƒDimensionInfo
{
ƒQueryHeader Header;
‰MaxDepth;
‰MinRasterWidth;
‰MinRasterHeight;
‰MaxRasterWidth;
‰MaxRasterHeight;
ƒRectangle Nominal;
ƒRectangle MaxOScan;
ƒRectangle VideoOScan;
ƒRectangle TxtOScan;
ƒRectangle StdOScan;
Špad[14];
—reserved[2];
};
ƒMonitorInfo
{
ƒQueryHeader Header;
ƒMonitorSpec*Mspc;
Point ViewPosition;
Point ViewResolution;
ƒRectangle ViewPositionRange;
‰TotalRows;
‰TotalColorClocks;
‰MinRow;
˜Compatibility;
Špad[32];
Point MouseTicks;
Point DefaultViewPosition;
—PreferredModeID;
—reserved[2];
};€MCOMPAT_MIXED 0€MCOMPAT_SELF 1€MCOMPAT_NOBODY -1€DISPLAYNAMELEN 32
ƒNameInfo
{
ƒQueryHeader Header;
ŠName[DISPLAYNAMELEN];
—reserved[2];
};
ƒVecInfo
{
ƒQueryHeader Header;
”Vec;
”Data;
‰Type;
‰pad[3];
—reserved[2];
};‡