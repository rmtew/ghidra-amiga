
#ifdef NONDP
#define _acos acos
#define _asin asin
#define _atan atan
#define _cos cos
#define _cosh cosh
#define _cot cot
#define _exp exp
#define _fabs fabs
#define _ldexp ldexp
#define _log log
#define _log10 log10
#define _modf modf
#define _pow pow
#define _pow2 pow2
#define _sin sin
#define _sinh sinh
#define _sqrt sqrt
#define _tan tan
#define _tanh tanh
#endif
 

struct exception 
	{
	int type;		
	char *name;		
	double arg1, arg2; 	
	double retval;		
	};


#define DOMAIN    1	
#define SING      2	
#define OVERFLOW  3	
#define UNDERFLOW 4	
#define TLOSS	  5	
#define PLOSS	  6	


#define FPEUND 1	
#define FPEOVF 2	
#define FPEZDV 3	
#define FPENAN 4	
 

#ifndef PI
#define PI   3.14159265358979323846
#endif
#ifndef PID2
#define PID2 1.57079632679489661923	
#endif
#define PID4 0.78539816339744830962	
#define I_PI 0.31830988618379067154	
#define I_PID2 0.63661977236758134308	
 
#define HUGE 1.797693e308		
#define TINY 2.2e-308			
#define LOGHUGE 709.778			
#define LOGTINY -708.396		


extern int _fperr;	
extern int errno;	

extern char *ecvt();
extern short *seed48();
extern int atoi(),matherr();
extern long atol(),strtol(),lrand48(),nrand48(),mrand48(),jrand48();
extern double atof(),exp(),log(),log10(),pow(),sqrt();
extern double floor(),ceil(),fmod(),fabs(),frexp(),ldexp(),modf();
extern double sinh(),cosh(),tanh(),sin(),cos(),tan(),cot(),asin(),acos();
extern double atan(),atan2(),except();
extern double drand48(),erand48();
