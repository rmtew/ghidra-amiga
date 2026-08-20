extern struct Library *MathIeeeSingBasBase;
float IEEESPFix(float);
float IEEESPFlt(long);
long IEEESPCmp(float, float);
long IEEESPTst(float);
float IEEESPAbs(float);
float IEEESPNeg(float);
float IEEESPAdd(float, float);
float IEEESPSub(float, float);
float IEEESPMul(float, float);
float IEEESPDiv(float, float);
#ifndef  NO_PRAGMAS
#pragma libcall MathIeeeSingBasBase IEEESPFix 1e 1
#pragma libcall MathIeeeSingBasBase IEEESPFlt 24 1
#pragma libcall MathIeeeSingBasBase IEEESPCmp 2a 1002
#pragma libcall MathIeeeSingBasBase IEEESPTst 30 1
#pragma libcall MathIeeeSingBasBase IEEESPAbs 36 1
#pragma libcall MathIeeeSingBasBase IEEESPNeg 3c 1
#pragma libcall MathIeeeSingBasBase IEEESPAdd 42 1002
#pragma libcall MathIeeeSingBasBase IEEESPSub 48 1002
#pragma libcall MathIeeeSingBasBase IEEESPMul 4e 1002
#pragma libcall MathIeeeSingBasBase IEEESPDiv 54 1002
#endif
