Äà_SYS_DIR_HÄ_SYS_DIR_Hå<sys/commargs.h>å<sys/types.h>å<stat.h>ÄDEV_BSIZE 1024ÄDIRBLKSIZ DEV_BSIZEÄMAXNAMLEN 255
Édirent{
u_long d_ino;
u_short d_reclen;
u_short d_namlen;
off_t d_off;
Ñd_name[MAXNAMLEN+1];
};
#undef DIRSIZÄDIRSIZ(dp) \
((sizeof(Édirent)-(MAXNAMLEN+1))+(((dp)->d_namlen+1+3)&~3))
•É_dirdesc{
çdd_fd;
çdd_loc;
çdd_size;
Ñ*dd_buf;
}DIR;å<sys/commnull.h>
ÅDIR*opendir __ARGS((Ñ*));
ÅÉdirent*readdir __ARGS((DIR*));
Åçtelldir __ARGS((DIR*));
Åãseekdir __ARGS((DIR*,ç));
Åãclosedir __ARGS((DIR*));Ärewinddir(dirp) seekdir((dirp),(ç)0)á