#ifndef EXEC_ALERTS_H
#define EXEC_ALERTS_H


#define SF_ALERTWACK (1<<1)   







#define AT_DeadEnd   0x80000000
#define AT_Recovery  0x00000000


#define AG_NoMemory     0x00010000
#define AG_MakeLib      0x00020000
#define AG_OpenLib      0x00030000
#define AG_OpenDev      0x00040000
#define AG_OpenRes      0x00050000
#define AG_IOError      0x00060000


#define AO_ExecLib      0x00008001
#define AO_GraphicsLib  0x00008002
#define AO_LayersLib    0x00008003
#define AO_Intuition    0x00008004
#define AO_MathLib      0x00008005
#define AO_CListLib     0x00008006
#define AO_DOSLib       0x00008007
#define AO_RAMLib       0x00008008
#define AO_IconLib      0x00008009
#define AO_AudioDev     0x00008010
#define AO_ConsoleDev   0x00008011
#define AO_GamePortDev  0x00008012
#define AO_KeyboardDev  0x00008013
#define AO_TrackDiskDev 0x00008014
#define AO_TimerDev     0x00008015
#define AO_CIARsrc      0x00008020
#define AO_DiskRsrc     0x00008021
#define AO_MiscRsrc     0x00008022
#define AO_BootStrap    0x00008030
#define AO_Workbench    0x00008031





#define AN_ExecLib      0x01000000
#define AN_ExcptVect    0x81000001  
#define AN_BaseChkSum   0x81000002  
#define AN_LibChkSum    0x81000003  
#define AN_LibMem       0x81000004  
#define AN_MemCorrupt   0x81000005  
#define AN_IntrMem      0x81000006  
#define  AN_InitAPtr    0x81000007  


#define AN_GraphicsLib  0x02000000
#define AN_CopDisplay   0x82010001  
#define AN_CopInstr     0x82010002  
#define AN_CopListOver  0x82000003  
#define AN_CopIListOver 0x82000004  
#define AN_CopListHead  0x82010005  
#define AN_LongFrame    0x82010006  
#define AN_ShortFrame   0x82010007  
#define AN_FloodFill    0x82010008  
#define AN_TextTmpRas   0x02010009  
#define AN_BltBitMap    0x8201000A  


#define AN_LayersLib    0x03000000


#define AN_Intuition    0x04000000
#define AN_GadgetType   0x84000001  
#define AN_BadGadget    0x04000001  
#define AN_CreatePort   0x84010002  
#define AN_ItemAlloc    0x04010003  
#define AN_SubAlloc     0x04010004  
#define AN_PlaneAlloc   0x84010005  
#define AN_ItemBoxTop   0x84000006  
#define AN_OpenScreen   0x84010007  
#define AN_OpenScrnRast 0x84010008  
#define AN_SysScrnType  0x84000009  
#define AN_AddSWGadget  0x8401000A  
#define AN_OpenWindow   0x8401000B  
#define  AN_BadState    0x8400000C  
#define  AN_BadMessage  0x8400000D  
#define  AN_WeirdEcho   0x8400000E  
#define  AN_NoConsole   0x8400000F  



#define AN_MathLib      0x05000000


#define AN_CListLib     0x06000000


#define AN_DOSLib       0x07000000
#define AN_StartMem     0x07010001  
#define AN_EndTask      0x07000002  
#define AN_QPktFail     0x07000003  
#define AN_AsyncPkt     0x07000004  
#define AN_FreeVec      0x07000005  
#define AN_DiskBlkSeq   0x07000006  
#define AN_BitMap       0x07000007  
#define AN_KeyFree      0x07000008  
#define AN_BadChkSum    0x07000009  
#define AN_DiskError    0x0700000A  
#define AN_KeyRange     0x0700000B  
#define AN_BadOverlay   0x0700000C  


#define AN_RAMLib       0x08000000


#define AN_IconLib      0x09000000


#define AN_AudioDev     0x10000000


#define AN_ConsoleDev   0x11000000


#define AN_GamePortDev  0x12000000


#define AN_KeyboardDev  0x13000000


#define AN_TrackDiskDev 0x14000000
#define AN_TDCalibSeek  0x14000001  
#define AN_TDDelay      0x14000002  


#define AN_TimerDev     0x15000000
#define AN_TMBadReq     0x15000001  


#define AN_CIARsrc      0x20000000


#define AN_DiskRsrc     0x21000000
#define AN_DRHasDisk    0x21000001  
#define AN_DRIntNoAct   0x21000002  


#define AN_MiscRsrc     0x22000000


#define AN_BootStrap    0x30000000
#define AN_BootError    0x30000001  


#define AN_Workbench    0x31000000

#endif !EXEC_ALERTS_H
