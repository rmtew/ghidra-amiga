




#ifndef DEVICES_SERIAL_H
#define DEVICES_SERIAL_H

#ifndef   EXEC_IO_H
#include "exec/io.h"
#endif   !EXEC_IO_H

                   
                   

 struct  IOTArray {    
        ULONG TermArray0; 
        ULONG TermArray1;
};




 
 struct  IOExtSer {
        struct   IOStdReq IOSer;

  
   ULONG   io_CtlChar;    
   ULONG   io_RBufLen;    
   ULONG   io_ExtFlags;   
   ULONG   io_Baud;       
   ULONG   io_BrkTime;    
   struct  IOTArray io_TermArray; 
   UBYTE   io_ReadLen;    
   UBYTE   io_WriteLen;   
   UBYTE   io_StopBits;   
   UBYTE   io_SerFlags;   
   UWORD   io_Status;  
};
   

#define   SDCMD_QUERY           CMD_NONSTD
#define   SDCMD_BREAK          (CMD_NONSTD+1)
#define   SDCMD_SETPARAMS      (CMD_NONSTD+2)
      
#define SERB_XDISABLED  7       
#define SERF_XDISABLED  (1<<7)  
#define  SERB_EOFMODE   6       
#define  SERF_EOFMODE   (1<<6)  
#define  SERB_SHARED    5       
#define  SERF_SHARED    (1<<5)  
#define SERB_RAD_BOOGIE 4       
#define SERF_RAD_BOOGIE (1<<4)  
#define  SERB_QUEUEDBRK 3       
#define  SERF_QUEUEDBRK (1<<3)  
#define  SERB_7WIRE     2       
#define  SERF_7WIRE     (1<<2)  
#define  SERB_PARTY_ODD 1       
#define  SERF_PARTY_ODD (1<<1)  
#define  SERB_PARTY_ON  0        
#define  SERF_PARTY_ON  (1<<0)  
#define  IOSERB_BUFRREAD 7      
#define  IOSERF_BUFRREAD (1<<7) 
#define  IOSERB_QUEUED  6       
#define  IOSERF_QUEUED  (1<<6)  
#define  IOSERB_ABORT   5       
#define  IOSERF_ABORT   (1<<5)  
#define  IOSERB_ACTIVE  4       
#define  IOSERF_ACTIVE  (1<<4)  
#define  IOSTB_XOFFREAD 4       
#define  IOSTF_XOFFREAD (1<<4)  
#define  IOSTB_XOFFWRITE 3      
#define  IOSTF_XOFFWRITE (1<<3) 
#define  IOSTB_READBREAK 2      
#define  IOSTF_READBREAK (1<<2) 
#define  IOSTB_WROTEBREAK 1     
#define  IOSTF_WROTEBREAK (1<<1) 
#define  IOSTB_OVERRUN  0       
#define  IOSTF_OVERRUN  (1<<0)  

#define SerErr_DevBusy         1
#define SerErr_BaudMismatch    2
#define SerErr_InvBaud         3
#define SerErr_BufErr          4
#define SerErr_InvParam        5
#define SerErr_LineErr         6
#define SerErr_NotOpen         7    
#define SerErr_PortReset       8   
#define SerErr_ParityErr       9
#define SerErr_InitErr        10
#define SerErr_TimerErr       11
#define SerErr_BufOverflow    12
#define SerErr_NoDSR          13
#define SerErr_NoCTS          14
#define SerErr_DetectedBreak  15

#define SERIALNAME     "serial.device"

#endif !DEVICES_SERIAL_H
