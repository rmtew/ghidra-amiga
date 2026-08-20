€ˆINTUITION_CGHOOKS_H€INTUITION_CGHOOKS_H 1ˆEXEC_TYPES_HŒ<exec/types.h>‡ˆINTUITION_INTUITION_HŒ<intuition/intuition.h>‡
ƒGadgetInfo{
ƒScreen*gi_Screen;
ƒWindow*gi_Window;
ƒRequester*gi_Requester;
ƒRastPort*gi_RastPort;
ƒLayer*gi_Layer;
ƒIBox gi_Domain;
ƒ{
ŠDetailPen;
ŠBlockPen;
}gi_Pens;
ƒDrawInfo*gi_DrInfo;
—gi_Reserved[6];
};
ƒPGX{
ƒIBox pgx_Container;
ƒIBox pgx_NewKnob;
};€CUSTOM_HOOK(gadget) ((ƒHook*)(gadget)->MutualExclude)‡