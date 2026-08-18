#ifndef  EXEC_TYPES_H
#define  EXEC_TYPES_H



#define GLOBAL extern               
#define IMPORT extern               
#define STATIC static               
#define REGISTER register           

#define VOID      void              

typedef long            LONG;       
typedef unsigned long   ULONG;      
typedef unsigned long   LONGBITS;   
typedef short           WORD;       
typedef unsigned short  UWORD;      
typedef unsigned short  WORDBITS;   
typedef char            BYTE;       
typedef unsigned char   UBYTE;      
typedef unsigned char   BYTEBITS;   
typedef unsigned char   *STRPTR;    
typedef STRPTR          *APTR;      


typedef short           SHORT;      
typedef unsigned short  USHORT;     



typedef float           FLOAT;
typedef double          DOUBLE;
typedef short           COUNT;
typedef unsigned short  UCOUNT;
typedef short           BOOL;
typedef unsigned char   TEXT;

#define TRUE      1
#define FALSE     0
#define NULL      0

#define BYTEMASK  0xFF

#define  LIBRARY_VERSION   31

#endif
