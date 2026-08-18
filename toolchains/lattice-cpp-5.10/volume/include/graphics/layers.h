// graphics/layers.h
//
// (C) 1987, MTS Associates
//
#ifndef GRAPHICS_LAYERS_H
#define GRAPHICS_LAYERS_H
#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif
#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif
#ifndef GRAPHICS_CLIP_H
#include <graphics/clip.h>
#endif

#define LAYERSIMPLE     1
#define LAYERSMART      2
#define LAYERSUPER      4
#define LAYERUPDATING   0x10
#define LAYERBACKDROP   0x40
#define LAYERREFRESH    0x80
#define LAYER_CLIPRECTS_LOST    0x100
#define LMN_REGION      -1


struct LayerInfo_extra;

struct Layer_Info
{
    Layer	*top_layer;
    Layer	*check_lp;
    Layer	*obs;
    uMinList	FreeClipRects;
    SignalSemaphore Lock;
    uList	gs_Head;
    LONG	longreserved;
    UWORD	Flags;
    BYTE	fatten_count;
    BYTE	LockLayersCount;
    UWORD	Layer_Info_extra_size;
    WORD	*blitbuff;
    LayerInfo_extra *Layer_Info_extra;
};


void BeginUpdate (Layer *l);
void BehindLayer(Layer_Info *li, Layer *l);
Layer *CreateBehindLayer(Layer_Info *li, BitMap *bm,
	const int x0, const int y0, const int x1, const int y1,
	const int flags, BitMap *bitmap2=NULL);

Layer *CreateUpfrontLayer(Layer_Info *li, BitMap *bm,
	const int x0, const int y0, const int x1, const int y1,
	const int flags, BitMap *bitmap2=NULL);
void DeleteLayer (Layer_Info *li, Layer *l);
void DisposeLayer (Layer_Info *li);
void EndUpdate(Layer *l, const int layerflag);
void FattenLayer_Info (Layer_Info *li);
void InitLayers(Layer_Info *li);
void LockLayer(Layer_Info* li, Layer *l);
void LockLayer_Info(Layer_Info* li);
void LockLayers(Layer_Info* li);
void MoveLayer(Layer_Info* li, Layer *l, const int dx, const int dy);
BOOL MoveLayerInFrontOf (Layer *me, Layer *him);
void NewLayer_Info(Layer_Info *li);
void ScrollLayer(Layer_Info *li, Layer *l, const int dx, const int dy);
void SizeLayer(Layer_Info *li, Layer *l, const int dx, const int dy);
void SwapBitsRastPortClipRect(Layer_Info *li);
void ThinLayer_Info(Layer_Info *li);
void UnlockLayer(Layer *l);
void UnlockLayer_Info(Layer_Info *l);
void UnlockLayers(Layer_Info *l);
void UpfrontLayer(Layer_Info *li, Layer *l);
Layer *WhichLayer(Layer_Info *li, const int x, const int y);

BOOL AttemptLockLayerRom(Layer *l);
void CopySBitMap(Layer *l);
void LockLayerRom(Layer *l);
void SyncSBitMap(Layer *l);
void UnlockLayerRom(Layer *l);

#define NEWLAYERINFO_CALLED 1
#define ALERTLAYERSNOMEM 0x83010000

class pLayersBase {
	class Library *gl;
	short open_count;
public:
   pLayersBase(const int version)
   {
	if (open_count++ == 0)
	{
		gl = OpenLibrary("layer.library", version);
		if (gl == NULL)
		{
			Gripe("Unable to open layer library.");
			exit(20);
		}
	}
    }
   ~pLayersBase()
   {
	if (--open_count == 0) CloseLibrary( (Library *) gl);
   }
};

#endif
