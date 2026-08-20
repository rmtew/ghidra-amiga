€ˆCLIB_GRAPHICS_PROTOS_H€CLIB_GRAPHICS_PROTOS_HˆGRAPHICS_GFX_HŒ<graphics/gfx.h>‡ˆGRAPHICS_DISPLAYINFO_HŒ<graphics/displayinfo.h>‡ˆGRAPHICS_GELS_HŒ<graphics/gels.h>‡ˆGRAPHICS_RASTPORT_HŒ<graphics/rastport.h>‡ˆGRAPHICS_VIEW_HŒ<graphics/view.h>‡ˆGRAPHICS_COPPER_HŒ<graphics/copper.h>‡ˆGRAPHICS_CLIP_HŒ<graphics/clip.h>‡ˆGRAPHICS_REGIONS_HŒ<graphics/regions.h>‡ˆGRAPHICS_SPRITE_HŒ<graphics/sprite.h>‡ˆGRAPHICS_TEXT_HŒ<graphics/text.h>‡ˆHARDWARE_BLIT_HŒ<hardware/blit.h>‡
’BltBitMap(ƒBitMap*srcBitMap,xSrc,ySrc,
ƒBitMap*destBitMap,xDest,yDest,xSize,
ySize,minterm,mask,
PLANEPTR tempA);
‹BltTemplate(PLANEPTR source,xSrc,srcMod,
ƒRastPort*destRP,xDest,yDest,xSize,
ySize);
‹ClearEOL(ƒRastPort*rp);
‹ClearScreen(ƒRastPort*rp);
˜TextLength(ƒRastPort*rp,STRPTR string,count);
’Text(ƒRastPort*rp,STRPTR string,count);
’SetFont(ƒRastPort*rp,ƒTextFont*textFont);
ƒTextFont*OpenFont(ƒTextAttr*textAttr);
‹CloseFont(ƒTextFont*textFont);
—AskSoftStyle(ƒRastPort*rp);
—SetSoftStyle(ƒRastPort*rp,style,
enable);
‹AddBob(ƒBob*bob,ƒRastPort*rp);
‹AddVSprite(ƒVSprite*vSprite,ƒRastPort*rp);
‹DoCollision(ƒRastPort*rp);
‹DrawGList(ƒRastPort*rp,ƒViewPort*vp);
‹InitGels(ƒVSprite*head,ƒVSprite*tail,
ƒGelsInfo*gelsInfo);
‹InitMasks(ƒVSprite*vSprite);
‹RemIBob(ƒBob*bob,ƒRastPort*rp,ƒViewPort*vp);
‹RemVSprite(ƒVSprite*vSprite);
‹SetCollision(num,
‹(*routine)(ƒVSprite*vSprite,”),
ƒGelsInfo*gelsInfo);
‹SortGList(ƒRastPort*rp);
‹AddAnimOb(ƒAnimOb*anOb,ƒAnimOb**anKey,
ƒRastPort*rp);
‹Animate(ƒAnimOb**anKey,ƒRastPort*rp);
BOOL GetGBuffers(ƒAnimOb*anOb,ƒRastPort*rp,flag);
‹InitGMasks(ƒAnimOb*anOb);
‹DrawEllipse(ƒRastPort*rp,xCenter,yCenter,a,
b);
’AreaEllipse(ƒRastPort*rp,xCenter,yCenter,a,
b);
‹LoadRGB4(ƒViewPort*vp,‰*colors,count);
‹InitRastPort(ƒRastPort*rp);
‹InitVPort(ƒViewPort*vp);
—MrgCop(ƒView*view);
—MakeVPort(ƒView*view,ƒViewPort*vp);
‹LoadView(ƒView*view);
‹WaitBlit(‹);
‹SetRast(ƒRastPort*rp,pen);
‹Move(ƒRastPort*rp,x,y);
‹Draw(ƒRastPort*rp,x,y);
’AreaMove(ƒRastPort*rp,x,y);
’AreaDraw(ƒRastPort*rp,x,y);
’AreaEnd(ƒRastPort*rp);
‹WaitTOF(‹);
‹QBlit(ƒbltnode*blit);
‹InitArea(ƒAreaInfo*areaInfo,”vectorBuffer,
maxVectors);
‹SetRGB4(ƒViewPort*vp,index,red,
green,blue);
‹QBSBlit(ƒbltnode*blit);
‹BltClear(PLANEPTR memBlock,byteCount,
flags);
‹RectFill(ƒRastPort*rp,xMin,yMin,xMax,
yMax);
‹BltPattern(ƒRastPort*rp,PLANEPTR mask,xMin,yMin,
xMax,yMax,maskBPR);
—ReadPixel(ƒRastPort*rp,x,y);
’WritePixel(ƒRastPort*rp,x,y);
BOOL Flood(ƒRastPort*rp,mode,x,y);
‹PolyDraw(ƒRastPort*rp,count,˜*polyTable);
‹SetAPen(ƒRastPort*rp,pen);
‹SetBPen(ƒRastPort*rp,pen);
‹SetDrMd(ƒRastPort*rp,drawMode);
‹InitView(ƒView*view);
‹CBump(ƒUCopList*copList);
‹CMove(ƒUCopList*copList,”destination,data);
‹CWait(ƒUCopList*copList,v,h);
’VBeamPos(‹);
‹InitBitMap(ƒBitMap*bitMap,depth,width,height);
‹ScrollRaster(ƒRastPort*rp,dx,dy,xMin,yMin,
xMax,yMax);
‹WaitBOVP(ƒViewPort*vp);
˜GetSprite(ƒSimpleSprite*sprite,num);
‹FreeSprite(num);
‹ChangeSprite(ƒViewPort*vp,ƒSimpleSprite*sprite,
PLANEPTR newData);
‹MoveSprite(ƒViewPort*vp,ƒSimpleSprite*sprite,x,
y);
‹LockLayerRom(ƒLayer*layer);
‹UnlockLayerRom(ƒLayer*layer);
‹SyncSBitMap(ƒLayer*layer);
‹CopySBitMap(ƒLayer*layer);
‹OwnBlitter(‹);
‹DisownBlitter(‹);
ƒTmpRas*InitTmpRas(ƒTmpRas*tmpRas,PLANEPTR buffer,
size);
‹AskFont(ƒRastPort*rp,ƒTextAttr*textAttr);
‹AddFont(ƒTextFont*textFont);
‹RemFont(ƒTextFont*textFont);
PLANEPTR AllocRaster(width,height);
‹FreeRaster(PLANEPTR p,width,height);
‹AndRectRegion(ƒRegion*region,ƒRectangle*rectangle);
BOOL OrRectRegion(ƒRegion*region,ƒRectangle*rectangle);
ƒRegion*NewRegion(‹);
BOOL ClearRectRegion(ƒRegion*region,ƒRectangle*rectangle);
‹ClearRegion(ƒRegion*region);
‹DisposeRegion(ƒRegion*region);
‹FreeVPortCopLists(ƒViewPort*vp);
‹FreeCopList(ƒCopList*copList);
‹ClipBlit(ƒRastPort*srcRP,xSrc,ySrc,
ƒRastPort*destRP,xDest,yDest,xSize,
ySize,minterm);
BOOL XorRectRegion(ƒRegion*region,ƒRectangle*rectangle);
‹FreeCprList(ƒcprlist*cprList);
ƒColorMap*GetColorMap(entries);
‹FreeColorMap(ƒColorMap*colorMap);
—GetRGB4(ƒColorMap*colorMap,entry);
‹ScrollVPort(ƒViewPort*vp);
ƒCopList*UCopperListInit(ƒUCopList*uCopList,n);
‹FreeGBuffers(ƒAnimOb*anOb,ƒRastPort*rp,flag);
‹BltBitMapRastPort(ƒBitMap*srcBitMap,xSrc,ySrc,
ƒRastPort*destRP,xDest,yDest,xSize,
ySize,minterm);
BOOL OrRegionRegion(ƒRegion*srcRegion,ƒRegion*destRegion);
BOOL XorRegionRegion(ƒRegion*srcRegion,ƒRegion*destRegion);
BOOL AndRegionRegion(ƒRegion*srcRegion,ƒRegion*destRegion);
‹SetRGB4CM(ƒColorMap*colorMap,index,red,
green,blue);
‹BltMaskBitMapRastPort(ƒBitMap*srcBitMap,xSrc,ySrc,
ƒRastPort*destRP,xDest,yDest,xSize,
ySize,minterm,PLANEPTR bltMask);
BOOL AttemptLockLayerRom(ƒLayer*layer);
”GfxNew(gfxNodeType);
‹GfxFree(”gfxNodePtr);
‹GfxAssociate(”associateNode,”gfxNodePtr);
‹BitMapScale(ƒBitScaleArgs*bitScaleArgs);
‰ScalerDiv(factor,numerator,
denominator);
˜TextExtent(ƒRastPort*rp,STRPTR string,count,
ƒTextExtent*textExtent);
—TextFit(ƒRastPort*rp,STRPTR string,strLen,
ƒTextExtent*textExtent,ƒTextExtent*constrainingExtent,
strDirection,constrainingBitWidth,
constrainingBitHeight);
”GfxLookUp(”associateNode);
BOOL VideoControl(ƒColorMap*colorMap,ƒTagItem*tagarray);
BOOL VideoControlTags(ƒColorMap*colorMap,tag1Type,
...);
ƒMonitorSpec*OpenMonitor(STRPTR monitorName,
displayID);
BOOL CloseMonitor(ƒMonitorSpec*monitorSpec);
DisplayInfoHandle FindDisplayInfo(displayID);
—NextDisplayInfo(displayID);
—GetDisplayInfoData(DisplayInfoHandle handle,Š*buf,
size,tagID,displayID);
‹FontExtent(ƒTextFont*font,ƒTextExtent*fontExtent);
’ReadPixelLine8(ƒRastPort*rp,xstart,
ystart,width,Š*array,
ƒRastPort*tempRP);
’WritePixelLine8(ƒRastPort*rp,xstart,
ystart,width,Š*array,
ƒRastPort*tempRP);
’ReadPixelArray8(ƒRastPort*rp,xstart,
ystart,xstop,ystop,
Š*array,ƒRastPort*temprp);
’WritePixelArray8(ƒRastPort*rp,xstart,
ystart,xstop,ystop,
Š*array,ƒRastPort*temprp);
’GetVPModeID(ƒViewPort*vp);
’ModeNotAvailable(modeID);
˜WeighTAMatch(ƒTextAttr*reqTextAttr,
ƒTextAttr*targetTextAttr,ƒTagItem*targetTags);
˜WeighTAMatchTags(ƒTextAttr*reqTextAttr,
ƒTextAttr*targetTextAttr,tag1Type,...);
‹EraseRect(ƒRastPort*rp,xMin,yMin,xMax,
yMax);
—ExtendFont(ƒTextFont*font,ƒTagItem*fontTags);
—ExtendFontTags(ƒTextFont*font,tag1Type,...);
‹StripFont(ƒTextFont*font);
‰CalcIVG(ƒView*v,ƒViewPort*vp);
’AttachPalExtra(ƒColorMap*cm,ƒViewPort*vp);
’ObtainBestPenA(ƒColorMap*cm,r,g,
b,ƒTagItem*tags);
’ObtainBestPen(ƒColorMap*cm,r,g,
b,tag1Type,...);
‹SetRGB32(ƒViewPort*vp,n,r,
g,b);
—GetAPen(ƒRastPort*rp);
—GetBPen(ƒRastPort*rp);
—GetDrMd(ƒRastPort*rp);
—GetOutlinePen(ƒRastPort*rp);
‹LoadRGB32(ƒViewPort*vp,—*table);
—SetChipRev(want);
‹SetABPenDrMd(ƒRastPort*rp,apen,bpen,
drawmode);
‹GetRGB32(ƒColorMap*cm,firstcolor,
ncolors,—*table);
ƒBitMap*AllocBitMap(sizex,sizey,
depth,flags,
ƒBitMap*friend_bitmap);
‹FreeBitMap(ƒBitMap*bm);
’GetExtSpriteA(ƒExtSprite*ss,ƒTagItem*tags);
’GetExtSprite(ƒExtSprite*ss,tag1Type,...);
—CoerceMode(ƒViewPort*vp,monitorid,
flags);
‹ChangeVPBitMap(ƒViewPort*vp,ƒBitMap*bm,
ƒDBufInfo*db);
‹ReleasePen(ƒColorMap*cm,n);
—ObtainPen(ƒColorMap*cm,n,r,
g,b,f);
—GetBitMapAttr(ƒBitMap*bm,attrnum);
ƒDBufInfo*AllocDBufInfo(ƒViewPort*vp);
‹FreeDBufInfo(ƒDBufInfo*dbi);
—SetOutlinePen(ƒRastPort*rp,pen);
—SetWriteMask(ƒRastPort*rp,msk);
‹SetMaxPen(ƒRastPort*rp,maxpen);
‹SetRGB32CM(ƒColorMap*cm,n,r,
g,b);
‹ScrollRasterBF(ƒRastPort*rp,dx,dy,xMin,
yMin,xMax,yMax);
’FindColor(ƒColorMap*cm,r,g,
b,maxcolor);
ƒExtSprite*AllocSpriteDataA(ƒBitMap*bm,ƒTagItem*tags);
ƒExtSprite*AllocSpriteData(ƒBitMap*bm,tag1Type,
...);
’ChangeExtSpriteA(ƒViewPort*vp,ƒExtSprite*oldsprite,
ƒExtSprite*newsprite,ƒTagItem*tags);
’ChangeExtSprite(ƒViewPort*vp,ƒExtSprite*oldsprite,
ƒExtSprite*newsprite,tag1Type,...);
‹FreeSpriteData(ƒExtSprite*sp);
‹SetRPAttrsA(ƒRastPort*rp,ƒTagItem*tags);
‹SetRPAttrs(ƒRastPort*rp,tag1Type,...);
‹GetRPAttrsA(ƒRastPort*rp,ƒTagItem*tags);
‹GetRPAttrs(ƒRastPort*rp,tag1Type,...);
—BestModeIDA(ƒTagItem*tags);
—BestModeID(tag1Type,...);
‹WriteChunkyPixels(ƒRastPort*rp,xstart,
ystart,xstop,ystop,
Š*array,bytesperrow);‡