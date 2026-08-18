€ˆGRAPHICS_LAYERS_H€GRAPHICS_LAYERS_Hˆ·Œ<exec/lists.h>‡ˆEXEC_SEMAPHORES_HŒ<exec/semaphores.h>‡€LAYERSIMPLE 1€LAYERSMART 2€LAYERSUPER 4€LAYERUPDATING 16€LAYERBACKDROP 64€LAYERREFRESH 128€LAYERIREFRESH 512€LAYERIREFRESH2 0x400€LAYER_CLIPRECTS_LOST 256
ƒLayer_Info
{
ƒLayer*top_layer;
ƒLayer*check_lp;
ƒClipRect*obs;
ƒClipRect*FreeClipRects;
’PrivateReserve1;
’PrivateReserve2;
ƒSignalSemaphore Lock;
ƒMinList gs_Head;
˜PrivateReserve3;
VOID*PrivateReserve4;
‰¦;
šfatten_count;
šLockLayersCount;
˜PrivateReserve5;
VOID*BlankHook;
VOID*LayerInfo_extra;
};€NEWLAYERINFO_CALLED 1€LAYERS_NOBACKFILL ((ƒHook*)1)€LAYERS_BACKFILL ((ƒHook*)0)‡