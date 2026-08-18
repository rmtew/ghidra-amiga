€ˆDATATYPES_PICTURECLASS_H€DATATYPES_PICTURECLASS_HˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆDATATYPES_DATATYPESCLASS_HŒ<datatypes/datatypesclass.h>‡ˆLIBRARIES_IFFPARSE_HŒ<libraries/iffparse.h>‡€PICTUREDTCLASS "picture.datatype"€PDTA_ModeID (DTA_Dummy+200)€PDTA_BitMapHeader (DTA_Dummy+201)€PDTA_BitMap (DTA_Dummy+202)€PDTA_ColorRegisters (DTA_Dummy+203)€PDTA_CRegs (DTA_Dummy+204)€PDTA_GRegs (DTA_Dummy+205)€PDTA_ColorTable (DTA_Dummy+206)€PDTA_ColorTable2 (DTA_Dummy+207)€PDTA_Allocated (DTA_Dummy+208)€PDTA_NumColors (DTA_Dummy+209)€PDTA_NumAlloc (DTA_Dummy+210)€PDTA_Remap (DTA_Dummy+211)€PDTA_Screen (DTA_Dummy+212)€PDTA_FreeSourceBitMap (DTA_Dummy+213)€PDTA_Grab (DTA_Dummy+214)€PDTA_DestBitMap (DTA_Dummy+215)€PDTA_ClassBitMap (DTA_Dummy+216)€PDTA_NumSparse (DTA_Dummy+217)€PDTA_SparseTable (DTA_Dummy+218)€mskNone 0€mskHasMask 1€mskHasTransparentColor 2€mskLasso 3€mskHasAlpha 4€cmpNone 0€cmpByteRun1 1€cmpByteRun2 2
ƒBitMapHeader
{
‰bmh_Width;
‰bmh_Height;
˜bmh_Left;
˜bmh_Top;
Šbmh_Depth;
Šbmh_Masking;
Šbmh_Compression;
Šbmh_Pad;
‰bmh_Transparent;
Šbmh_XAspect;
Šbmh_YAspect;
˜bmh_PageWidth;
˜bmh_PageHeight;
};
ƒColorRegister
{
Šred,green,blue;
};€ID_ILBM MAKE_ID('I','L','B','M')€ID_BMHD MAKE_ID('B','M','H','D')€ID_BODY MAKE_ID('B','O','D','Y')€ID_CMAP MAKE_ID('C','M','A','P')€ID_CRNG MAKE_ID('C','R','N','G')€ID_GRAB MAKE_ID('G','R','A','B')€ID_SPRT MAKE_ID('S','P','R','T')€ID_DEST MAKE_ID('D','E','S','T')€ID_CAMG MAKE_ID('C','A','M','G')‡