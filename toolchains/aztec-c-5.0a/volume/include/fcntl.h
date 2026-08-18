/* Copyright Manx Software Systems, Inc. 1988.  All rights reserved */

#ifndef __FCNTL_H 
#define __FCNTL_H 

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif

#define O_RDONLY	0
#define O_WRONLY	1
#define O_RDWR		2
#define O_CREAT		0x0100
#define O_TRUNC		0x0200
#define O_EXCL		0x0400
#define O_APPEND	0x0800
#define	O_TEXT		0x1000

#define	O_CONRAW	0x4000
#define	O_STDIO		0x8000

extern struct _dev {
	struct FileHandle *fd;
	short	mode;
} *_devtab;

extern short _numdev;

int creat(const char *_name, int _mode);
int open(const char * _name, int _mode, ...);
int access(char *, int);
int close(int);
size_t read(int, void *, size_t);
size_t write(int, void *, size_t);
long lseek(int, long, int);
struct sgttyb;
int ioctl(int, int, struct sgttyb *);
struct MsgPort;
long dos_packet(struct MsgPort *, long, ...);
int isatty(int);
int setenv(char *, char *);
int unlink(char *);
void set_con(void);
void set_raw(void);
void _exit(int _code);
int chdir(char *_path);
char *getcwd(char *_path, int _size);
int mkdir(char *_name);
char *scdir(char *_pat);
void _abort(void);
void pokeb(unsigned long, int);
int peekb(unsigned long);
void pokew(unsigned long, int);
int peekw(unsigned long);
void pokel(unsigned long, long);
long peekl(unsigned long);
int wait(void);
int fexecl(char *file, char *arg0);
int fexecv(char *cmd, char **argv);
int execl(char *file, char *arg0);
int execv(char *arg, char **argv);
int execlp(char *file, char *arg0);
int execvp(char *arg, char **argv);

#endif

