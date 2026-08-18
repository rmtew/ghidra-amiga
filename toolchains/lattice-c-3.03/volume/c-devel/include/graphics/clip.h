#ifndef GRAPHICS_CLIP_H
#define GRAPHICS_CLIP_H

#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif
#ifndef EXEC_PORTS_H
#include <exec/ports.h>
#endif










struct Layer
{
    struct  Layer *front,*back;     
    struct  ClipRect    *ClipRect;  
    struct  RastPort    *rp;        
    struct  Rectangle   bounds;     
    UBYTE   Lock;                   
    UBYTE   LockCount;              
    UBYTE   LayerLockCount;         
    UBYTE   reserved;
    UWORD   reserved1;
    UWORD   Flags;                  
    struct  BitMap *SuperBitMap;
    struct  ClipRect *SuperClipRect; 
                                  
    APTR    Window;                 
    SHORT   Scroll_X,Scroll_Y;
    struct  MsgPort LockPort;
    struct  Message LockMessage;
    struct  MsgPort ReplyPort;
    struct  Message l_LockMessage;
    struct  Region  *DamageList;    
   struct   ClipRect *_cliprects;   
   struct   Layer_Info  *LayerInfo; 
   struct   Task  *LayerLocker;     
   struct   ClipRect *SuperSaveClipRects; 
   struct   ClipRect *cr,*cr2,*crnew;  
   APTR  _p1;                       
};

struct ClipRect
{
    struct  ClipRect *Next;         
    struct  ClipRect *prev;         
    struct  Layer   *lobs;          
    struct  BitMap  *BitMap;
    struct  Rectangle   bounds;     
    struct  ClipRect *_p1,*_p2;     
    LONG    reserved;               
#ifdef NEWCLIPRECTS_1_1
    LONG    Flags;                  
#endif
};


#define CR_NEEDS_NO_CONCEALED_RASTERS  1


#define ISLESSX 1
#define ISLESSY 2
#define ISGRTRX 4
#define ISGRTRY 8
#endif
