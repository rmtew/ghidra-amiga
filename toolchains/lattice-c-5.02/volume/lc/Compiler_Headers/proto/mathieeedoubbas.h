extern struct Library *MathIeeeDoubBasBase;
long   IEEEDPFix(double);
double IEEEDPFlt(long);
long   IEEEDPCmp(double, double);
long   IEEEDPTst(double);
double IEEEDPAbs(double);
double IEEEDPNeg(double);
double IEEEDPAdd(double, double);
double IEEEDPSub(double, double);
double IEEEDPMul(double, double);
double IEEEDPDiv(double, double);
/*--------------------------------------------------------------------*/
/*                                                                    */
/*             New functions added for release 1.2                    */
/*                                                                    */
/*--------------------------------------------------------------------*/
double IEEEDPFloor(double);
double IEEEDPCeil(double);
#ifndef  NO_PRAGMAS
#pragma libcall MathIeeeDoubBasBase IEEEDPFix 1e 1
#pragma libcall MathIeeeDoubBasBase IEEEDPFlt 24 1
#pragma libcall MathIeeeDoubBasBase IEEEDPCmp 2a 2002
#pragma libcall MathIeeeDoubBasBase IEEEDPTst 30 1
#pragma libcall MathIeeeDoubBasBase IEEEDPAbs 36 1
#pragma libcall MathIeeeDoubBasBase IEEEDPNeg 3c 1
#pragma libcall MathIeeeDoubBasBase IEEEDPAdd 42 2002
#pragma libcall MathIeeeDoubBasBase IEEEDPSub 48 2002
#pragma libcall MathIeeeDoubBasBase IEEEDPMul 4e 2002
#pragma libcall MathIeeeDoubBasBase IEEEDPDiv 54 2002
/*--------------------------------------------------------------------*/
/*                                                                    */
/*             New functions added for release 1.2                    */
/*                                                                    */
/*--------------------------------------------------------------------*/
#pragma libcall MathIeeeDoubBasBase IEEEDPFloor 5a 1
#pragma libcall MathIeeeDoubBasBase IEEEDPCeil 60 1
#endif
