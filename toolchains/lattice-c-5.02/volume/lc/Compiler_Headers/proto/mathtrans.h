extern struct Library *MathTransBase;
float SPAtan(float);
float SPSin(float);
float SPCos(float);
float SPTan(float);
float SPSincos(float, float*);
float SPSinh(float);
float SPCosh(float);
float SPTanh(float);
float SPExp(float);
float SPLog(float);
float SPPow(float, float);
float SPSqrt(float);
float SPTieee(float);
float SPFieee(long);
/*------------------------------------------------*/
/*                                                */
/*      New functions added for Release 1.1       */
/*                                                */
/*------------------------------------------------*/
float SPAsin(float);
float SPAcos(float);
float SPLog10(float);
#ifndef  NO_PRAGMAS
#pragma libcall MathTransBase SPAtan 1e 1
#pragma libcall MathTransBase SPSin 24 1
#pragma libcall MathTransBase SPCos 2a 1
#pragma libcall MathTransBase SPTan 30 1
#pragma libcall MathTransBase SPSincos 36 102
#pragma libcall MathTransBase SPSinh 3c 1
#pragma libcall MathTransBase SPCosh 42 1
#pragma libcall MathTransBase SPTanh 48 1
#pragma libcall MathTransBase SPExp 4e 1
#pragma libcall MathTransBase SPLog 54 1
#pragma libcall MathTransBase SPPow 5a 102
#pragma libcall MathTransBase SPSqrt 60 1
#pragma libcall MathTransBase SPTieee 66 1
#pragma libcall MathTransBase SPFieee 6c 1
/*------------------------------------------------*/
/*                                                */
/*      New functions added for Release 1.1       */
/*                                                */
/*------------------------------------------------*/
#pragma libcall MathTransBase SPAsin 72 1
#pragma libcall MathTransBase SPAcos 78 1
#pragma libcall MathTransBase SPLog10 7e 1
#endif
