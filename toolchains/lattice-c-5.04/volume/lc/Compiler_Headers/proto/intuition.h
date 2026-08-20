/*  $Header: intuition_lib.fd,v 1.8 86/09/06 17:00:51 jimm Exp $*/
/**/
#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif
extern struct IntuitionBase *IntuitionBase;
void OpenIntuition(void);
void Intuition(struct InputEvent);
long AddGadget(struct Window *, struct Gadget *, long);
long ClearDMRequest(struct Window *);
void ClearMenuStrip(struct Window *);
void ClearPointer(struct Window *);
void CloseScreen(struct Screen *);
void CloseWindow(struct Window *);
long CloseWorkBench(void);
void CurrentTime(long *, long *);
long DisplayAlert(long, char *, long);
void DisplayBeep(struct Screen *);
long DoubleClick(long, long, long, long);
void DrawBorder(struct RastPort *, struct Border *, long, long);
void DrawImage(struct RastPort *, struct Image *, long, long);
void EndRequest(struct Requester *, struct Window *);
struct Preferences *GetDefPrefs(struct Preferences *, long);
struct Preferences *GetPrefs(struct Preferences *, long);
void InitRequester(struct Requester *);
struct MenuItem *ItemAddress(struct Menu *, long);
void ModifyIDCMP(struct Window *, long);
void ModifyProp(struct Gadget *, struct Window *, struct Requester *, long, long, long, long, long);
void MoveScreen(struct Screen *, long, long);
void MoveWindow(struct Window *, long, long);
void OffGadget(struct Gadget *, struct Window *, struct Requester *);
void OffMenu(struct Window *, long);
void OnGadget(struct Gadget *, struct Window *, struct Requester *);
void OnMenu(struct Window *, long);
struct Screen *OpenScreen(struct NewScreen *);
struct Window *OpenWindow(struct NewWindow *);
long OpenWorkBench(void);
void PrintIText(struct RastPort *, struct IntuiText *, long, long);
void RefreshGadgets(struct Gadget *, struct Window *, struct Requester *);
long RemoveGadget(struct Window *, struct Gadget *);
void ReportMouse(struct Window *, long);
long Request(struct Requester *, struct Window *);
void ScreenToBack(struct Screen *);
void ScreenToFront(struct Screen *);
long SetDMRequest(struct Window *, struct Requester *);
void SetMenuStrip(struct Window *, struct Menu *);
void SetPointer(struct Window *, short *, long, long, long, long);
void SetWindowTitles(struct Window *, char *, char *);
void ShowTitle(struct Screen *, long);
void SizeWindow(struct Window *, long, long);
struct View *ViewAddress(void);
struct ViewPort *ViewPortAddress(struct Window *);
void WindowToBack(struct Window *);
void WindowToFront(struct Window *);
long WindowLimits(struct Window *, long, long, long, long);
/*--- start of next generation of names ---------------------------------------*/
void SetPrefs(struct Preferences *, long, long);
/*--- start of next next generation of names ----------------------------------*/
long IntuiTextLength(struct IntuiText *);
long WBenchToBack(void);
long WBenchToFront(void);
/*--- start of next next next generation of names -----------------------------*/
long AutoRequest(struct Window *, struct IntuiText *, struct IntuiText *, struct IntuiText *, long, long, long, long);
void BeginRefresh(struct Window *);
struct Window *BuildSysRequest(struct Window *, struct IntuiText *, struct IntuiText *, struct IntuiText *, long, long, long);
void EndRefresh(struct Window *, long);
void FreeSysRequest(struct Window *);
void MakeScreen(struct Screen *);
void RemakeDisplay(void);
void RethinkDisplay(void);
/*--- start of next next next next generation of names ------------------------*/
char *AllocRemember(struct Remember **, long, long);
void AlohaWorkbench(long);
void FreeRemember(struct Remember **, long);
/*--- start of 15 Nov 85 names ------------------------*/
long  LockIBase(long);
void UnlockIBase(long);
/*--- start of post-1.1 names ----*/
long GetScreenData(char *, long, long, struct Screen *);
void RefreshGList(struct Gadget *, struct Window *, struct Requester *, long);
long AddGList(struct Window *, struct Gadget *, long, long, struct Requester *);
long RemoveGList(struct Window *, struct Gadget *, long);
void ActivateWindow(struct Window *);
void RefreshWindowFrame(struct Window *);
long ActivateGadget(struct Gadget *, struct Window *, struct Requester *);
void NewModifyProp(struct Gadget *, struct Window *, struct Requester *, long, long, long, long, long, long);
#ifndef  NO_PRAGMAS
#pragma libcall IntuitionBase OpenIntuition 1e 0
#pragma libcall IntuitionBase Intuition 24 801
#pragma libcall IntuitionBase AddGadget 2a 9803
#pragma libcall IntuitionBase ClearDMRequest 30 801
#pragma libcall IntuitionBase ClearMenuStrip 36 801
#pragma libcall IntuitionBase ClearPointer 3c 801
#pragma libcall IntuitionBase CloseScreen 42 801
#pragma libcall IntuitionBase CloseWindow 48 801
#pragma libcall IntuitionBase CloseWorkBench 4e 0
#pragma libcall IntuitionBase CurrentTime 54 9802
#pragma libcall IntuitionBase DisplayAlert 5a 18003
#pragma libcall IntuitionBase DisplayBeep 60 801
#pragma libcall IntuitionBase DoubleClick 66 321004
#pragma libcall IntuitionBase DrawBorder 6c 109804
#pragma libcall IntuitionBase DrawImage 72 109804
#pragma libcall IntuitionBase EndRequest 78 9802
#pragma libcall IntuitionBase GetDefPrefs 7e 802
#pragma libcall IntuitionBase GetPrefs 84 802
#pragma libcall IntuitionBase InitRequester 8a 801
#pragma libcall IntuitionBase ItemAddress 90 802
#pragma libcall IntuitionBase ModifyIDCMP 96 802
/*pragma libcall IntuitionBase ModifyProp 9c 210a9808*/
#pragma libcall IntuitionBase MoveScreen a2 10803
#pragma libcall IntuitionBase MoveWindow a8 10803
#pragma libcall IntuitionBase OffGadget ae a9803
#pragma libcall IntuitionBase OffMenu b4 802
#pragma libcall IntuitionBase OnGadget ba a9803
#pragma libcall IntuitionBase OnMenu c0 802
#pragma libcall IntuitionBase OpenScreen c6 801
#pragma libcall IntuitionBase OpenWindow cc 801
#pragma libcall IntuitionBase OpenWorkBench d2 0
#pragma libcall IntuitionBase PrintIText d8 109804
#pragma libcall IntuitionBase RefreshGadgets de a9803
#pragma libcall IntuitionBase RemoveGadget e4 9802
#pragma libcall IntuitionBase ReportMouse ea 802
#pragma libcall IntuitionBase Request f0 9802
#pragma libcall IntuitionBase ScreenToBack f6 801
#pragma libcall IntuitionBase ScreenToFront fc 801
#pragma libcall IntuitionBase SetDMRequest 102 9802
#pragma libcall IntuitionBase SetMenuStrip 108 9802
#pragma libcall IntuitionBase SetPointer 10e 32109806
#pragma libcall IntuitionBase SetWindowTitles 114 a9803
#pragma libcall IntuitionBase ShowTitle 11a 802
#pragma libcall IntuitionBase SizeWindow 120 10803
#pragma libcall IntuitionBase ViewAddress 126 0
#pragma libcall IntuitionBase ViewPortAddress 12c 801
#pragma libcall IntuitionBase WindowToBack 132 801
#pragma libcall IntuitionBase WindowToFront 138 801
#pragma libcall IntuitionBase WindowLimits 13e 3210805
/*--- start of next generation of names ---------------------------------------*/
#pragma libcall IntuitionBase SetPrefs 144 10803
/*--- start of next next generation of names ----------------------------------*/
#pragma libcall IntuitionBase IntuiTextLength 14a 801
#pragma libcall IntuitionBase WBenchToBack 150 0
#pragma libcall IntuitionBase WBenchToFront 156 0
/*--- start of next next next generation of names -----------------------------*/
/*pragma libcall IntuitionBase AutoRequest 15c 10ba9808*/
#pragma libcall IntuitionBase BeginRefresh 162 801
/*pragma libcall IntuitionBase BuildSysRequest 168 10ba9807*/
#pragma libcall IntuitionBase EndRefresh 16e 802
#pragma libcall IntuitionBase FreeSysRequest 174 801
#pragma libcall IntuitionBase MakeScreen 17a 801
#pragma libcall IntuitionBase RemakeDisplay 180 0
#pragma libcall IntuitionBase RethinkDisplay 186 0
/*--- start of next next next next generation of names ------------------------*/
#pragma libcall IntuitionBase AllocRemember 18c 10803
#pragma libcall IntuitionBase AlohaWorkbench 192 801
#pragma libcall IntuitionBase FreeRemember 198 802
/* PointerColors(Screen,Red,Gren,Blue)(A0,D0,D1,D2)*/
/*--- start of 15 Nov 85 names ------------------------*/
#pragma libcall IntuitionBase LockIBase 19e 1
#pragma libcall IntuitionBase UnlockIBase 1a4 801
/*--- start of post-1.1 names ----*/
#pragma libcall IntuitionBase GetScreenData 1aa 910804
#pragma libcall IntuitionBase RefreshGList 1b0 a9804
#pragma libcall IntuitionBase AddGList 1b6 a109805
#pragma libcall IntuitionBase RemoveGList 1bc 9803
#pragma libcall IntuitionBase ActivateWindow 1c2 801
#pragma libcall IntuitionBase RefreshWindowFrame 1c8 801
#pragma libcall IntuitionBase ActivateGadget 1ce a9803
/*pragma libcall IntuitionBase NewModifyProp 1d4 210a9809*/
#endif
