extern struct Library *MathBase;
long  SPFix(float);
float SPFlt(long);
long  SPCmp(float, float);
long  SPTst(float);
float SPAbs(float);
float SPNeg(float);
float SPAdd(float, float);
float SPSub(float, float);
float SPMul(float, float);
float SPDiv(float, float);
/*----------------------------------------------------------*/
/*                                                          */
/*          New functions added for release 1.2             */
/*                                                          */
/*----------------------------------------------------------*/
float SPFloor(float);
float SPCeil(float);
#ifndef  NO_PRAGMAS
#pragma libcall MathBase SPFix 1e 1
#pragma libcall MathBase SPFlt 24 1
#pragma libcall MathBase SPCmp 2a 102
#pragma libcall MathBase SPTst 30 101
#pragma libcall MathBase SPAbs 36 1
#pragma libcall MathBase SPNeg 3c 1
#pragma libcall MathBase SPAdd 42 102
#pragma libcall MathBase SPSub 48 102
#pragma libcall MathBase SPMul 4e 102
#pragma libcall MathBase SPDiv 54 102
/*----------------------------------------------------------*/
/*                                                          */
/*          New functions added for release 1.2             */
/*                                                          */
/*----------------------------------------------------------*/
#pragma libcall MathBase SPFloor 5a 1
#pragma libcall MathBase SPCeil 60 1
#endif
