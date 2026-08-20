#ifndef     DEVICES_PRINTER_H
#define     DEVICES_PRINTER_H






#ifndef  EXEC_NODES_H
#include "exec/nodes.h"
#endif

#ifndef  EXEC_LISTS_H
#include "exec/lists.h"
#endif

#ifndef  EXEC_PORTS_H
#include "exec/ports.h"
#endif

#define  PRD_RAWWRITE      (CMD_NONSTD+0)
#define  PRD_PRTCOMMAND    (CMD_NONSTD+1)
#define  PRD_DUMPRPORT     (CMD_NONSTD+2)



#define aRIS     0  
#define aRIN     1  
#define aIND     2                 
#define aNEL     3           
#define aRI      4        

#define aSGR0    5      
#define aSGR3    6          
#define aSGR23   7  
#define aSGR4    8  
#define aSGR24   9  
#define aSGR1   10  
#define aSGR22  11  
#define aSFC    12  
#define aSBC    13  

#define aSHORP0 14  
#define aSHORP2 15  
#define aSHORP1 16  
#define aSHORP4 17  
#define aSHORP3 18  
#define aSHORP6 19  
#define aSHORP5 20  

#define aDEN6   21  
#define aDEN5   22  
#define aDEN4   23  
#define aDEN3   24  
#define aDEN2   25  
#define aDEN1   26  

#define aSUS2   27  
#define aSUS1   28  
#define aSUS4   29  
#define aSUS3   30  
#define aSUS0   31  
#define aPLU    32  
#define aPLD    33  

#define aFNT0   34  
#define aFNT1   35  
#define aFNT2   36  
#define aFNT3   37  
#define aFNT4   38  
#define aFNT5   39  
#define aFNT6   40  
#define aFNT7   41  
#define aFNT8   42  
#define aFNT9   43  
#define aFNT10  44  
                              
#define aPROP2  45  
#define aPROP1  46  
#define aPROP0  47  
#define aTSS    48  
#define aJFY5   49  
#define aJFY7   50  
#define aJFY6   51  
#define aJFY0   52  
#define aJFY3   53  
#define aJFY1   54  

#define aVERP0  55  
#define aVERP1  56  
#define aSLPP   57  
#define aPERF   58  
#define aPERF0  59  
                        
#define aLMS    60  
#define aRMS    61  
#define aTMS    62  
#define aBMS    63  
#define aSTBM   64  
#define aSLRM   65  
#define aCAM    66  

#define aHTS    67  
#define aVTS    68  
#define aTBC0   69  
#define aTBC3   70  
#define aTBC1   71  
#define aTBC4   72  
#define aTBCALL 73  
#define aTBSALL 74  
#define aEXTEND 75  

struct IOPrtCmdReq {
    struct  Message io_Message;
    struct  Device  *io_Device;     
    struct  Unit    *io_Unit;       
    UWORD   io_Command;             
    UBYTE   io_Flags;
    BYTE    io_Error;               
    UWORD   io_PrtCommand;          
    UBYTE   io_Parm0;               
    UBYTE   io_Parm1;               
    UBYTE   io_Parm2;               
    UBYTE   io_Parm3;               
};

struct IODRPReq {
    struct  Message io_Message;
    struct  Device  *io_Device;     
    struct  Unit    *io_Unit;       
    UWORD   io_Command;             
    UBYTE   io_Flags;
    BYTE    io_Error;               
    struct  RastPort *io_RastPort;  
    struct  ColorMap *io_ColorMap;  
    ULONG   io_Modes;               
    UWORD   io_SrcX;                
    UWORD   io_SrcY;                
    UWORD   io_SrcWidth;            
    UWORD   io_SrcHeight;           
    LONG    io_DestCols;            
    LONG    io_DestRows;            
    UWORD   io_Special;             
};

#define  SPECIAL_MILCOLS   0x001    
#define  SPECIAL_MILROWS   0x002    
#define  SPECIAL_FULLCOLS  0x004    
#define  SPECIAL_FULLROWS  0x008    
#define  SPECIAL_FRACCOLS  0x010    
#define  SPECIAL_FRACROWS  0x020    
#define  SPECIAL_ASPECT    0x080    
#define  SPECIAL_DENSITYMASK  0xf00 
#define  SPECIAL_DENSITY1  0x100    
#define  SPECIAL_DENSITY2  0x200    
#define  SPECIAL_DENSITY3  0x300    
#define  SPECIAL_DENSITY4  0x400    

#define  PDERR_CANCEL         1     
#define  PDERR_NOTGRAPHICS    2     
#define  PDERR_INVERTHAM      3     
#define  PDERR_BADDIMENSION   4     
#define  PDERR_DIMENSIONOVFLOW   5  
#define  PDERR_INTERNALMEMORY 6     
#define  PDERR_BUFFERMEMORY   7     
#endif
