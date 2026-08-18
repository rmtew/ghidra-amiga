/**
*
* This header file defines various ASCII character manipulation macros,
* as follows:
*
*       isalpha(c)    non-zero if c is alpha
*       isupper(c)    non-zero if c is upper case
*       islower(c)    non-zero if c is lower case
*       isdigit(c)    non-zero if c is a digit (0 to 9)
*       isxdigit(c)   non-zero if c is a hexadecimal digit (0 to 9, A to F,
*                   a to f)
*       isspace(c)    non-zero if c is white space
*       ispunct(c)    non-zero if c is punctuation
*       isalnum(c)    non-zero if c is alpha or digit
*       isprint(c)    non-zero if c is printable (including blank)
*       isgraph(c)    non-zero if c is graphic (excluding blank)
*       iscntrl(c)    non-zero if c is control character
*       isascii(c)    non-zero if c is ASCII
*       iscsym(c)     non-zero if valid character for C symbols
*       iscsymf(c)    non-zero if valid first character for C symbols
*
**/

const _U =	1;         /* upper case flag */
const _L =	2;         /* lower case flag */
const _N =	4;         /* number flag */
const _S =	8;         /* space flag */
const _P =	16;        /* punctuation flag */
const _C =	32;        /* control character flag */
const _B =	64;        /* blank flag */
const _X =	128;       /* hexadecimal flag */

extern char _ctype[];   /* character type table */

inline int isalpha(int c) {return (_ctype[(c)+1]&(_U|_L)); }
inline int isupper(int c) {return (_ctype[(c)+1]&_U); }
inline int islower(int c) {return (_ctype[(c)+1]&_L); }
inline int isdigit(int c) {return (_ctype[(c)+1]&_N); }
inline int isxdigit(int c) {return (_ctype[(c)+1]&_X); }
inline int isspace(int c) {return (_ctype[(c)+1]&_S); }
inline int ispunct(int c) {return (_ctype[(c)+1]&_P); }
inline int isalnum(int c) {return (_ctype[(c)+1]&(_U|_L|_N)); }
inline int isprint(int c) {return (_ctype[(c)+1]&(_P|_U|_L|_N|_B)); }
inline int isgraph(int c) {return (_ctype[(c)+1]&(_P|_U|_L|_N)); }
inline int iscntrl(int c) {return (_ctype[(c)+1]&_C); }
inline int isascii(int c) {return ((unsigned)(c)<=127); }
inline int iscsym(int c) {return (isalnum(c)||(((c)&127)==0x5f)); }
inline int iscsymf(int c) {return (isalpha(c)||(((c)&127)==0x5f)); }

inline int toupper(int c) {return (islower(c)?((c)-('a'-'A')):(c)); }
inline int tolower(int c) {return (isupper(c)?((c)+('a'-'A')):(c)); }
inline int toascii(int c) {return ((c)&127); }
/**
*
* Define NULL if it's not already defined
*
*/
#ifndef NULL
#define NULL 0
#endif
