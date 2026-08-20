€ˆINTUITION_CLASSES_H€INTUITION_CLASSES_H 1ˆUTILITY_HOOKS_HŒ<utility/hooks.h>‡ˆINTUITION_CLASSUSR_HŒ<intuition/classusr.h>‡
¥ƒIClass{
ƒHook cl_Dispatcher;
—cl_Reserved;
ƒIClass*cl_Super;
ClassID cl_ID;
‰cl_InstOffset;
‰cl_InstSize;
—cl_UserData;
—cl_SubclassCount;
—cl_ObjectCount;
—cl_Flags;€CLF_INLIST 1
}Class;€INST_DATA(cl,o) ((VOID*)(((Š*)o)+cl->cl_InstOffset))€SIZEOF_INSTANCE(cl) ((cl)->cl_InstOffset+(cl)->cl_InstSize\
+sizeof(ƒ_Object))
ƒ_Object{
ƒMinNode o_Node;
ƒIClass*o_Class;
};€_OBJ(o) ((ƒ_Object*)(o))€BASEOBJECT(_obj) ((Object*)(_OBJ(_obj)+1))€_OBJECT(o) (_OBJ(o)-1)€OCLASS(o) ((_OBJECT(o))->o_Class)‡