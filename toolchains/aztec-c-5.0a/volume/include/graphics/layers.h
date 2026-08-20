#ifndef	GRAPHICS_LAYERS_H
#define	GRAPHICS_LAYERS_H
#ifndef	EXEC_LISTS_H
#include	<exec/lists.h>
#endif
#ifndef	EXEC_SEMAPHORES_H
#include	<exec/semaphores.h>
#endif
#define	LAYERSIMPLE	1
#define	LAYERSMART	2
#define	LAYERSUPER	4
#define	LAYERUPDATING	0x10
#define	LAYERBACKDROP	0x40
#define	LAYERREFRESH	0x80
#define	LAYER_CLIPRECTS_LOST	0x100
#define	LMN_REGION	-1
struct	Layer_Info
{
struct	Layer	*top_layer;
struct	Layer	*check_lp;
struct	Layer	*obs;
struct	MinList	FreeClipRects;
struct	SignalSemaphore	Lock;
struct	List	gs_Head;
LONG	longreserved;
UWORD	Flags;
BYTE	fatten_count;
BYTE	LockLayersCount;
UWORD	LayerInfo_extra_size;
WORD	*blitbuff;
struct	LayerInfo_extra	*LayerInfo_extra;
};
#define	NEWLAYERINFO_CALLED	1
#define	ALERTLAYERSNOMEM	0x83010000
#endif
