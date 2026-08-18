Äå"exec/types.h"å"exec/ports.h"å"exec/lists.h"å"libraries/dos.h"ÄLATTICE 1ÄLATTICE_V4 1
•éÑbyte;ÄSECSIZ 512ÄFNSIZE 32ÄFMSIZE 64ÄFESIZE 32ÄDISKINFO InfoDataÄFILEINFO FileInfoBlock
ÉMELT
{
ÉMELT*fwd;
çsize;
};ÄMELTSIZE sizeof(ÉMELT)
ÉMELT2
{
ÉMELT2*fwd;
ÉMELT2*bwd;
ésize;
};ÄMELT2SIZE sizeof(ÉMELT2)
ÉProcID{
ÉProcID*nextID;
ÉProcess*process;
ÇUserPortFlag;
É©*parent;
É©*child;
°seglist;
};
ÉFORKENV{
çpriority;
çstack;
°std_in;
°std_out;
°console;
É©*msgport;
};
ÉTermMsg{
ÉØmsg;
çclass;
ütype;
ÉProcess*process;
çret;
};à__ARGS∞™Ä__ARGS(a) ()ùÄ__ARGS(a) aáá
ÅÇforkl __ARGS((Ñ*,Ñ*,));
ÅÇforkv __ARGS((Ñ*,Ñ**,ÉFORKENV*,ÉProcID*));
ÅÇwait __ARGS((ÉProcID*));
ÅÉProcID*waitm __ARGS((ÉProcID**));
ÅÇ_dclose __ARGS((ç));
Åç_dcreat __ARGS((Ñ*,Ç));
Åç_dcreatx __ARGS((Ñ*,Ç));
ÅÇdfind __ARGS((ÉFILEINFO*,Ñ*,Ç));
ÅÇdnext __ARGS((ÉFILEINFO*));
Åç_dopen __ARGS((Ñ*,Ç));
Åé_dread __ARGS((ç,Ñ*,é));
Åç_dseek __ARGS((ç,ç,Ç));
Åédwrite __ARGS((ç,Ñ*,é));
ÅÇgetcd __ARGS((Ç,Ñ*));
ÅÇgetdfs __ARGS((Ñ*,ÉDISKINFO*));
ÅÇgetfa __ARGS((Ñ*));
Åçgetft __ARGS((Ñ*));
ÅÇchgclk __ARGS((éÑ*));
ÅÇgetclk __ARGS((éÑ*));
ÅÇonbreak __ARGS((Ç__ARGS((*))__ARGS(())));
Åãonerror __ARGS((Ç));
ÅÇposerr __ARGS((Ñ*));
