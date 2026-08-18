€ˆCLIB_LOWLEVEL_PROTOS_H€CLIB_LOWLEVEL_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆEXEC_INTERRUPTS_HŒ<exec/interrupts.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆDEVICES_TIMER_HŒ<devices/timer.h>‡ˆLIBRARIES_LOWLEVEL_HŒ<libraries/lowlevel.h>‡
—ReadJoyPort(port);
ŠGetLanguageSelection(‹);
—GetKey(‹);
‹QueryKeys(ƒKeyQuery*queryArray,arraySize);
”AddKBInt(”intRoutine,”intData);
‹RemKBInt(”intHandle);
—SystemControlA(ƒTagItem*tagList);
—SystemControl(Tag firstTag,...);
”AddTimerInt(”intRoutine,”intData);
‹RemTimerInt(”intHandle);
‹StopTimerInt(”intHandle);
‹StartTimerInt(”intHandle,timeInterval,
continuous);
—ElapsedTime(ƒEClockVal*context);
”AddVBlankInt(”intRoutine,”intData);
‹RemVBlankInt(”intHandle);
BOOL SetJoyPortAttrsA(portNumber,ƒTagItem*tagList);
BOOL SetJoyPortAttrs(portNumber,Tag firstTag,...);‡