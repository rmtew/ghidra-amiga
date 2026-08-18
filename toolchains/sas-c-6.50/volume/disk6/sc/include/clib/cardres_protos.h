€ˆCLIB_CARDRES_PROTOS_H€CLIB_CARDRES_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆEXEC_RESIDENT_HŒ<exec/resident.h>‡ˆRESOURCES_CARD_HŒ<resources/card.h>‡
ƒCardHandle*OwnCard(ƒCardHandle*handle);
‹ReleaseCard(ƒCardHandle*handle,flags);
ƒCardMemoryMap*GetCardMap(‹);
BOOL BeginCardAccess(ƒCardHandle*handle);
BOOL EndCardAccess(ƒCardHandle*handle);
ŠReadCardStatus(‹);
BOOL CardResetRemove(ƒCardHandle*handle,flag);
ŠCardMiscControl(ƒCardHandle*handle,
control_bits);
—CardAccessSpeed(ƒCardHandle*handle,nanoseconds);
’CardProgramVoltage(ƒCardHandle*handle,voltage);
BOOL CardResetCard(ƒCardHandle*handle);
BOOL CopyTuple(ƒCardHandle*handle,Š*buffer,
tuplecode,size);
—DeviceTuple(Š*tuple_data,ƒDeviceTData*storage);
ƒResident*IfAmigaXIP(ƒCardHandle*handle);
BOOL CardForceChange(‹);
—CardChangeCount(‹);
—CardInterface(‹);‡