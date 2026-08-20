Äà_STDIO_HÄ_STDIO_H 1å<sys/commsize.h>å<sys/commlist.h>
•éçfpos_t;å<sys/commnull.h>Ä_IOFBF 0Ä_IOREAD 1Ä_IOWRT 2Ä_IONBF 4Ä_IOMYBUF 8Ä_IOEOF 16Ä_IOERR 32Ä_IOLBF 64Ä_IORW 128Ä_IORKEY 0x2000Ä_IOAPP 0x4000Ä_IOXLAT 0x8000ÄBUFSIZ 512ÄEOF (-1)ÄFOPEN_MAX 20ÄFILENAME_MAX 64ÄL_tmpnam 64ÄSEEK_SET 0ÄSEEK_CUR 1ÄSEEK_END 2ÄTMP_MAX 999
É__iobuf{
É__iobuf*_next;
éÑ*_ptr;
Ç_rcnt;
Ç_wcnt;
éÑ*_base;
Ç_size;
Ç_flag;
Ç_file;
éÑ_cbuff;
};
•É__iobuf¢;
ÅÉ__iobuf __iob[];Ästdin (&__iob[0])Ästdout (&__iob[1])Ästderr (&__iob[2])
ÅÇremove(constÑ*);
ÅÇrename(constÑ*,constÑ*);
Å¢*tmpfile(ã);
ÅÑ*tmpnam(Ñ*s);
ÅÇfclose(¢*);
ÅÇfflush(¢*);
Å¢*fopen(constÑ*,constÑ*);
Å¢*freopen(constÑ*,constÑ*,¢*);
Åãsetbuf(¢*,Ñ*);
ÅÇsetvbuf(¢*,Ñ*,Ç,size_t);
ÅÇfprintf(¢*,constÑ*,...);
ÅÇfscanf(¢*,constÑ*,...);
ÅÇprintf(constÑ*,...);
ÅÇ__builtin_printf(constÑ*,...);
ÅÇscanf(constÑ*,...);
ÅÇsprintf(Ñ*,constÑ*,...);
ÅÇsscanf(constÑ*,constÑ*,...);
ÅÇvfprintf(¢*,constÑ*,va_list);
ÅÇvprintf(constÑ*,va_list);
ÅÇvsprintf(Ñ*,constÑ*,va_list);
ÅÇfgetc(¢*);
ÅÑ*fgets(Ñ*,Ç,¢*);
ÅÇfputc(Ç,¢*);
ÅÇfputs(constÑ*,¢*);
ÅÇgetc(¢*);Ägetc(p) \
(((p)->_flag&_IOREAD)?\
(--(p)->_rcnt>=0&&!((p)->_file&_IORKEY)?\
*(p)->_ptr++\
:\
fgetc(p))\
:\
fgetc(p))
ÅÇgetchar(ã);Ägetchar() getc(stdin)
ÅÑ*gets(Ñ*);
ÅÇputc(Ç,¢*);Äputc(c,p) \
(((p)->_flag&_IOWRT)?\
(++(p)->_wcnt<=(p)->_size?\
((((Ç)(*(p)->_ptr++=((éÑ)(c))))=='\n'&&\
((p)->_flag&_IOLBF))?\
fflush(p),((éÑ)(c))\
:\
((éÑ)(c)))\
:\
(--(p)->_wcnt,fputc(c,p)))\
:\
fputc(c,p))
ÅÇputchar(Ç);Äputchar(c) putc(c,stdout)
ÅÇputs(constÑ*);
ÅÇungetc(Ç,¢*);
Åsize_t fread(ã*,size_t,size_t,¢*);
Åsize_t fwrite(constã*,size_t,size_t,¢*);
ÅÇfgetpos(¢*,fpos_t*);
ÅÇfseek(¢*,çÇ,Ç);
ÅÇfsetpos(¢*,const fpos_t*);
ÅçÇftell(¢*);
Åãrewind(¢*);Ärewind(p) fseek(p,0L,0)
Åãclearerr(¢*);Äclearerr(p) ((p)->_flag&=~(_IOERR|_IOEOF))
ÅÇfeof(¢*);Äfeof(p) (((p)->_flag&_IOEOF)!=0)
ÅÇferror(¢*);Äferror(p) (((p)->_flag&_IOERR)!=0)
Åãperror(constÑ*);Äprintf __builtin_printfà_STRICT_ANSIÄR_OK 4ÄW_OK 2ÄX_OK 1ÄF_OK 0
ÅÇ__io2errno(Ç);
ÅÇfcloseall(ã);
Å¢*fdopen(Ç,constÑ*);
ÅÇfhopen(ç,Ç);
ÅÇfgetchar(ã);
ÅÇfileno(¢*);
ÅÇflushall(ã);
Åãfmode(¢*,Ç);
ÅÇ_writes(constÑ*,...);
ÅÇ_tinyprintf(Ñ*,...);
ÅÇfputchar(Ç);
Åãsetnbf(¢*);
ÅÇ__fillbuff(¢*);
ÅÇ__flushbuff(Ç,¢*);
ÅÇ__access(constÑ*,Ç);
ÅÇaccess(constÑ*,Ç);
ÅÇchdir(constÑ*);
ÅÇchmod(constÑ*,Ç);
ÅÑ*getcwd(Ñ*,Ç);
ÅÇunlink(constÑ*);
ÅÇposerr(constÑ*);Äclrerr clearerrÄaccess __accessÄ_filbf __fillbuffÄfileno(p) ((p)->_file)Ä_flsbf __flushbuff
Çrawcon(Ç);
Çgetch(ã);á
Åéç__fmask;
ÅÇ__fmode;á