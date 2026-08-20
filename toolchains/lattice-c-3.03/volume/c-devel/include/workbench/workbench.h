


#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif !EXEC_TYPES_H

#ifndef  EXEC_NODES_H
#include "exec/nodes.h"
#endif !EXEC_NODES_H

#ifndef  EXEC_LISTS_H
#include "exec/lists.h"
#endif !EXEC_LISTS_H

#ifndef EXEC_TASKS_H
#include "exec/tasks.h"
#endif !EXEC_TASKS_H

#ifndef INTUITION_INTUITION_H
#include "intuition/intuition.h"
#endif !INTUITION_INTUITION_H

#define  WBDISK      1
#define  WBDRAWER    2
#define  WBTOOL      3
#define  WBPROJECT   4
#define  WBGARBAGE   5
#define  WBDEVICE    6
#define  WBKICK      7

struct DrawerData {
    struct NewWindow dd_NewWindow;     
    LONG                dd_CurrentX;   
    LONG                dd_CurrentY;   
    LONG                dd_MinX;       
    LONG                dd_MinY;       
    LONG                dd_MaxX;       
    LONG                dd_MaxY;       
    struct Gadget       dd_HorizScroll;
    struct Gadget       dd_VertScroll;
    struct Gadget       dd_UpMove;
    struct Gadget       dd_DownMove;
    struct Gadget       dd_LeftMove;
    struct Gadget       dd_RightMove;
    struct Image        dd_HorizImage;
    struct Image        dd_VertImage;
    struct PropInfo     dd_HorizProp;
    struct PropInfo     dd_VertProp;
    struct Window *     dd_DrawerWin;  
    struct WBObject *   dd_Object;     
    struct List         dd_Children;   
    LONG                dd_Lock;
};


#define DRAWERDATAFILESIZE (sizeof( struct NewWindow ) + 2*sizeof(LONG))


struct DiskObject {
    UWORD               do_Magic; 
    UWORD               do_Version; 
    struct Gadget       do_Gadget;  
    UBYTE               do_Type;
    char *              do_DefaultTool;
    char **             do_ToolTypes;
    LONG                do_CurrentX;
    LONG                do_CurrentY;
    struct DrawerData * do_DrawerData;
    char *              do_ToolWindow;    
    LONG                do_StackSize;     

};

#define WB_DISKMAGIC 0xe310      
#define WB_DISKVERSION  1        

struct FreeList {
    WORD                fl_NumFree;
    struct List         fl_MemList;
};

struct WBObject {
    struct Node         wo_MasterNode; 
    struct Node         wo_Siblings;   
    struct Node         wo_SelectNode; 
    struct Node         wo_UtilityNode;   
    struct WBObject *   wo_Parent;

    
#ifdef SMARTCOMPILER
    UBYTE               wo_IconDisp:1;    
    UBYTE               wo_DrawerOpen:1;  
    UBYTE               wo_Selected:1;    
    UBYTE               wo_Background:1;  
#else
    
    UBYTE               wo_Flags;
#endif

    UBYTE               wo_Type;          
    USHORT              wo_UseCount;      
    char *              wo_Name;          
    SHORT               wo_NameXOffset;   
    SHORT               wo_NameYOffset;

    char *              wo_DefaultTool;
    struct DrawerData * wo_DrawerData;    
    struct Window *     wo_IconWin;       
    LONG                wo_CurrentX;      
    LONG                wo_CurrentY;      
    char **             wo_ToolTypes;     
    struct Gadget       wo_Gadget;        
    struct FreeList     wo_FreeList;      
    char *              wo_ToolWindow;    
    LONG                wo_StackSize;     
    LONG                wo_Lock;          

};

#define TMAlloc( size, type ) ((type)MAlloc( size ))
#define ObjAlloc( obj, size, type )   ((type)OAlloc( obj, size ))
#define STREQ( a, b )      (!strcmp( a, b ))



#define MTYPE_PSTD         1  
#define MTYPE_TOOLEXIT     2  
#define MTYPE_DISKCHANGE   3  
#define MTYPE_TIMER        4  
#define MTYPE_CLOSEDOWN    5  
#define MTYPE_IOPROC       6  


#define GID_WBOBJECT       0  
#define GID_HORIZSCROLL    1  
#define GID_VERTSCROLL     2  
#define GID_LEFTSCROLL     3  
#define GID_RIGHTSCROLL    4  
#define GID_UPSCROLL       5  
#define GID_DOWNSCROLL     6  
#define GID_NAME           7  



#define GADGBACKFILL       0x0001


#define NO_ICON_POSITION   (0x80000000)
