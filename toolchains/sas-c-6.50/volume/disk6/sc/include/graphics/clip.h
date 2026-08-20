ÄàGRAPHICS_CLIP_HÄGRAPHICS_CLIP_HàEXEC_TYPES_Hå<exec/types.h>áàGRAPHICS_GFX_Hå<graphics/gfx.h>áàEXEC_SEMAPHORES_Hå<exec/semaphores.h>áàUTILITY_HOOKS_Hå<utility/hooks.h>áÄNEWLOCKS
ÉLayer
{
ÉLayer*front,*back;
ÉClipRect*ClipRect;
ÉRastPort*rp;
ÉRectangle bounds;
äreserved[4];
âpriority;
â¶;
ÉBitMap*SuperBitMap;
ÉClipRect*SuperClipRect;
îWindow;
òScroll_X,Scroll_Y;
ÉClipRect*cr,*cr2,*crnew;
ÉClipRect*SuperSaveClipRects;
ÉClipRect*_cliprects;
ÉLayer_Info*LayerInfo;
ÉSignalSemaphore Lock;
ÉHook*BackFill;
óreserved1;
ÉRegion*ClipRegion;
ÉRegion*saveClipRects;
òWidth,Height;
äreserved2[18];
ÉRegion*DamageList;
};
ÉClipRect
{
ÉClipRect*Next;
ÉClipRect*prev;
ÉLayer*lobs;
ÉBitMap*BitMap;
ÉRectangle bounds;
ã*_p1;
ã*_p2;
íreserved;∞NEWCLIPRECTS_1_1
í¶;á
};ÄCR_NEEDS_NO_CONCEALED_RASTERS 1ÄCR_NEEDS_NO_LAYERBLIT_DAMAGE 2ÄISLESSX 1ÄISLESSY 2ÄISGRTRX 4ÄISGRTRY 8á