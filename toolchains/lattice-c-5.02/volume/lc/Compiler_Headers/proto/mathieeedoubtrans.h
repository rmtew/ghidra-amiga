extern struct Library *MathIeeeDoubTransBase;
double IEEEDPAtan(double);
double IEEEDPSin(double);
double IEEEDPCos(double);
double IEEEDPTan(double);
double IEEEDPSincos(double, double*);
double IEEEDPSinh(double);
double IEEEDPCosh(double);
double IEEEDPTanh(double);
double IEEEDPExp(double);
double IEEEDPLog(double);
double IEEEDPPow(double, double);
double IEEEDPSqrt(double);
double IEEEDPTieee(double);
double IEEEDPFieee(long);
/*------------------------------------------------*/
/*                                                */
/*      New functions added for Release 1.1       */
/*                                                */
/*------------------------------------------------*/
double IEEEDPAsin(double);
double IEEEDPAcos(double);
double IEEEDPLog10(double);
#ifndef  NO_PRAGMAS
#pragma libcall MathIeeeDoubTransBase IEEEDPAtan 1e 1
#pragma libcall MathIeeeDoubTransBase IEEEDPSin 24 1
#pragma libcall MathIeeeDoubTransBase IEEEDPCos 2a 1
#pragma libcall MathIeeeDoubTransBase IEEEDPTan 30 1
#pragma libcall MathIeeeDoubTransBase IEEEDPSincos 36 802
#pragma libcall MathIeeeDoubTransBase IEEEDPSinh 3c 1
#pragma libcall MathIeeeDoubTransBase IEEEDPCosh 42 1
#pragma libcall MathIeeeDoubTransBase IEEEDPTanh 48 1
#pragma libcall MathIeeeDoubTransBase IEEEDPExp 4e 1
#pragma libcall MathIeeeDoubTransBase IEEEDPLog 54 1
#pragma libcall MathIeeeDoubTransBase IEEEDPPow 5a 202
#pragma libcall MathIeeeDoubTransBase IEEEDPSqrt 60 1
#pragma libcall MathIeeeDoubTransBase IEEEDPTieee 66 1
#pragma libcall MathIeeeDoubTransBase IEEEDPFieee 6c 1
/*------------------------------------------------*/
/*                                                */
/*      New functions added for Release 1.1       */
/*                                                */
/*------------------------------------------------*/
#pragma libcall MathIeeeDoubTransBase IEEEDPAsin 72 1
#pragma libcall MathIeeeDoubTransBase IEEEDPAcos 78 1
#pragma libcall MathIeeeDoubTransBase IEEEDPLog10 7e 1
#endif
