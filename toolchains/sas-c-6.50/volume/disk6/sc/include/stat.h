€ˆ_STAT_H€_STAT_H 1ˆ_INO_T€_INO_T 1
¥ino_t;‡ˆ_DEV_T€_DEV_T 1
¥dev_t;‡ˆ_OFF_T€_OFF_T 1
¥off_t;‡Œ<sys/commtime.h>
ƒstat{
Ÿst_mode;
ino_t st_ino;
dev_t st_dev;
„*st_rdev;
Ÿst_nlink;
Ÿst_uid;
Ÿst_gid;
off_t st_size;
time_t st_atime;
time_t st_mtime;
time_t st_ctime;
Ÿst_type;
„*st_comment;
};
‚stat(const„*,ƒstat*);
‚lstat(const„*,ƒstat*);
‚fstat(‚,ƒstat*);€OFS 0x444F5300€FFS 0x444F5301Œ<sys/commifmt.h>‡