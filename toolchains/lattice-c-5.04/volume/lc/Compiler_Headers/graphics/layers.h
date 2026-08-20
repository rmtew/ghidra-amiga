#ifndef GRAPHICS_LAYERS_H
#define GRAPHICS_LAYERS_H
/*
**	$Filename: graphics/layers.h $
**	$Release: 1.3 $
**
**	
**
**	(C) Copyright 1985,1986,1987,1988 Commodore-Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

#define LAYERSIMPLE	1
#define LAYERSMART	2
#define LAYERSUPER	4
#define LAYERUPDATING	0x10
#define LAYERBACKDROP	0x40
#define LAYERREFRESH	0x80
#define LAYER_CLIPRECTS_LOST	0x100	/* during BeginUpdate */
					/* or during layerop */
					/* this happens if out of memory */
#define LMN_REGION	-1

struct Layer_Info
{
    struct Layer *top_layer;
    struct Layer *check_lp;	    /* system use */
    struct Layer *obs;		    /* system use */
	struct MinList	FreeClipRects;
	struct SignalSemaphore Lock;
	struct List gs_Head;			/* system use */
	LONG	longreserved;
	UWORD	Flags;
	BYTE	fatten_count;
	BYTE	LockLayersCount;
    UWORD  LayerInfo_extra_size;
    WORD	*blitbuff;
    struct LayerInfo_extra *LayerInfo_extra;
};

#define NEWLAYERINFO_CALLED 1
#define ALERTLAYERSNOMEM 0x83010000

#endif	/* GRAPHICS_LAYERS_H */
