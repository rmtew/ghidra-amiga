€ˆLIBRARIES_NONVOLATILE_H€LIBRARIES_NONVOLATILE_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆµŒ<exec/nodes.h>‡
ƒNVInfo
{
—nvi_MaxStorage;
—nvi_FreeStorage;
};
ƒNVEntry
{
ƒMinNode nve_Node;
STRPTR nve_Name;
—nve_Size;
—nve_Protection;
};€NVEB_DELETE 0€NVEB_APPNAME 31€NVEF_DELETE (1<<NVEB_DELETE)€NVEF_APPNAME (1<<NVEB_APPNAME)€NVERR_BADNAME 1€NVERR_WRITEPROT 2€NVERR_FAIL 3€NVERR_FATAL 4€SizeNVData(DataPtr) ((((—*)DataPtr)[-1])-4)‡