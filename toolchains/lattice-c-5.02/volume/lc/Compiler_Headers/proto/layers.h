#ifndef GRAPHICS_LAYERS_H
#include <graphics/layers.h>
#endif
extern struct LayersBase *LayersBase;
/*layerlib routines*/
void InitLayers(struct Layer_Info *);
struct Layer *CreateUpfrontLayer(struct Layer_Info *, struct BitMap *, long, long, long, long, long, struct BitMap *);
struct Layer *CreateBehindLayer(struct Layer_Info *, struct BitMap *, long, long, long, long, long, struct BitMap *);
long UpfrontLayer(struct Layer_Info *, struct Layer *);
long BehindLayer(struct Layer_Info *, struct Layer *);
void MoveLayer(struct Layer_Info *, struct Layer *, long, long);
void SizeLayer(struct Layer_Info *, struct Layer *, long, long);
void ScrollLayer(struct Layer_Info *, struct Layer *, long, long);
void BeginUpdate(struct Layer *);
void EndUpdate(struct Layer *, long);
void DeleteLayer(struct Layer_Info *, struct Layer *);
void LockLayer(struct Layer_Info *, struct Layer *);
void UnlockLayer(struct Layer *);
void LockLayers(struct Layer_Info *);
void UnlockLayers(struct Layer_Info *);
void LockLayerInfo(struct Layer_Info *);
void SwapBitsRastPortClipRect(struct RastPort *, struct ClipRect *);
struct Layer *WhichLayer(struct Layer_Info *, long, long);
void UnlockLayerInfo(struct Layer_Info *);
struct Layer_Info *NewLayerInfo(void);
void DisposeLayerInfo(struct Layer_Info *);
void FattenLayerInfo(struct Layer_Info *);
void ThinLayerInfo(struct Layer_Info *);
long MoveLayerInFrontOf(struct Layer *, struct Layer *);
struct Region *InstallClipRegion(struct Layer *, struct Region *);
#ifndef  NO_PRAGMAS
/*layerlib routines*/
#pragma libcall LayersBase InitLayers 1e 801
/*pragma libcall LayersBase CreateUpfrontLayer 24 32109808*/
/*pragma libcall LayersBase CreateBehindLayer 2a 32109808*/
#pragma libcall LayersBase UpfrontLayer 30 9802
#pragma libcall LayersBase BehindLayer 36 9802
#pragma libcall LayersBase MoveLayer 3c 109804
#pragma libcall LayersBase SizeLayer 42 109804
#pragma libcall LayersBase ScrollLayer 48 109804
#pragma libcall LayersBase BeginUpdate 4e 801
#pragma libcall LayersBase EndUpdate 54 802
#pragma libcall LayersBase DeleteLayer 5a 9802
#pragma libcall LayersBase LockLayer 60 9802
#pragma libcall LayersBase UnlockLayer 66 801
#pragma libcall LayersBase LockLayers 6c 801
#pragma libcall LayersBase UnlockLayers 72 801
#pragma libcall LayersBase LockLayerInfo 78 801
#pragma libcall LayersBase SwapBitsRastPortClipRect 7e 9802
#pragma libcall LayersBase WhichLayer 84 10803
#pragma libcall LayersBase UnlockLayerInfo 8a 801
#pragma libcall LayersBase NewLayerInfo 90 0
#pragma libcall LayersBase DisposeLayerInfo 96 801
#pragma libcall LayersBase FattenLayerInfo 9c 801
#pragma libcall LayersBase ThinLayerInfo a2 801
#pragma libcall LayersBase MoveLayerInFrontOf a8 9802
#pragma libcall LayersBase InstallClipRegion ae 9802
#endif
