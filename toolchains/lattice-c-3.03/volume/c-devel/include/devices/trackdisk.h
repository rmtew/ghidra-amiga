






#ifndef DEVICES_TRACKDISK_H
#define DEVICES_TRACKDISK_H

#ifndef EXEC_IO_H
#include "exec/io.h"
#endif !EXEC_IO_H



#define  NUMCYLS  80             
#define  MAXCYLS  (NUMCYLS+20)   
#define  NUMSECS  11
#define  NUMHEADS 2
#define  MAXRETRY 10
#define  NUMTRACKS (NUMCYLS*NUMHEADS)
#define NUMUNITS 4




#define  TD_SECTOR 512
#define  TD_SECSHIFT 9        





#define  TD_NAME  "trackdisk.device"

#define  TDF_EXTCOM (1<<15)            


#define  TD_MOTOR       (CMD_NONSTD+0) 
#define  TD_SEEK        (CMD_NONSTD+1) 
#define  TD_FORMAT      (CMD_NONSTD+2) 
#define  TD_REMOVE      (CMD_NONSTD+3) 
#define  TD_CHANGENUM   (CMD_NONSTD+4) 
#define  TD_CHANGESTATE (CMD_NONSTD+5) 
#define  TD_PROTSTATUS  (CMD_NONSTD+6) 

#define  TD_LASTCOMM TD_PROTSTATUS



#define  ETD_WRITE   (CMD_WRITE|TDF_EXTCOM)
#define  ETD_READ    (CMD_READ|TDF_EXTCOM)
#define  ETD_MOTOR   (TD_MOTOR|TDF_EXTCOM)
#define  ETD_SEEK    (TD_SEEK|TDF_EXTCOM)
#define  ETD_FORMAT  (TD_FORMAT|TDF_EXTCOM)
#define  ETD_UPDATE  (CMD_UPDATE|TDF_EXTCOM)
#define  ETD_CLEAR   (CMD_CLEAR|TDF_EXTCOM)



struct IOExtTD {
   struct   IOStdReq iotd_Req;
   ULONG iotd_Count;
   ULONG iotd_SecLabel;
};



#define  TD_LABELSIZE 16



#define  TDERR_NotSpecified   20
#define  TDERR_NoSecHdr       21
#define  TDERR_BadSecPreamble 22
#define  TDERR_BadSecID       23
#define  TDERR_BadHdrSum      24
#define  TDERR_BadSecSum      25
#define  TDERR_TooFewSecs     26
#define  TDERR_BadSecHdr      27
#define  TDERR_WriteProt      28
#define  TDERR_DiskChanged    29
#define  TDERR_SeekError      30
#define  TDERR_NoMem          31
#define  TDERR_BadUnitNum     32
#define  TDERR_BadDriveType   33
#define  TDERR_DriveInUse     34

#endif DEVICES_TRACKDISK_H
