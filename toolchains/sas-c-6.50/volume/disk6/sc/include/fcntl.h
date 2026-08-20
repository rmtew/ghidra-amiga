€ˆ_FCNTL_H€_FCNTL_H 1€O_RDONLY 0€O_WRONLY 1€O_RDWR 2€O_NDELAY 0€O_APPEND 8€O_CREAT 256€O_TRUNC 512€O_EXCL 0x400€O_LOCK 0x1000€O_TEMP 0x2000€O_XLATE 0x4000€O_RAW 0x8000€O_BINARY 0x8000Œ<sys/commifmt.h>€F_DUPFD 0€F_GETFD 1€F_SETFD 2€F_GETFL 3€F_SETFL 4
‚__open(const„*,‚,...);
‚__creat(const„*,‚);
‚__read(‚,‹*,‚);
‚__write(‚,const‹*,‚);
__lseek(‚,,‚);
‚__close(‚);
‚open(const„*,‚,...);
‚creat(const„*,‚);
‚read(‚,‹*,‚);
‚write(‚,const‹*,‚);
lseek(‚,,‚);
tell(‚);
‚close(‚);
‚unlink(const„*);
‚iomode(‚,‚);
‚isatty(‚);€open __open€creat __creat€read __read€write __write€lseek __lseek€close __close€tell(x) lseek(x,0L,1)Œ<sys/commnull.h>‡