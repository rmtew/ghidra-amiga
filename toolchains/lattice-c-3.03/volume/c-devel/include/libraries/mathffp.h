#ifndef  LIBRARIES_MATHFFP_H
#define  LIBRARIES_MATHFFP_H






#ifndef PI
#define PI        ((FLOAT) 3.1415192653857)
#endif
#define TWO_PI    (((FLOAT) 2) * PI)
#define PI2       (PI / ((FLOAT) 2))
#define PI4       (PI / ((FLOAT) 4))
#ifndef E
#define E         ((FLOAT) 2.7182818284590453)
#endif
#define LOG10     ((FLOAT) 2.3025850929940456)

#define FPTEN     ((FLOAT) 10.0)
#define FPONE     ((FLOAT) 1.0)
#define FPHALF    ((FLOAT) 0.5)
#define FPZERO    ((FLOAT) 0.0)

#define trunc(x)  ((int) (x))
#define round(x)  ((int) ((x) + 0.5))
#define itof(i)   ((FLOAT) (i))

int      SPFix();                         
FLOAT    SPFlt();
int      SPCmp();
int      SPTst();
FLOAT    SPAbs();
#ifndef abs
FLOAT    abs();
#endif
FLOAT    SPNeg();
FLOAT    SPAdd();
FLOAT    SPSub();
FLOAT    SPMul();
FLOAT    SPDiv();

FLOAT    SPAsin(),  SPAcos(),  SPAtan();  
FLOAT    SPSin(),   SPCos(),   SPTan(),   SPSincos();
FLOAT    SPSinh(),  SPCosh(),  SPTanh();
FLOAT    SPExp(),   SPLog(),   SPLog10(), SPPow();
FLOAT    SPSqrt(),  SPFieee();

FLOAT    afp(),    dbf();                 

#endif   !LIBRARIES_MATHFFP_H
