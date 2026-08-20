€ˆ_DOS_H€_DOS_H 1Œ<exec/types.h>Œ<exec/ports.h>Œ<exec/lists.h>Œ<libraries/dos.h>Œ<libraries/dosextens.h>Œ<sys/commsize.h>
¥„byte;€SECSIZ 512€FNSIZE 108€FMSIZE 256€FESIZE 32
ƒMELT{
ƒMELT*fwd;
size;
};€MELTSIZE sizeof(ƒMELT)
ƒMELT2{
ƒMELT2*fwd;
ƒMELT2*bwd;
size;
};€MELT2SIZE sizeof(ƒMELT2)
ƒProcID{
ƒProcID*nextID;
ƒ¯*process;
‚UserPortFlag;
ƒ©*parent;
ƒ©*child;
¡seglist;
};
ƒFORKENV{
priority;
stack;
¡std_in;
¡std_out;
¡console;
ƒ©*msgport;
};
ƒTermMsg{
ƒ¯msg;
_class;
Ÿtype;
ƒProcess*process;
ret;
};
‚forkl(„*,„*,...);
‚forkv(„*,„**,ƒFORKENV*,ƒProcID*);
‚wait(ƒProcID*);
ƒProcID*waitm(ƒProcID**);
‚dfind(ƒFileInfoBlock*,const„*,‚);
‚dnext(ƒFileInfoBlock*);
‚_dclose();
_dcreat(const„*,‚);
_dcreatx(const„*,‚);
_dopen(const„*,‚);
‚_dread(,„*,‚);
_dseek(,,‚);
‚_dwrite(,„*,‚);
‚getcd(‚,„*);
‚chdir(const„*);
„*getcwd(„*,‚);
‚mkdir(const„*);
‚rmdir(const„*);
‚getfnl(const„*,„*,size_t,‚);
‚getdfs(const„*,ƒInfoData*);
‚getfa(const„*);
getft(const„*);
‚getpath(¡,„*);
¡findpath(const„*);
_BackGroundIO;
¡_Backstdout;
__priority;
„*__procname;
„__stdiowin[];
„__stdiov37[];
__oslibversion;
__stack;
__STKNEED;Œ<sys/commwben.h>
‚datecmp(constƒDateStamp*,constƒDateStamp*);
‚chgclk(const„*);
‹getclk(„*);
‚onbreak(‚(*)(‹));
‚poserr(const„*);
‹__regargs __chkabort(‹);
‹chkabort(‹);
‹Chk_Abort(‹);
ƒDeviceList*getasn(const„*);
stacksize(‹);
stackused(‹);
stackavail(‹);€geta4 __builtin_geta4
‹geta4(‹);€getreg __builtin_getreg
getreg(‚);€putreg __builtin_putreg
‹putreg(‚,);€__emit __builtin_emit
‹__emit(‚);€REG_D0 0€REG_D1 1€REG_D2 2€REG_D3 3€REG_D4 4€REG_D5 5€REG_D6 6€REG_D7 7€REG_A0 8€REG_A1 9€REG_A2 10€REG_A3 11€REG_A4 12€REG_A5 13€REG_A6 14€REG_A7 15€REG_FP0 16€REG_FP1 17€REG_FP2 18€REG_FP3 19€REG_FP4 20€REG_FP5 21€REG_FP6 22€REG_FP7 23‡