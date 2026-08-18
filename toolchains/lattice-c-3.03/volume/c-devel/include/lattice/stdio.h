
#define _BUFSIZ 512		
#define BUFSIZ 512		
#define _NFILE 20		

struct _iobuf
{
unsigned char *_ptr;			
int _rcnt;			
int _wcnt;			
unsigned char *_base;		 	
char _flag;			
char _file;			
int _size;			
unsigned char _cbuff;			
char _pad;			
};

extern struct _iobuf _iob[_NFILE];

#define _IOREAD 1		
#define _IOWRT 2		
#define _IONBF 4		
#define _IOMYBUF 8		
#define _IOEOF 16		
#define _IOERR 32		
#define _IOSTRG 64
#define _IORW 128		

#ifndef NULL
#if SPTR
#define NULL 0			
#else
#define NULL 0L
#endif
#endif
#define FILE struct _iobuf	
#define EOF (-1)		

#define stdin (&_iob[0])	
#define stdout (&_iob[1])	
#define stderr (&_iob[2])	

#define getc(p) (--(p)->_rcnt>=0? *(p)->_ptr++:_filbf(p))
#define getchar() getc(stdin)
#define putc(c,p) (--(p)->_wcnt>=0? ((int)(*(p)->_ptr++=(c))):_flsbf((c),p))
#define putchar(c) putc(c,stdout)
#define feof(p) (((p)->_flag&_IOEOF)!=0)
#define ferror(p) (((p)->_flag&_IOERR)!=0)
#define fileno(p) (p)->_file
#define rewind(fp) fseek(fp,0L,0)
#define fflush(fp) _flsbf(-1,fp)
#define clearerr(fp) clrerr(fp)

FILE *fopen();
FILE *freopen();
long ftell();
char *fgets();

#ifndef abs
#define abs(x) ((x)<0?-(x):(x))
#endif
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<=(b)?(a):(b))

