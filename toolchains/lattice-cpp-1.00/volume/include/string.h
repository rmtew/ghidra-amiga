/**
*
* Define NULL if it's not already defined
*
*/
#ifndef NULL
#define NULL 0			/* null pointer value */
#endif

// Definitions for Lattice version 4.0:
#ifndef STRING_H
#define STRING_H 1
/**
*
* Builtin function definitions
*
**/
#if defined(LATTICE_4)
  #ifndef strlen
  #define strlen __builtin_strlen
  #define strcmp __builtin_strcmp
  #define strcpy __builtin_strcpy
  #endif
  #ifndef memset
  #define memset __builtin_memset
  #define memcmp __builtin_memcmp
  #define memcpy __builtin_memcpy
  #endif
#endif
/**
*
* External definitions for string services
*
*/

extern int stcarg(char *c1, char *c2);
extern int stccpy(char *t, const char *f, int l);
extern int stcgfe(char *x, char *n);
extern int stcgfn(char *node, char *n);
extern int stcis(const char *s, const char *b);
extern int stcisn(const char *s, const char *b);
extern int stclen(const char *s);
extern int stcd_i(const char *s, int *i);
extern int stcd_l(const char *, long *l);
extern int stch_i(const char *, int *i);
extern int stch_l(const char *, long *l);

extern int stci_d(char *, int i);
extern int stci_h(char *, int i);
extern int stci_o(char *, int i);
extern int stcl_d(char *, const long l);
extern int stcl_h(char *, const long l);
extern int stcl_o(char *, const long l);

extern int stco_i(const char *s, int *i);
extern int stco_l(const char *s, long *l);

extern int stcpm(const char *s, const char *p, char **m);
extern int stcpma(const char *s, const char *p);

extern int stcu_d(char *s, unsigned i);
extern int stcul_d(char *s, unsigned long l);

typedef char/*[3]*/ Lattice_Date;

enum Lattice_Date_Mode {
	LDM_YYMMDD_BCD,
	LDM_YYMMDD,		// ASCII
	LDM_MM_DD_YY_SLASH,
	LDM_MM_DD_YY_DASH,
	LDM_MMM_DD_YYYY,
	LDM_NAME_D_YYYY,
	LDM_DD_MM_YY,
	LDM_DD_MMM_YYYY
};

extern char *stpdate(char *to,
	 const enum Lattice_Date_mode m,
	 const Lattice_Date *d);

enum Lattice_Time_Mode {
	LTM_HHMMSSDD_BCD,
	LTM_HHMMSS,		// ASCII
	LTM_HH_MM_SS,
	LTM_HHMMSSDD,
	LTM_HH_MM_SS_DD,
	LTM_HH_MM,
	LTM_HH_MM_SS_XM,	// AM or PM
	LTM_HH_MM_XM
};

typedef char/*[4]*/ Lattice_Time;

extern char *stptime(char *s,
	const enum Lattice_Time_Mode m,
	const Lattice_Time *t);

extern char *stpblk(const char *s);
extern char *stpbrk(const char *s, const char *p);
extern char *stpchr(const char *s, int c);
extern char *stpcpy(char *t, const char *f);

extern char *stpsym(const char *s, char *sym, int len);

extern char *stptok(const char *s, char *t, int toklen, const char *b);


extern int strbpl(char **l, int n, const char *t);
extern char *strcat	(char *t, const char *f);
extern char *strchr(char *s, int c);
extern int strcmp(const char *a, const char *b);
extern int stricmp(const char *a, const char *b);
extern char *strcpy(char *t, const char *f);
extern int strcspn(const char *s, const char *p);
extern char *strdup(const char *s);
extern void strins(char *t, const char *f);
extern int strlen(const char *s);
extern char *strlwr(char *s);

extern void strmfe(char *n, const char *a, const char *x);
extern void strmfn(char *n, const char *d, const char *p,
	const char *n, const char *x);
extern void strmfp(char *n, const char *p, const char *n);

extern char *strncat(char *t, const char *f, unsigned l);
extern int strncmp(const char *a, const char *b, unsigned l);
extern char *strncpy(char *t, const char *f, unsigned l);
extern int strnicmp(const char *a, const char *b, unsigned l);
extern char *strnset(char *s, int v, int l);
extern char *strpbrk(char *s, char *b);
extern char *strrchr(const char *s, int c);
extern char *strrev(char *s);
extern char *strset(char *s, int c);
extern void strsfn(const char *f, char *d, char *f, char *n, char *x);
extern int strspn(const char *s, const char *p);
extern char *strtok(const char *s, const char *b);
extern long	strtol(const char *, char **, int);
extern char *strupr(char *s);

extern int stscmp(const char *a, const char *b);
extern int stspfp(const char *s, int *nx);

#define strcmpi stricmp		/* For Microsoft compatibility */
inline int streq(const char *a, const char *b)
	{return (strcmp (a,b) == 0); } // actually BOOLean

/**
*
* External definitions for memory block services
*
**/

extern char *memccpy(void *t, const char *f, int c, unsigned n);
extern char *memchr(const void *a, int c, unsigned n);
extern int memcmp(const void *a, const void *b, unsigned l);
extern char *memcpy(void *a, void *b, unsigned n);
extern char *memset(void *m, int c, unsigned n);

extern void movmem(void *t, const void *f, unsigned l);
extern void repmem(void *t, const void *f, int nv, int n);
extern void setmem(char *m, unsigned n, int c);
extern void swmem(void *a, void *b, unsigned l);
#endif STRING_H
