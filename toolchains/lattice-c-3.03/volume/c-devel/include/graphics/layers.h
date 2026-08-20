



#ifndef GRAPHICS_LAYERS_H
#define GRAPHICS_LAYERS_H

#ifndef EXEC_PORTS_H
#include <exec/ports.h>
#endif

#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif

#define LAYERSIMPLE     1
#define LAYERSMART      2
#define LAYERSUPER      4
#define LAYERBACKDROP   0x40
#define LAYERREFRESH    0x80

struct Layer_Info
{
    struct Layer *top_layer;
    struct Layer *check_lp;         
    struct Layer *obs;              
    struct MsgPort  RP_ReplyPort;   
    struct MsgPort  LockPort;       
    UBYTE  Lock;
    UBYTE  broadcast;               
    UBYTE  LockNest;
    UBYTE  Flags;
    struct Task *Locker;
    BYTE   fatten_count;
    UBYTE  bytereserved;
    UWORD  wordreserved;  
    UWORD  LayerInfo_extra_size;
    ULONG  longreserved;
    struct LayerInfo_extra *LayerInfo_extra;
};

#define NEWLAYERINFO_CALLED 1
#define ALERTLAYERSNOMEM 0x83010000

#endif
