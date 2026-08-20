€ˆINTUITION_CLASSUSR_H€INTUITION_CLASSUSR_H 1ˆUTILITY_HOOKS_HŒ<utility/hooks.h>‡
¥—Object;
¥Š*ClassID;
¥ƒ{
—MethodID;
}*Msg;€ROOTCLASS "rootclass"€IMAGECLASS "imageclass"€FRAMEICLASS "frameiclass"€SYSICLASS "sysiclass"€FILLRECTCLASS "fillrectclass"€GADGETCLASS "gadgetclass"€PROPGCLASS "propgclass"€STRGCLASS "strgclass"€BUTTONGCLASS "buttongclass"€FRBUTTONCLASS "frbuttonclass"€GROUPGCLASS "groupgclass"€ICCLASS "icclass"€MODELCLASS "modelclass"€ITEXTICLASS "itexticlass"€POINTERCLASS "pointerclass"€OM_Dummy (0x100)€OM_NEW (0x101)€OM_DISPOSE (0x102)€OM_SET (0x103)€OM_GET (0x104)€OM_ADDTAIL (0x105)€OM_REMOVE (0x106)€OM_NOTIFY (0x107)€OM_UPDATE (0x108)€OM_ADDMEMBER (0x109)€OM_REMMEMBER (0x10A)
ƒopSet{
—MethodID;
ƒTagItem*ops_AttrList;
ƒGadgetInfo*ops_GInfo;
};
ƒopUpdate{
—MethodID;
ƒTagItem*opu_AttrList;
ƒGadgetInfo*opu_GInfo;
—opu_Flags;
};€OPUF_INTERIM (1<<0)
ƒopGet{
—MethodID;
—opg_AttrID;
—*opg_Storage;
};
ƒopAddTail{
—MethodID;
ƒ®*opat_List;
};€opAddMember opMember
ƒopMember{
—MethodID;
Object*opam_Object;
};‡