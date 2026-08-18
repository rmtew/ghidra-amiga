#ifndef	LIBRARIES_MATHFFP_H
#define	LIBRARIES_MATHFFP_H
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif
#ifndef	PI
#define	PI	((float)	3.141592653589793)
#endif
#define	TWO_PI	(((float)	2)	*	PI)
#define	PI2	(PI/	((float)	2))
#define	PI4	(PI/	((float)	4))
#ifndef	E
#define	E	((float)	2.718281828459045)
#endif
#define	LOG10	((float)	2.302585092994046)
#define	FPTEN	((float)	10.0)
#define	FPONE	((float)	1.0)
#define	FPHALF	((float)	0.5)
#define	FPZERO	((float)	0.0)
#define	trunc(x)	((int)	(x))
#define	round(x)	((int)	((x)	+	0.5))
#define	itof(i)	((float)	(i))
#define	fabs	SPAbs
#define	floor	SPFloor
#define	ceil	SPCeil
#define	tan	SPTan
#define	atan	SPAtan
#define	cos	SPCos
#define	acos	SPAcos
#define	sin	SPSin
#define	asin	SPAsin
#define	exp	SPExp
#define	pow(a,b)	SPPow((b),(a))
#define	log	SPLog
#define	log10	SPLog10
#define	sqrt	SPSqrt
#define	sinh	SPSinh
#define	cosh	SPCosh
#define	tanh	SPTanh
int	SPFix();
float	SPFlt();
int	SPCmp();
int	SPTst();
float	SPAbs();
float	SPFloor();
float	SPCeil();
#ifndef	abs
float	abs();
#endif
float	SPNeg();
float	SPAdd();
float	SPSub();
float	SPMul();
float	SPDiv();
float	SPAsin(),	SPAcos(),	SPAtan();
float	SPSin(),	SPCos(),	SPTan(),	SPSincos();
float	SPSinh(),	SPCosh(),	SPTanh();
float	SPExp(),	SPLog(),	SPLog10(),	SPPow();
float	SPSqrt(),	SPFieee();
float	afp(),	dbf();
#endif
