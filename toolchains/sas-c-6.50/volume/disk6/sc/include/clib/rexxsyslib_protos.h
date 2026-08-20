€ˆCLIB_REXXSYSLIB_PROTOS_H€CLIB_REXXSYSLIB_PROTOS_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆREXX_RXSLIB_HŒ<rexx/rxslib.h>‡ˆREXX_REXXIO_HŒ<rexx/rexxio.h>‡
Š*CreateArgstring(Š*string,length);
‹DeleteArgstring(Š*argstring);
—LengthArgstring(Š*argstring);
ƒRexxMsg*CreateRexxMsg(ƒ©*port,Š*extension,
Š*host);
‹DeleteRexxMsg(ƒRexxMsg*packet);
‹ClearRexxMsg(ƒRexxMsg*msgptr,count);
BOOL FillRexxMsg(ƒRexxMsg*msgptr,count,
mask);
BOOL IsRexxMsg(ƒRexxMsg*msgptr);
‹LockRexxBase(resource);
‹UnlockRexxBase(resource);‡