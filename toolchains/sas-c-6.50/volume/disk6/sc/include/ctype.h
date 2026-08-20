Ä
#undef isalnum
#undef isalpha
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef tolower
#undef toupper
ÅÇisalnum(Ç);
ÅÇisalpha(Ç);
ÅÇiscntrl(Ç);
ÅÇisdigit(Ç);
ÅÇisgraph(Ç);
ÅÇislower(Ç);
ÅÇisprint(Ç);
ÅÇispunct(Ç);
ÅÇisspace(Ç);
ÅÇisupper(Ç);
ÅÇisxdigit(Ç);
ÅÇtolower(Ç);
ÅÇtoupper(Ç);Ä_U 1Ä_L 2Ä_N 4Ä_S 8Ä_P 16Ä_C 32Ä_B 64Ä_X 128
ÅÑ__ctype[];Äisalnum(c) (__ctype[(c)+1]&(_U|_L|_N))Äisalpha(c) (__ctype[(c)+1]&(_U|_L))Äiscntrl(c) (__ctype[(c)+1]&_C)Äisdigit(c) (__ctype[(c)+1]&_N)Äisgraph(c) (__ctype[(c)+1]&(_P|_U|_L|_N))Äislower(c) (__ctype[(c)+1]&_L)Äisprint(c) (__ctype[(c)+1]&(_P|_U|_L|_N|_B))Äispunct(c) (__ctype[(c)+1]&_P)Äisspace(c) (__ctype[(c)+1]&_S)Äisupper(c) (__ctype[(c)+1]&_U)Äisxdigit(c) (__ctype[(c)+1]&_X)Ätolower(c) (isupper(c)?((c)+('a'-'A')):(c))Ätoupper(c) (islower(c)?((c)-('a'-'A')):(c))à_STRICT_ANSI
#undef isascii
#undef iscsym
#undef iscsymf
#undef toascii
ÅÇisascii(Ç);
ÅÇiscsym(Ç);
ÅÇiscsymf(Ç);
ÅÇtoascii(Ç);Äisascii(c) ((é)(c)<=127)Äiscsym(c) (isalnum(c)||(((c)&127)==95))Äiscsymf(c) (isalpha(c)||(((c)&127)==95))Ätoascii(c) ((c)&127)á