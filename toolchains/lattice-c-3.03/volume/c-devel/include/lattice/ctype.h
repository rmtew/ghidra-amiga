

#define _U 1          
#define _L 2          
#define _N 4          
#define _S 8          
#define _P 16         
#define _C 32         
#define _B 64         
#define _X 128        

extern char _ctype[];   

#define isalpha(c)      (_ctype[(c)+1]&(_U|_L))
#define isupper(c)      (_ctype[(c)+1]&_U)
#define islower(c)      (_ctype[(c)+1]&_L)
#define isdigit(c)      (_ctype[(c)+1]&_N)
#define isxdigit(c)     (_ctype[(c)+1]&_X)
#define isspace(c)      (_ctype[(c)+1]&_S)
#define ispunct(c)      (_ctype[(c)+1]&_P)
#define isalnum(c)      (_ctype[(c)+1]&(_U|_L|_N))
#define isprint(c)      (_ctype[(c)+1]&(_P|_U|_L|_N|_B))
#define isgraph(c)      (_ctype[(c)+1]&(_P|_U|_L|_N))
#define iscntrl(c)      (_ctype[(c)+1]&_C)
#define isascii(c)      ((unsigned)(c)<=127)
#define iscsym(c)       (isalnum(c)||(((c)&127)==0x5f))
#define iscsymf(c)      (isalpha(c)||(((c)&127)==0x5f))

#define toupper(c)     (islower(c)?((c)-('a'-'A')):(c))
#define tolower(c)     (isupper(c)?((c)+('a'-'A')):(c))
#define toascii(c)      ((c)&127)
