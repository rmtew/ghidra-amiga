#ifndef LIBRARIES_DOSEXTENS_H
#include <libraries/dosextens.h>
#endif
extern struct DosLibrary *DOSBase;
BPTR Open(char *, long);
void Close(BPTR);
long Read(BPTR, char *, long);
long Write(BPTR, char *, long);
BPTR Input(void);
BPTR Output(void);
long Seek(BPTR, long, long);
long DeleteFile(char *);
long Rename(char *, char *);
BPTR Lock(char *, long);
void UnLock(BPTR);
BPTR DupLock(BPTR);
long Examine(BPTR, struct FileInfoBlock *);
long ExNext(BPTR, struct FileInfoBlock *);
long Info(BPTR, struct InfoData *);
BPTR CreateDir(char *);
BPTR CurrentDir(BPTR);
long IoErr(void);
struct MsgPort *CreateProc(char *, long, BPTR, long);
void Exit(long);
BPTR LoadSeg(char *);
void UnLoadSeg(BPTR);
struct MsgPort *DeviceProc(char *);
long SetComment(char *, char *);
long SetProtection(char *, long);
long * DateStamp(long *);
void Delay(long);
long WaitForChar(BPTR, long);
BPTR ParentDir(BPTR);
long IsInteractive(BPTR);
long Execute(char *, BPTR, BPTR);
#ifndef  NO_PRAGMAS
#pragma libcall DOSBase Open 1e 2102
#pragma libcall DOSBase Close 24 101
#pragma libcall DOSBase Read 2a 32103
#pragma libcall DOSBase Write 30 32103
#pragma libcall DOSBase Input 36 0
#pragma libcall DOSBase Output 3c 0
#pragma libcall DOSBase Seek 42 32103
#pragma libcall DOSBase DeleteFile 48 101
#pragma libcall DOSBase Rename 4e 2102
#pragma libcall DOSBase Lock 54 2102
#pragma libcall DOSBase UnLock 5a 101
#pragma libcall DOSBase DupLock 60 101
#pragma libcall DOSBase Examine 66 2102
#pragma libcall DOSBase ExNext 6c 2102
#pragma libcall DOSBase Info 72 2102
#pragma libcall DOSBase CreateDir 78 101
#pragma libcall DOSBase CurrentDir 7e 101
#pragma libcall DOSBase IoErr 84 0
#pragma libcall DOSBase CreateProc 8a 432104
#pragma libcall DOSBase Exit 90 101
#pragma libcall DOSBase LoadSeg 96 101
#pragma libcall DOSBase UnLoadSeg 9c 101
#pragma libcall DOSBase DeviceProc ae 101
#pragma libcall DOSBase SetComment b4 2102
#pragma libcall DOSBase SetProtection ba 2102
#pragma libcall DOSBase DateStamp c0 101
#pragma libcall DOSBase Delay c6 101
#pragma libcall DOSBase WaitForChar cc 2102
#pragma libcall DOSBase ParentDir d2 101
#pragma libcall DOSBase IsInteractive d8 101
#pragma libcall DOSBase Execute de 32103
#endif
