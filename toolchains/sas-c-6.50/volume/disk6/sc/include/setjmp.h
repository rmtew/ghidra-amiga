Äà_SETJMP_HÄ_SETJMP_H 1
É_JMP_BUF{
çjmpret,
jmp_d1,jmp_d2,jmp_d3,jmp_d4,jmp_d5,jmp_d6,jmp_d7,
jmp_a1,jmp_a2,jmp_a3,jmp_a4,jmp_a5,jmp_a6,jmp_a7;
çjmp_fp0[3],jmp_fp1[3],jmp_fp2[3],jmp_fp3[3],
jmp_fp4[3],jmp_fp5[3],jmp_fp6[3],jmp_fp7[3];
};
•É_JMP_BUF jmp_buf[1];
ÅÇ__setjmp(jmp_buf);
Åãlongjmp(jmp_buf,Ç);Äsetjmp(x) __setjmp(x)á