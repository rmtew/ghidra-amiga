#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
typedef void (*__fptr)();
/*------ special functions ---------------------------------------------*/
void InitCode(long, long);
void InitStruct(char *, char *, long);
struct Library *MakeLibrary(long**, char *, __fptr, long, char *);
void MakeFunctions(char *, long**, long);
struct Resident *FindResident(char *);
void InitResident(struct Resident *, char *);
void Alert(long, char *);
void Debug(long);
/*------ interrupts ----------------------------------------------------*/
void Disable(void);
void Enable(void);
void Forbid(void);
void Permit(void);
long SetSR(long, long);
long SuperState(void);
void UserState(char *);
struct Interrupt *SetIntVector(long, struct Interrupt *);
void AddIntServer(long, struct Interrupt *);
void RemIntServer(long, struct Interrupt *);
void Cause(struct Interrupt *);
/*------ memory allocation: ---------------------------------------------*/
void * Allocate(struct MemHeader *, long);
void Deallocate(struct MemHeader *, void *, long);
void* AllocMem(long, long);
void AllocAbs(long, void*);
void FreeMem(void *, long);
long AvailMem(long);
struct MemList * AllocEntry(struct MemList *);
void FreeEntry(struct MemList *);
/*------ lists: ---------------------------------------------------------*/
void Insert(struct List *, struct Node *, struct Node *);
void AddHead(struct List *, struct Node *);
void AddTail(struct List *, struct Node *);
void Remove(struct Node *);
struct Node *RemHead(struct List *);
struct Node *RemTail(struct List *);
void Enqueue(struct List *, struct Node *);
struct Node *FindName(struct List *, char *);
/*------ tasks: ---------------------------------------------------------*/
void AddTask(struct Task *, char *, char *);
void RemTask(struct Task *);
struct Task *FindTask(char *);
long SetTaskPri(struct Task *, long);
long SetSignal(long, long);
long SetExcept(long, long);
long Wait(long);
void Signal(struct Task *, long);
long AllocSignal(long);
void FreeSignal(long);
long AllocTrap(long);
void FreeTrap(long);
/*------ messages: ------------------------------------------------------*/
void AddPort(struct MsgPort *);
void RemPort(struct MsgPort *);
void PutMsg(struct MsgPort *, struct Message *);
struct Message *GetMsg(struct MsgPort *);
void ReplyMsg(struct Message *);
struct Message *WaitPort(struct MsgPort *);
struct MsgPort *FindPort(char *);
/*------ libraries: -----------------------------------------------------*/
void AddLibrary(struct Library *);
long RemLibrary(struct Library *);
struct Library *OldOpenLibrary(char *);
void CloseLibrary(struct Library *);
__fptr SetFunction(struct Library *, long, __fptr);
void SumLibrary(struct Library *);
/*------ devices: -------------------------------------------------------*/
void AddDevice(struct Device *);
long RemDevice(struct Device *);
long OpenDevice(char *, long, struct IORequest *, long);
void CloseDevice(struct IORequest *);
long DoIO(struct IORequest *);
void SendIO(struct IORequest *);
long CheckIO(struct IORequest *);
long WaitIO(struct IORequest *);
void AbortIO(struct IORequest *);
/*------ resources: ----------------------------------------------------*/
void AddResource(struct Resource *);
void RemResource(struct Resource *);
struct Resource *OpenResource(char *);
/*------ new functions:*/
long GetCC(void);
void TypeOfMem(char *);
long Procure(struct Semaphore *, struct Message *);
void Vacate(struct Semaphore *);
struct Library *OpenLibrary(char *, long);
/*------ 1.2 new semaphore support*/
void InitSemaphore(struct SignalSemaphore *);
void ObtainSemaphore(struct SignalSemaphore *);
void ReleaseSemaphore(struct SignalSemaphore *);
long AttemptSemaphore(struct SignalSemaphore *);
void ObtainSemaphoreList(struct List *);
void ReleaseSemaphoreList(struct List *);
struct SignalSemaphore *FindSemaphore(char *);
void AddSemaphore(struct SignalSemaphore *);
void RemSemaphore(struct SignalSemaphore *);
/*------ 1.2 rom "kickstart" support + memory support*/
void SumKickData(void);
long AddMemList(long, long, long, char *, char *);
void CopyMem(char *, char *, long);
void CopyMemQuick(char *, char *, long);
/*------ Common support library functions ---------*/
void BeginIO(struct IORequest *);
struct IORequest *CreateExtIO(struct MsgPort *, long);
struct MsgPort *CreatePort(char *, long);
struct IOStdReq *CreateStdIO(struct MsgPort *);
struct Task *CreateTask(char *, unsigned char, APTR, unsigned long);
void DeleteExtIO(struct IORequest *);
void DeletePort(struct MsgPort *);
void DeleteStdIO(struct IOStdReq *);
void DeleteTask(struct Task *);
void NewList(struct List *);
#ifndef  NO_PRAGMAS
/*------ special functions ---------------------------------------------*/
#pragma syscall InitCode 48 1002
#pragma syscall InitStruct 4e a903
#pragma syscall MakeLibrary 54 10a9805
#pragma syscall MakeFunctions 5a a9803
#pragma syscall FindResident 60 901
#pragma syscall InitResident 66 1902
#pragma syscall Alert 6c d702
#pragma syscall Debug 72 0
/*------ interrupts ----------------------------------------------------*/
#pragma syscall Disable 78 0
#pragma syscall Enable 7e 0
#pragma syscall Forbid 84 0
#pragma syscall Permit 8a 0
#pragma syscall SetSR 90 1002
#pragma syscall SuperState 96 0
#pragma syscall UserState 9c 1
#pragma syscall SetIntVector a2 9002
#pragma syscall AddIntServer a8 9002
#pragma syscall RemIntServer ae 9002
#pragma syscall Cause b4 901
/*------ memory allocation: ---------------------------------------------*/
#pragma syscall Allocate ba 802
#pragma syscall Deallocate c0 9803
#pragma syscall AllocMem c6 1002
#pragma syscall AllocAbs cc 9002
#pragma syscall FreeMem d2 902
#pragma syscall AvailMem d8 101
#pragma syscall AllocEntry de 801
#pragma syscall FreeEntry e4 801
/*------ lists: ---------------------------------------------------------*/
#pragma syscall Insert ea a9803
#pragma syscall AddHead f0 9802
#pragma syscall AddTail f6 9802
#pragma syscall Remove fc 901
#pragma syscall RemHead 102 801
#pragma syscall RemTail 108 801
#pragma syscall Enqueue 10e 9802
#pragma syscall FindName 114 9802
/*------ tasks: ---------------------------------------------------------*/
#pragma syscall AddTask 11a ba903
#pragma syscall RemTask 120 901
#pragma syscall FindTask 126 901
#pragma syscall SetTaskPri 12c 902
#pragma syscall SetSignal 132 1002
#pragma syscall SetExcept 138 1002
#pragma syscall Wait 13e 1
#pragma syscall Signal 144 902
#pragma syscall AllocSignal 14a 1
#pragma syscall FreeSignal 150 1
#pragma syscall AllocTrap 156 1
#pragma syscall FreeTrap 15c 1
/*------ messages: ------------------------------------------------------*/
#pragma syscall AddPort 162 901
#pragma syscall RemPort 168 901
#pragma syscall PutMsg 16e 9802
#pragma syscall GetMsg 174 801
#pragma syscall ReplyMsg 17a 901
#pragma syscall WaitPort 180 801
#pragma syscall FindPort 186 901
/*------ libraries: -----------------------------------------------------*/
#pragma syscall AddLibrary 18c 901
#pragma syscall RemLibrary 192 901
#pragma syscall OldOpenLibrary 198 901
#pragma syscall CloseLibrary 19e 901
#pragma syscall SetFunction 1a4 8903
#pragma syscall SumLibrary 1aa 901
/*------ devices: -------------------------------------------------------*/
#pragma syscall AddDevice 1b0 901
#pragma syscall RemDevice 1b6 901
#pragma syscall OpenDevice 1bc 190804
#pragma syscall CloseDevice 1c2 901
#pragma syscall DoIO 1c8 901
#pragma syscall SendIO 1ce 901
#pragma syscall CheckIO 1d4 901
#pragma syscall WaitIO 1da 901
#pragma syscall AbortIO 1e0 901
/*------ resources: ----------------------------------------------------*/
#pragma syscall AddResource 1e6 901
#pragma syscall RemResource 1ec 901
#pragma syscall OpenResource 1f2 901
/*------ new functions:*/
#pragma syscall GetCC 210 0
#pragma syscall TypeOfMem 216 901
#pragma syscall Procure 21c 9802
#pragma syscall Vacate 222 801
#pragma syscall OpenLibrary 228 902
/*------ 1.2 new semaphore support*/
#pragma syscall InitSemaphore 22e 801
#pragma syscall ObtainSemaphore 234 801
#pragma syscall ReleaseSemaphore 23a 801
#pragma syscall AttemptSemaphore 240 801
#pragma syscall ObtainSemaphoreList 246 801
#pragma syscall ReleaseSemaphoreList 24c 801
#pragma syscall FindSemaphore 252 801
#pragma syscall AddSemaphore 258 801
#pragma syscall RemSemaphore 25e 801
/*------ 1.2 rom "kickstart" support + memory support*/
#pragma syscall SumKickData 264 0
#pragma syscall AddMemList 26a 9821005
#pragma syscall CopyMem 270 9803
#pragma syscall CopyMemQuick 276 9803
#endif
