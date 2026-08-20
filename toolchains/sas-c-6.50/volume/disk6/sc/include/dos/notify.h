€ˆDOS_NOTIFY_H€DOS_NOTIFY_HˆEXEC_TYPES_HŒ"exec/types.h"‡ˆ¸Œ"exec/ports.h"‡ˆEXEC_TASKS_HŒ"exec/tasks.h"‡€NOTIFY_CLASS 0x40000000€NOTIFY_CODE 0x1234
ƒNotifyMessage{
ƒ¯nm_ExecMessage;
—nm_Class;
‰nm_Code;
ƒNotifyRequest*nm_NReq;
—nm_DoNotTouch;
—nm_DoNotTouch2;
};
ƒNotifyRequest{
Š*nr_Name;
Š*nr_FullName;
—nr_UserData;
—nr_Flags;
«{
ƒ{
ƒ©*nr_Port;
}nr_Msg;
ƒ{
ƒTask*nr_Task;
Šnr_SignalNum;
Šnr_pad[3];
}nr_Signal;
}nr_stuff;
—nr_Reserved[4];
—nr_MsgCount;
ƒ©*nr_Handler;
};€NRF_SEND_MESSAGE 1€NRF_SEND_SIGNAL 2€NRF_WAIT_REPLY 8€NRF_NOTIFY_INITIAL 16€NRF_MAGIC 0x80000000€NRB_SEND_MESSAGE 0€NRB_SEND_SIGNAL 1€NRB_WAIT_REPLY 3€NRB_NOTIFY_INITIAL 4€NRB_MAGIC 31€NR_HANDLER_FLAGS 0xffff0000‡