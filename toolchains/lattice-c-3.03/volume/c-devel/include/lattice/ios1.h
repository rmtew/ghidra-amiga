
struct UFB
{
char ufbflg;		
char ufbtyp;
int ufbfh;		
};
#define NUFBS 20	


#define UFB_OP 0x80	
#define UFB_RA 0x40	
#define UFB_WA 0x20	
#define UFB_NT 0x10	
#define UFB_AP 8		
#define UFB_NC 4	


#if MSDOS1
#define D_DISK 0
#define D_CON 1
#define D_PRN 2
#define D_AUX 3
#define D_NULL 4
#endif
