// devices/trackdisk.h
//
// (C) 1987, MTS Associates
//
#ifndef DEVICES_TRACKDISK_H
#define DEVICES_TRACKDISK_H
#ifndef EXEC_IO_H
#include "exec/io.h"
#endif !EXEC_IO_H
#ifndef EXEC_DEVICES_H
#include "exec/devices.h"
#endif !EXEC_DEVICES_H

#define NUMSECS 11
#define NUMUNITS 4
#define TD_SECTOR 512
#define TD_SECSHIFT 9
#define TD_NAME "trackdisk.device"
#define TDF_EXTCOM (1<<15)
#define TD_MOTOR        (CMD_NONSTD+0)
#define TD_SEEK         (CMD_NONSTD+1)
#define TD_FORMAT       (CMD_NONSTD+2)
#define TD_REMOVE       (CMD_NONSTD+3)
#define TD_CHANGENUM    (CMD_NONSTD+4)
#define TD_CHANGESTATE  (CMD_NONSTD+5)
#define TD_PROTSTATUS   (CMD_NONSTD+6)
#define TD_RAWREAD      (CMD_NONSTD+7)
#define TD_RAWWRITE     (CMD_NONSTD+8)
#define TD_GETDRIVETYPE (CMD_NONSTD+9)
#define TD_GETNUMTRACKS (CMD_NONSTD+10)
#define TD_ADDCHANGEINT (CMD_NONSTD+11)
#define TD_REMCHANGEINT (CMD_NONSTD+12)
#define TD_LASTCOMM     (CMD_NONSTD+13)
#define ETD_WRITE       (CMD_WRITE|TDF_EXTCOM)
#define ETD_READ        (CMD_READ|TDF_EXTCOM)
#define ETD_MOTOR       (TD_MOTOR|TDF_EXTCOM)
#define ETD_SEEK        (TD_SEEK|TDF_EXTCOM)
#define ETD_FORMAT      (TD_FORMAT|TDF_EXTCOM)
#define ETD_UPDATE      (CMD_UPDATE|TDF_EXTCOM)
#define ETD_CLEAR       (CMD_CLEAR|TDF_EXTCOM)
#define ETD_RAWREAD     (TD_RAWREAD|TDF_EXTCOM)
#define ETD_RAWWRITE    (TD_RAWWRITE|TDF_EXTCOM)

struct IOExtTD : public IOStdReq {
 ULONG   iotd_Count;
 ULONG   iotd_SecLabel;
};

#define IOTDB_INDEXSYNC 4
#define IOTDF_INDEXSYNC (1<<4)
#define TD_LABELSIZE 16
#define TDB_ALLOW_NON_3_5       0
#define TDF_ALLOW_NON_3_5       (1<<0)
#define DRIVE3_5        1
#define DRIVE5_25       2
#define TDERR_NotSpecified      20
#define TDERR_NoSecHdr          21
#define TDERR_BadSecPreamble    22
#define TDERR_BadSecID          23
#define TDERR_BadHdrSum         24
#define TDERR_BadSecSum         25
#define TDERR_TooFewSecs        26
#define TDERR_BadSecHdr         27
#define TDERR_WriteProt         28
#define TDERR_DiskChanged       29
#define TDERR_SeekError         30
#define TDERR_NoMem             31
#define TDERR_BadUnitNum        32
#define TDERR_BadDriveType      33
#define TDERR_DriveInUse        34
#define TDERR_PostReset         35

struct TDU_PublicUnit : public Unit {
 UWORD   tdu_Comp01Track;
 UWORD   tdu_Comp10Track;
 UWORD   tdu_Comp11Track;
 ULONG   tdu_StepDelay;
 ULONG   tdu_SettleDelay;
 UBYTE   tdu_RetryCnt;
};
#endif DEVICES_TRACKDISK_H
