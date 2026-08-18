/* Copyright Manx Software Systems, Inc. 1989.  All rights reserved */

#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H
#include <libraries/dos.h>

typedef void (*PVF)();

long Chk_Abort(void);
extern int Enable_Abort;

/************** structs ****************************/
struct cLPool;
struct InputEvent;
struct Device;
struct KeyMap;
struct TextAttr;
struct TextFont;
struct FileHandle;
struct FileLock;
struct FileInfoBlock;
struct InfoData;
struct Process;
struct Library;
struct MemList;
struct Resident;
struct Interrupt;
struct MemHeader;
struct List;
struct Node;
struct Task;
struct MsgPort;
struct Message;
struct IORequest;
struct MiscResource;
struct Semaphore;
struct SignalSemaphore;
struct IOStdReq;
struct ConfigDev;
struct CurrentBinding;
struct DeviceNode;
struct BitMap;
struct RastPort;
struct Bob;
struct VSprite;
struct ViewPort;
struct GelsInfo;
struct AnimOb;
struct View;
struct bltnode;
struct AreaInfo;
struct UCopList;
struct SimpleSprite;
struct Layer;
struct TmpRas;
struct Region;
struct Rectangle;
struct CopList;
struct cprlist;
struct ColorMap;
struct WBObject;
struct DiskObject;
struct FreeList;
struct Window;
struct Gadget;
struct Screen;
struct Border;
struct Image;
struct Requester;
struct Preferences;
struct Menu;
struct MenuItem;
struct NewScreen;
struct NewWindow;
struct IntuiText;
struct Remember;
struct Layer_Info;
struct Layer;
struct ClipRect;
struct timeval;

/************** CList ******************************/

long InitCLPool(struct cLPool *cLPool,long size);
long AllocCList(long cLPool);
void FreeCList(long cList);
void FlushCList(long cList);
long SizeCList(long cList);
long PutCLChar(long cList,long byte);
long GetCLChar(long cList);
long UnGetCLChar(long cList,long byte);
long UnPutCLChar(long cList);
long PutCLWord(long cList,long word);
long GetCLWord(long cList);
long UnGetCLWord(long cList,long word);
long UnPutCLWord(long cList);
long PutCLBuf(long cList,char *buffer,long length);
long GetCLBuf(long cList,char *buffer,long maxlength);
long MarkCList(long cList, long mark);
long IncrCLMark(long cList);
long PeekCLMark(long cList);
long SplitCList(long cList);
long CopyCList(long cList);
long SubCList(long cList,long index,long length);
long ConcatCList(long sourceCList,long destCList);

/************** Console ****************************/

struct InputEvent * CDInputHandler(struct InputEvent *event,struct Device *device);
long RawKeyConvert(struct InputEvent *event,char *buffer,long length,struct KeyMap *keyMap);

/************** DiskFont ***************************/

struct TextFont * OpenDiskFont(struct TextAttr *textAttr);
long AvailFonts(char *buffer,long bufBytes,long flags);

/************** Dos ********************************/

BPTR Open(const char *name, long accessMode);
void Close(BPTR file);
long Read(BPTR file, char *buffer, long length);
long Write(BPTR file, char *buffer, long length);
BPTR Input(void);
BPTR Output(void);
long Seek(BPTR file, long position, long offset);
long DeleteFile(const char *name);
long Rename(const char *oldName, const char *newName);
BPTR Lock(const char *name, long type);
void UnLock(BPTR);
BPTR DupLock(BPTR);
long Examine(BPTR lock, BPTR fileInfoBlock);
long ExNext(BPTR lock, BPTR fileInfoBlock);
long Info(BPTR lock, struct InfoData *parameterBlock);
BPTR CreateDir(const char *name);
BPTR CurrentDir(struct FileLock *lock);
long IoErr(void);
struct Process * CreateProc(const char *name, long pri, long segList, long stackSize);
void Exit(long returnCode);
long LoadSeg(const char *fileName);
long UnLoadSeg(long segList);
long GetPacket(long wait);
long QueuePacket(long *packet);
struct Process * DeviceProc(const char *name);
long SetComment(const char *name, const char *comment);
long SetProtection(const char *name, long mask);
long * DateStamp(long *date);
void Delay(long timeout);
long WaitForChar(struct FileHandle *file, long timeout);
struct FileLock * ParentDir(struct FileLock *lock);
long IsInteractive(struct FileHandle *file);
long Execute(const char *string, struct FileHandle *input, struct FileHandle *output);

/************** Exec *******************************/

void Supervisor(void);
void ExitIntr(void);
void Schedule(void);
void Reschedule(void);
void Switch(void);
void Dispatch(void);
void Exception(void);
void InitCode(long startClass, long version);
void InitStruct(short *initTable, void *memory, long size);
struct Library * MakeLibrary(PVF funcInit[], short *structInit, struct Library *(*libInit)(), long dataSize, struct MemList *codeSize);
long MakeFunctions(char *target, PVF functionArray[], char *funcDispBase);
struct Resident * FindResident(const char *name);
void InitResident(struct Resident *resident, long segList);
long Alert(long alertNum, long parameters);
void Debug(void);
void Disable(void);
void Enable(void);
void Forbid(void);
void Permit(void);
long SetSR(long newSR, long mask);
long SuperState(void);
void UserState(void *sysStack);
struct Interrupt * SetIntVector(long intNumber, struct Interrupt *interrupt);
void AddIntServer(long intNumber,struct Interrupt *interrupt);
void RemIntServer(long intNumber, struct Interrupt *interrupt);
void Cause(struct Interrupt *interrupt);
void * Allocate(struct MemHeader *freeList, long byteSize);
void Deallocate(struct MemHeader *freeList, void *memoryBlock, long byteSize);
void * AllocMem(long byteSize, long requirements);
void * AllocAbs(long byteSize, void *location);
void FreeMem(void *memoryBlock, long byteSize);
long AvailMem(long requirements);
struct MemList * AllocEntry(struct MemList *entry);
void FreeEntry(struct MemList *entry);
void Insert(struct List *list, struct Node *node, struct Node *pred);
void AddHead(struct List *list, struct Node *node);
void AddTail(struct List *list, struct Node *node);
void Remove(struct Node *node);
struct Node * RemHead(struct List *list);
struct Node * RemTail(struct List *list);
void Enqueue(struct List *list, struct Node *node);
struct Node * FindName(struct List *list, const char *name);
void AddTask(struct Task *task, void *initPC, void *finalPC);
void RemTask(struct Task *task);
struct Task * FindTask(const char *name);
long SetTaskPri(struct Task *task, long priority);
long SetSignal(long newSignals, long signalSet);
long SetExcept(long newSignals, long signalSet);
long Wait(long signalSet);
void Signal(struct Task *task, long signalSet);
long AllocSignal(long signalNum);
void FreeSignal(long signalNum);
long AllocTrap(long trapNum);
void FreeTrap(long trapNum);
void AddPort(struct MsgPort *port);
void RemPort(struct MsgPort *port);
void PutMsg(struct MsgPort *port, struct Message *message);
struct Message * GetMsg(struct MsgPort *port);
void ReplyMsg(struct Message *message);
struct Message * WaitPort(struct MsgPort *port);
struct MsgPort * FindPort(const char *name);
void AddLibrary(struct Library *library);
long RemLibrary(struct Library *library);
void * OldOpenLibrary(const char *libName);
void CloseLibrary(void *library);
PVF SetFunction(struct Library *library, long funcOffset, PVF funcEntry);
void SumLibrary(struct Library *library);
void AddDevice(struct Device *device);
long RemDevice(struct Device *device);
long OpenDevice(const char *devName, long unit, struct IORequest *ioRequest, long flags);
void CloseDevice(struct IORequest *ioRequest);
long DoIO(struct IORequest *ioRequest);
void SendIO(struct IORequest *ioRequest);
struct IORequest * CheckIO(struct IORequest *ioRequest);
long WaitIO(struct IORequest *ioRequest);
unsigned char AbortIO(struct IORequest *ioRequest);
void AddResource(struct MiscResource *resource);
void RemResource(struct MiscResource *resource);
struct MiscResource * OpenResource(const char *resName, long version);
void RawIOInit(void);
long RawMayGetChar(void);
void RawPutChar(long c);
void RawDoFmt(const char *FormatString, char *DataStream, PVF PutChProc, long PutChData);
long GetCC(void);
long TypeOfMem(char *address);
long Procure(struct Semaphore *semaport, struct Message *bidMsg);
void Vacate(struct Semaphore *semaport);
void * OpenLibrary(const char *libName, long version);
void InitSemaphore(struct SignalSemaphore *sigSem);
void ObtainSemaphore(struct SignalSemaphore *sigSem);
void ReleaseSemaphore(struct SignalSemaphore *sigSem);
long AttemptSemaphore(struct SignalSemaphore *sigSem);
void ObtainSemaphoreList(struct List *sigSemList);
void ReleaseSemaphoreList(struct List *sigSemList);
struct SignalSemaphore * FindSemaphore(const char *name);
void AddSemaphore(struct SignalSemaphore *sigSem);
void RemSemaphore(struct SignalSemaphore *sigSem);
void SumKickData(void);
long AddMemList(long size, long attributes, long pri, void *base, const char *name);
void CopyMem(char *source, char *dest, long size);
void CopyMemQuick(char *source, char *dest, long size);

/* Not in ROM */
struct IORequest * CreateExtIO(struct MsgPort *ioReplyPort, long size);
struct MsgPort* CreatePort(const char *name,long pri);
struct IOStdReq * CreateStdIO(struct MsgPort *mp);
struct Task * CreateTask(const char *name,long pri,void *start_pc,long stksiz);
void DeleteExtIO (struct IORequest *ioReq);
void DeletePort(struct MsgPort *port);
void DeleteStdIO(struct IOStdReq *iop);
void DeleteTask(struct Task *tp);
void BeginIO(struct IORequest *ioRequest);
void NewList(struct List *list);


/************** Expansion **************************/

void AddConfigDev(struct ConfigDev *configDev);
long AllocBoardMem(long slotSpec);
struct ConfigDev * AllocConfigDev(void);
long AllocExpansionMem(long numSlots,long SlotAlign,long SlotOffset);
long ConfigBoard(long board,struct ConfigDev *configDev);
long ConfigChain(long baseAddr);
struct ConfigDev * FindConfigDev(struct ConfigDev *oldConfigDev,long manufacturer,long product);
void FreeBoardMem(long startSlot,long slotSpec);
void FreeConfigDev(struct ConfigDev *configDev);
void FreeExpansionMem(long startSlot,long numSlots);
long ReadExpansionByte(const char *board,long offset);
long ReadExpansionRom(const char *board,struct ConfigDev *configDev);
void RemConfigDev(struct ConfigDev *configDev);
long WriteExpansionByte(const char *board,long offset,long byte);
void ObtainConfigBinding(void);
void ReleaseConfigBinding(void);
void SetCurrentBinding(struct CurrentBinding *currentBinding,long bindingSize);
long GetCurrentBinding(struct CurrentBinding *currentBinding,long bindingSize);
struct DeviceNode * MakeDosNode(long *parmPacket);
long AddDosNode(long bootPri,long flags,struct DeviceNode *dosNode);

/************** Graphics ***************************/

long BltBitMap(struct BitMap *srcBitMap, long srcX, long srcY, struct BitMap *destBitMap, long destX, long destY, long sizeX, long sizeY, long minterm, long mask, long tempA);
void BltTemplate(char *source, long srcX, long srcMod, struct RastPort *destRastPort, long destX, long destY, long sizeX, long sizeY);
void ClearEOL(struct RastPort *rastPort);
void ClearScreen(struct RastPort *rastPort);
long TextLength(struct RastPort *rastPort, const char *string, long count);
long Text(struct RastPort *rastPort, const char *string, long count);
void SetFont(struct RastPort *rastPort, struct TextFont *textFont);
struct TextFont * OpenFont(struct TextAttr *textAttr);
void CloseFont(struct TextFont *textFont);
long AskSoftStyle(struct RastPort *rastPort);
long SetSoftStyle(struct RastPort *rastPort, long style, long enable);
void AddBob(struct Bob *bob, struct RastPort *rastPort);
void AddVSprite(struct VSprite *vSprite, struct RastPort *rastPort);
void DoCollision(struct RastPort *rastPort);
void DrawGList(struct RastPort *rastPort, struct ViewPort *viewPort);
void InitGels(struct VSprite *dummyHead, struct VSprite *dummyTail, struct GelsInfo *gelsInfo);
void InitMasks(struct VSprite *vSprite);
void RemIBob(struct Bob *bob, struct RastPort *rastPort, struct ViewPort *viewPort);
void RemVSprite(struct VSprite *vSprite);
void SetCollision(long type, PVF routine, struct GelsInfo *gelsInfo);
void SortGList(struct RastPort *rastPort);
void AddAnimOb(struct AnimOb *obj, struct AnimOb **animationKey, struct RastPort *rastPort);
void Animate(struct AnimOb **animationKey, struct RastPort *rastPort);
long GetGBuffers(struct AnimOb *animationObj, struct RastPort *rastPort, long doubleBuffer);
void InitGMasks(struct AnimOb *animationObj);
void DrawEllipse(struct RastPort *rastPort, long cx, long cy, long a, long b);
long AreaEllipse(struct RastPort *rastPort, long cx, long cy, long a, long b);
void LoadRGB4(struct ViewPort *viewPort, unsigned short *colors, long count);
void InitRastPort(struct RastPort *rastPort);
void InitVPort(struct ViewPort *viewPort);
void MrgCop(struct View *view);
void MakeVPort(struct View *view, struct ViewPort *viewPort);
void LoadView(struct View *view);
void WaitBlit(void);
void SetRast(struct RastPort *rastPort, long color);
void Move(struct RastPort *rastPort, long x, long y);
void Draw(struct RastPort *rastPort, long x, long y);
long AreaMove(struct RastPort *rastPort,long x,long y);
long AreaDraw(struct RastPort *rastPort,long x,long y);
void AreaEnd(struct RastPort *rastPort);
void WaitTOF(void);
void QBlit(struct bltnode *blit);
void InitArea(struct AreaInfo *areaInfo, short *vectorTable, long vectorTableSize);
void SetRGB4(struct ViewPort *viewPort,long index,long r,long g,long b);
void QBSBlit(struct bltnode *blit);
void BltClear(char *memory,long size,long flags);
void RectFill(struct RastPort *rastPort,long xl,long yl,long xu,long yu);
void BltPattern(struct RastPort *rastPort,char *ras,long xl,long yl,long maxX,long maxY,long fillBytes);
long ReadPixel(struct RastPort *rastPort,long x,long y);
void WritePixel(struct RastPort *rastPort,long x,long y);
long Flood(struct RastPort *rastPort,long mode,long x,long y);
void PolyDraw(struct RastPort *rastPort,long count,short *polyTable);
void SetAPen(struct RastPort *rastPort,long pen);
void SetBPen(struct RastPort *rastPort,long pen);
void SetDrMd(struct RastPort *rastPort,long drawMode);
void InitView(struct View *view);
void CBump(struct UCopList *copperList);
void CMove(struct UCopList *copperList,long *reg,long data);
void CWait(struct UCopList *copperList,long x,long y);
long VBeamPos(void);
void InitBitMap(struct BitMap *bitMap,long depth,long width,long height);
void ScrollRaster(struct RastPort *rastPort,long dX,long dY,long minx,long miny,long maxx,long maxy);
void WaitBOVP(struct ViewPort *viewport);
long GetSprite(struct SimpleSprite *simplesprite, long num);
void FreeSprite(long num);
void ChangeSprite(struct ViewPort *vp,struct SimpleSprite *simplesprite,short *data);
void MoveSprite(struct ViewPort *viewport,struct SimpleSprite *simplesprite,long x,long y);
void LockLayerRom(struct Layer *layer);
void UnlockLayerRom(struct Layer *layer);
void SyncSBitMap(struct Layer *l);
void CopySBitMap(struct Layer *l);
void OwnBlitter(void);
void DisownBlitter(void);
void InitTmpRas(struct TmpRas *tmpras,char *buff,long size);
void AskFont(struct RastPort *rastPort,struct TextAttr *textAttr);
void AddFont(struct TextFont *textFont);
long RemFont(struct TextFont *textFont);
void * AllocRaster(long width,long height);
void FreeRaster(void *planeptr,long width,long height);
void AndRectRegion(struct Region *rgn,struct Rectangle *rect);
void OrRectRegion(struct Region *rgn,struct Rectangle *rect);
struct Region * NewRegion(void);
long ClearRectRegion(struct Region *rgn, struct Rectangle *rect);
void ClearRegion(struct Region *rgn);
void DisposeRegion(struct Region *rgn);
void FreeVPortCopLists(struct ViewPort *viewport);
void FreeCopList(struct CopList *coplist);
void ClipBlit(struct RastPort *srcrp,long srcX,long srcY,struct RastPort *destrp,long destX,long destY,long sizeX,long sizeY,long minterm);
void XorRectRegion(struct Region *rgn,struct Rectangle *rect);
void FreeCprList(struct cprlist *cprlist);
struct ColorMap * GetColorMap(long entries);
void FreeColorMap(struct ColorMap *colormap);
long GetRGB4(struct ColorMap *colormap,long entry);
void ScrollVPort(struct ViewPort *vp);
void UCopperListInit(struct UCopList *copperlist,long num);
void FreeGBuffers(struct AnimOb *animationObj,struct RastPort *rastPort,long doubleBuffer);
long BltBitMapRastPort(struct BitMap *srcbm,long srcx,long srcy,struct RastPort *destrp,long destX,long destY,long sizeX,long sizeY,long minterm);
long OrRegionRegion(struct Region *src,struct Region *dst);
long XorRegionRegion(struct Region *src,struct Region *dst);
long AndRegionRegion(struct Region *src,struct Region *dst);
void SetRGB4CM(struct ColorMap *cm,long i,long r,long g,long b);
void BltMaskBitMapRastPort(struct BitMap *srcbm,long srcx,long srcy,struct RastPort *destrp,long destX,long destY,long sizeX,long sizeY,long minterm,void *bltmask);
void GraphicsReserved1(void);
void GraphicsReserved2(void);
long AttemptLockLayerRom(struct Layer *layer);

/* Not in ROM */
#ifndef RemBob
void RemBob(struct Bob *bob);
void InitAnimate(struct AnimOb **animKey);
#endif
#ifndef SetOPen
void SetOPen(struct RastPort *rp, long pen);
void SetDrPt(struct RastPort *rp, long pattern);
void SetWrMsk(struct RastPort *rp, long mask);
void SetAfPt(struct RastPort *rp, unsigned short *pattern, long size);
void BNDRYOFF(struct RastPort *rp);
void CINIT(struct UCopList *c, long n);
void CMOVE(struct UCopList *c, long a, long b);
void CWAIT(struct UCopList *c, long a, long b);
void CEND(struct UCopList *c);
void DrawCircle(struct RastPort *rp, long cx, long cy, long r);
long AreaCircle(struct RastPort *rp, long cx, long cy, long r);
#endif


/************** Icon *******************************/

struct WBObject * GetWBObject(const char *name);
long PutWBObject(const char *name,struct WBObject *object);
long GetIcon(const char *name,struct DiskObject *icon,struct FreeList *freelist);
long PutIcon(const char *name,struct DiskObject *icon);
void FreeFreeList(struct FreeList *freelist);
void FreeWBObject(struct WBObject *WBObject);
struct WBObject * AllocWBObject(void);
void AddFreeList(struct FreeList *freelist,char *mem,long size);
struct DiskObject * GetDiskObject(const char *name);
long PutDiskObject(const char *name,struct DiskObject *diskobj);
void FreeDiskObject(struct DiskObject *diskobj);
char * FindToolType(char **toolTypeArray,const char *typeName);
long MatchToolValue(char *typeString,char *value);
char * BumpRevision(const char *newname,const char *oldname);

/************** Intuition **************************/

void OpenIntuition(void);
struct InputEvent * Intuition(struct InputEvent *ievent);
long AddGadget(struct Window *AddPtr,struct Gadget *gadget,long Position);
long ClearDMRequest(struct Window *Window);
void ClearMenuStrip(struct Window *Window);
void ClearPointer(struct Window *Window);
void CloseScreen(struct Screen *Screen);
void CloseWindow(struct Window *Window);
long CloseWorkBench(void);
void CurrentTime(long *Seconds,long *Micros);
long DisplayAlert(long AlertNumber,char *String,long Height);
void DisplayBeep(struct Screen *Screen);
long DoubleClick(long sseconds,long smicros,long cseconds,long cmicros);
void DrawBorder(struct RastPort *RPort,struct Border *Border,long LeftOffset,long TopOffset);
void DrawImage(struct RastPort *RPort,struct Image *Image,long LeftOffset,long TopOffset);
void EndRequest(struct Requester *requester,struct Window *window);
struct Preferences * GetDefPrefs(struct Preferences *preferences,long size);
struct Preferences * GetPrefs(struct Preferences *preferences,long size);
void InitRequester(struct Requester *req);
struct MenuItem * ItemAddress(struct Menu *MenuStrip,long MenuNumber);
void ModifyIDCMP(struct Window *Window, long Flags);
void ModifyProp(struct Gadget *Gadget,struct Window *Ptr,struct Requester *Req,long Flags,long HPos,long VPos,long HBody,long VBody);
void MoveScreen(struct Screen *Screen,long dx,long dy);
void MoveWindow(struct Window *window,long dx,long dy);
void OffGadget(struct Gadget *Gadget, struct Window *Ptr,struct Requester *Req);
void OffMenu(struct Window *Window,long MenuNumber);
void OnGadget(struct Gadget *Gadget,struct Window *Ptr,struct Requester *Req);
void OnMenu(struct Window *Window,long MenuNumber);
struct Screen *OpenScreen(struct NewScreen *OSargs);
struct Window *OpenWindow(struct NewWindow *OWargs);
struct Screen *OpenWorkBench(void);
void PrintIText(struct RastPort *rp,struct IntuiText *itext,long left,long top);
void RefreshGadgets(struct Gadget *Gadgets,struct Window *Ptr,struct Requester *Req);
long RemoveGadget(struct Window *RemPtr,struct Gadget *Gadget);
void ReportMouse(long Boolean, struct Window *Window);
long Request(struct Requester *Requester,struct Window *Window);
void ScreenToBack(struct Screen *Screen);
void ScreenToFront(struct Screen *Screen);
long SetDMRequest(struct Window *Window,struct Requester *req);
void SetMenuStrip(struct Window *Window,struct Menu *Menu);
void SetPointer(struct Window *Window,short *Pointer,long Height,long Width,long Xoffset,long Yoffset);
void SetWindowTitles(struct Window *window,const char *windowtitle,const char *screentitle);
void ShowTitle(struct Screen *Screen,long ShowIt);
void SizeWindow(struct Window *window,long dx,long dy);
struct View * ViewAddress(void);
struct View * ViewPortAddress(struct Window *window);
void WindowToBack(struct Window *window);
void WindowToFront(struct Window *window);
long WindowLimits(struct Window *window,long minwidth,long minheight,long maxwidth,long maxheight);
struct Preferences * SetPrefs(struct Preferences *preferences,long size,long flag);
long IntuiTextLength(struct IntuiText *itext);
long WBenchToBack(void);
long WBenchToFront(void);
long AutoRequest(struct Window *Window,struct IntuiText *Body,struct IntuiText *PText,struct IntuiText *NText,long PFlag,long NFlag,long W,long H);
void BeginRefresh(struct Window *Window);
struct Window * BuildSysRequest(struct Window *Window,struct IntuiText *Body,struct IntuiText *PosText,struct IntuiText *NegText,long Flags,long W,long H);
void EndRefresh(struct Window *Window,long Complete);
void FreeSysRequest(struct Window *Window);
void MakeScreen(struct Screen *Screen);
void RemakeDisplay(void);
void RethinkDisplay(void);
void * AllocRemember(struct Remember **RememberKey,long Size,long Flags);
void AlohaWorkbench(struct MsgPort *wbport);
void FreeRemember(struct Remember **RememberKey,long ReallyForget);
long LockIBase(long dontknow);
void UnlockIBase(long IBLock);
long GetScreenData(char *buffer,long size,long type,struct Screen *screen);
void RefreshGList(struct Gadget *Gadgets,struct Window *Ptr,struct Requester *Req,long NumGad);
long AddGList(struct Window *AddPtr,struct Gadget *Gadget,long Position,long NumGad,struct Requester *Requester);
long RemoveGList(struct Window *RemPtr,struct Gadget *Gadget,long NumGad);
void ActivateWindow(struct Window *Window);
void RefreshWindowFrame(struct Window *Window);
long ActivateGadget(struct Gadget *Gadgets,struct Window *Window,struct Requester *Req);
void NewModifyProp(struct Gadget *Gadget,struct Window *Ptr,struct Requester *Req,long Flags,long HPos,long VPos,long HBody,long VBody,long NumGad);

/************** Layers *****************************/

void InitLayers(struct Layer_Info *li);
struct Layer * CreateUpfrontLayer(struct Layer_Info *li,struct BitMap *bm,long x0,long y0,long x1,long y1,long flags,struct BitMap *bm2);
struct Layer * CreateBehindLayer(struct Layer_Info *li,struct BitMap *bm,long x0,long y0,long x1,long y1,long flags,struct BitMap *bm2);
long UpfrontLayer(struct Layer_Info *li,struct Layer *layer);
long BehindLayer(struct Layer_Info *li,struct Layer *layer);
long MoveLayer(struct Layer_Info *li,struct Layer *layer,long dx,long dy);
long SizeLayer(struct Layer_Info *li,struct Layer *layer,long dx,long dy);
void ScrollLayer(struct Layer_Info *li,struct Layer *layer,long dx,long dy);
long BeginUpdate(struct Layer *layer);
void EndUpdate(struct Layer *layer,long flag);
void DeleteLayer(struct Layer_Info *li,struct Layer *layer);
void LockLayer(struct Layer_Info *li,struct Layer *layer);
void UnlockLayer(struct Layer *layer);
void LockLayers(struct Layer_Info *li);
void UnlockLayers(struct Layer_Info *li);
void LockLayerInfo(struct Layer_Info *li);
void SwapBitsRastPortClipRect(struct RastPort *rp,struct ClipRect *cr);
struct Layer * WhichLayer(struct Layer_Info *li,long x,long y);
void UnlockLayerInfo(struct Layer_Info *li);
struct Layer_Info * NewLayerInfo(void);
void DisposeLayerInfo(struct Layer_Info *li);
void FattenLayerInfo(struct Layer_Info *li);
void ThinLayerInfo(struct Layer_Info *li);
long MoveLayerInFrontOf(struct Layer *layer_to_move,struct Layer *layer_to_be_infront_of);
struct Region * InstallClipRegion(struct Layer *layer,struct Region *region);

/************** PotGo ******************************/

long AllocPotBits(long bits);
void FreePotBits(long bits);
void WritePotgo(long word,long mask);

/************** Timer ******************************/

void AddTime(struct timeval *dest,struct timeval *src);
void SubTime(struct timeval *dest,struct timeval *src);
long CmpTime(struct timeval *dest,struct timeval *src);

/************** Translator *************************/

long Translate(const char *inputString,long inputLength,char *outputBuffer,long bufferSize);

#ifndef __NO_PRAGMAS
#include <pragmas.h>
#endif

#endif

