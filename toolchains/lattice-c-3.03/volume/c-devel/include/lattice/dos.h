


#define LATTICE 1




#if LATTICE
typedef char byte;
#endif

#if BDS
#define byte char
#endif

#if BTL
typedef unsigned char byte;
#endif

#if MANX
#define byte char
#endif


#define SECSIZ 128		


struct FCB
	{
	char fcbdrv;		
	char fcbnam[8];		
	char fcbext[3];		
	char fcbexn;		
	char fcbs1;		
	char fcbs2;		
	char fcbrc;		
	char fcbsys[16];	
	char fcbcr;		
	short fcbrec;		
	char fcbovf;		
	};

#define FCBSIZ sizeof(struct FCB)


#define FNSIZE 30	
#define FMSIZE 30	



#if LATTICE
#define OPENR 0x8000		
#define OPENW 0x8001		
#define OPENU 0x8002		
#define OPENC 0x8001		
#else 
#define OPENR 0
#define OPENW 1
#define OPENU 2
#endif


struct MELT
	{
	struct MELT *fwd;	
#if SPTR
	unsigned size;		
#else
	long size;		
#endif
	};
#define MELTSIZE sizeof(struct MELT)
