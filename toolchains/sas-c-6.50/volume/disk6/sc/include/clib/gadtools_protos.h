€ˆCLIB_GADTOOLS_PROTOS_H€CLIB_GADTOOLS_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆINTUITION_INTUITION_HŒ<intuition/intuition.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆLIBRARIES_GADTOOLS_HŒ<libraries/gadtools.h>‡
ƒ»*CreateGadgetA(kind,ƒ»*gad,
ƒNewGadget*ng,ƒTagItem*taglist);
ƒ»*CreateGadget(kind,ƒ»*gad,
ƒNewGadget*ng,Tag tag1,...);
‹FreeGadgets(ƒ»*gad);
‹GT_SetGadgetAttrsA(ƒ»*gad,ƒWindow*win,
ƒRequester*req,ƒTagItem*taglist);
‹GT_SetGadgetAttrs(ƒ»*gad,ƒWindow*win,
ƒRequester*req,Tag tag1,...);
ƒMenu*CreateMenusA(ƒNewMenu*newmenu,ƒTagItem*taglist);
ƒMenu*CreateMenus(ƒNewMenu*newmenu,Tag tag1,...);
‹FreeMenus(ƒMenu*menu);
BOOL LayoutMenuItemsA(ƒMenuItem*firstitem,”vi,
ƒTagItem*taglist);
BOOL LayoutMenuItems(ƒMenuItem*firstitem,”vi,Tag tag1,...);
BOOL LayoutMenusA(ƒMenu*firstmenu,”vi,ƒTagItem*taglist);
BOOL LayoutMenus(ƒMenu*firstmenu,”vi,Tag tag1,...);
ƒIntuiMessage*GT_GetIMsg(ƒ©*iport);
‹GT_ReplyIMsg(ƒIntuiMessage*imsg);
‹GT_RefreshWindow(ƒWindow*win,ƒRequester*req);
‹GT_BeginRefresh(ƒWindow*win);
‹GT_EndRefresh(ƒWindow*win,complete);
ƒIntuiMessage*GT_FilterIMsg(ƒIntuiMessage*imsg);
ƒIntuiMessage*GT_PostFilterIMsg(ƒIntuiMessage*imsg);
ƒ»*CreateContext(ƒ»**glistptr);
‹DrawBevelBoxA(ƒRastPort*rport,left,top,width,
height,ƒTagItem*taglist);
‹DrawBevelBox(ƒRastPort*rport,left,top,width,
height,Tag tag1,...);
”GetVisualInfoA(ƒScreen*screen,ƒTagItem*taglist);
”GetVisualInfo(ƒScreen*screen,Tag tag1,...);
‹FreeVisualInfo(”vi);
’GT_GetGadgetAttrsA(ƒ»*gad,ƒWindow*win,
ƒRequester*req,ƒTagItem*taglist);
’GT_GetGadgetAttrs(ƒ»*gad,ƒWindow*win,
ƒRequester*req,Tag tag1,...);‡