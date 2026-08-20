€ˆ·€·ˆµŒ"exec/nodes.h"‡
ƒ®{
ƒ¬*lh_Head;
ƒ¬*lh_Tail;
ƒ¬*lh_TailPred;
Šlh_Type;
Šl_pad;
};
ƒMinList{
ƒMinNode*mlh_Head;
ƒMinNode*mlh_Tail;
ƒMinNode*mlh_TailPred;
};€IsListEmpty(x) \
(((x)->lh_TailPred)==(ƒ¬*)(x))€IsMsgPortEmpty(x) \
(((x)->mp_MsgList.lh_TailPred)==(ƒ¬*)(&(x)->mp_MsgList))‡