/** stdio.h - standard stream I/O definitions
*
* This header file defines the information used by the Lattice v3.10
* standard I/O package. It DOES NOT work properly with earlier versions!
*
* Manx definitions are for Manx version 3.4a.
*
**/

#ifndef STDIO_H
#define STDIO_H
#define MAXSTREAM 20

#if defined(LATTICE_4)
  #define printf __builtin_printf // LATTICE_40
#endif

#if !defined(AMIGA)
  #message "ERROR - stdio.h environment is not AMIGA"
  #exit (20)
#endif

#define _BUFSIZ 512		/* standard buffer size */
#define BUFSIZ _BUFSIZ
#define _NFILE MAXSTREAM		/* maximum number of files */


enum _streamioflag { // Definitions associated with _iobuf._flag
	_IOFBF = 0,		/* fully buffered (for setvbuf) */
	_IOREAD = 1,		/* read flag */
	_IOWRT =2,		/* write flag */
	_IONBF =4,		/* non-buffered flag */
	_IOMYBUF= 8,		/* private buffer flag */
	_IOEOF =16,		/* end-of-file flag */
	_IOERR =32,		/* error flag */
	_IOLBF =64,		/* line-buffered flag */
	_IORW =128,		/* read-write (update) flag */
	_IOAPP =0x4000,		/* append flag */
	_IOXLAT =0x8000,	/* translation flag */
};


struct _iobuf
{
struct _iobuf *_next;
unsigned char *_ptr;		/* current buffer pointer */
int _rcnt;			/* current byte count for reading */
int _wcnt;			/* current byte count for writing */
unsigned char *_base;		/* base address of I/O buffer */
int _size;			/* size of buffer */
enum _streamioflag _flag;			/* control flags */
#ifdef MSDOS
unsigned char _file;		/* file number */
#else
unsigned long _file;		/* file descriptor */
#endif
unsigned char _cbuff;		/* single char buffer */
};

#ifndef NULL
#define NULL 0
#endif

const EOF = -1;			/* end-of-file code */

typedef struct _iobuf FILE;	/* shorthand */

extern struct _iobuf _iob[MAXSTREAM];

#define stdin (&_iob[0])	/* standard input file pointer */
#define stdout (&_iob[1])	/* standard output file pointer */
#define stderr (&_iob[2])	/* standard error file pointer */
#ifdef MSDOS
#define stdaux (&_iob[3])	/* standard auxiliary file pointer */
#define stdprt (&_iob[4])	/* standard printer file pointer */
#endif

//struct FileInfoBlock;	// stuff defined in dos.h

//extern int dclose(int fh);
//extern int dcreat(const char *, int);
//extern int dcreatx(const char *, int);
//extern int dopen(char *, int);
//extern int dfind(struct FileInfoBlock*, const char *, int);
//extern int dnext(struct FileInfoBlock*);
//extern int dread(int fh, char *, unsigned int len);
//extern int dwrite(int fh, const char *, unsigned int len);
//extern int dseek(int fh, long pos, int mode);


extern int fseek(FILE *f, long pos, int mode);

#ifdef MANX
extern int agetc();
extern int aputc(int ch, FILE *outf);

#define getchar() agetc(stdin)
#define putchar(c) aputc(c, stdout)
inline int feof(FILE *fp) {return (((fp)->_flags&_EOF)!=0); }
inline int ferror(FILE *fp) {return (((fp)->_flags&_IOERR)!=0); }
inline int clearerr(FILE *fp) {return ((fp)->_flags &= ~(_IOERR|_EOF));}
inline int fileno(FILE *fp) {return ((fp)->_unit); }
extern int flsh_(FILE *fp, int);
inline int fflush(FILE *fp) {return flsh_(fp,-1); }
inline int rewind(FILE *fp) {return fseek(fp, 0L, 0); }
extern int putc(int, FILE *fp);

#define	P_tmpdir	""
#define L_tmpnam	40

#else
extern int _filbf(FILE *f);
extern int _flsbf(int m, FILE *f);

inline int getc(FILE *p)
{ return (--(p)->_rcnt>=0? *(p)->_ptr++:_filbf(p)); }

#define getchar() getc(stdin)

// inline int putc(int c, FILE *p)
// {
//  return (--(p)->_wcnt>=0
// 	? ((int) (*(p->_ptr++)=c) ):_flsbf((unsigned char)(c),p));
// }
#define putc(c,p) (--(p)->_wcnt>=0 ?((int)(*(p)->_ptr++=(c))):_flsbf((unsigned char)(c),p))
#define putchar(c) putc(c,stdout)

inline int feof(FILE *p) {return (((p)->_flag&_IOEOF)!=0); }
inline int ferror(FILE *p) {return (((p)->_flag&_IOERR)!=0);}
inline int fileno(FILE *p) {return(p)->_file; }
#define rewind(fp) fseek(fp,0L,0)
inline void fflush(FILE *fp) {_flsbf(-1,fp); }
#define clearerr(fp) clrerr(fp)

extern void clrerr(FILE *p);

#endif

extern int cprintf(const char *c, ... );
extern int cscanf(char *, ... );
extern int fclose(FILE *p);
extern int fcloseall();
extern FILE *fdopen(int i, char *c);
extern int fgetc(FILE *f);
extern int fgetchar();
extern char *fgets(char *, int l, FILE *f);
extern int flushall();
extern int fmode(FILE *, int mode);
extern FILE *fopen(const char *name, const char *mode);
// extern FILE *fopene(char *c1, char *c2, char *);
extern int fprintf(FILE *f, const char *s , ... );
extern int fputc(int c, FILE *f);
extern int fputchar(int c);
extern int fputs(const char *s, FILE *f);
extern int fread(char *b, int i, int j, FILE *f);
extern FILE *freopen(const char *nm, const char *mode, FILE *f);
extern int fscanf(FILE *f, const char *s, ... );

extern long ftell(FILE *f);
extern int fwrite(const char *b, int i, int j, FILE *f);
extern char *gets(char *buf);
extern int printf(const char *s, ... );
extern int puts(char *s);
extern scanf(const char *s, ... );
extern int setbuf(FILE *f, char *b);
extern int setnbf(FILE *f);
extern int setvbuf(FILE *f, char *b, int typ, int siz);
extern int sprintf(char *s, const char *f, ... );
extern sscanf(char *s, const char *f, ... );
extern int ungetc(int ch, FILE *f);

extern int access(const char *name, int m);
extern int chdir(const char *d);
extern int chmod(const char *name, int m);
extern char *getcwd(char *d, int sz);
extern int mkdir(const char *name);
extern int perror(const char *prefix);
extern int rename(const char *oldname, const char *newname);
extern int rmdir(const char *name);
extern char *tmpnam(const char *model);
#endif
