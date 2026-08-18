#ifndef	DEVICES_AUDIO_H
#define	DEVICES_AUDIO_H
#ifndef	EXEC_IO_H
#include	"exec/io.h"
#endif
#define	AUDIONAME	"audio.device"
#define	ADHARD_CHANNELS	4L
#define	ADALLOC_MINPREC	-128L
#define	ADALLOC_MAXPREC	127L
#define	ADCMD_FREE	(CMD_NONSTD+0)
#define	ADCMD_SETPREC	(CMD_NONSTD+1)
#define	ADCMD_FINISH	(CMD_NONSTD+2)
#define	ADCMD_PERVOL	(CMD_NONSTD+3)
#define	ADCMD_LOCK	(CMD_NONSTD+4)
#define	ADCMD_WAITCYCLE	(CMD_NONSTD+5)
#define	ADCMDB_NOUNIT	5L
#define	ADCMDF_NOUNIT	(1L<<5)
#define	ADCMD_ALLOCATE	(ADCMDF_NOUNIT+0)
#define	ADIOB_PERVOL	4L
#define	ADIOF_PERVOL	(1L<<4)
#define	ADIOB_SYNCCYCLE	5L
#define	ADIOF_SYNCCYCLE	(1L<<5)
#define	ADIOB_NOWAIT	6L
#define	ADIOF_NOWAIT	(1L<<6)
#define	ADIOB_WRITEMESSAGE	7L
#define	ADIOF_WRITEMESSAGE	(1L<<7)
#define	ADIOERR_NOALLOCATION	-10L
#define	ADIOERR_ALLOCFAILED	-11L
#define	ADIOERR_CHANNELSTOLEN	-12L
struct	IOAudio	{
struct	IORequest	ioa_Request;
WORD	ioa_AllocKey;
UBYTE	*ioa_Data;
ULONG	ioa_Length;
UWORD	ioa_Period;
UWORD	ioa_Volume;
UWORD	ioa_Cycles;
struct	Message	ioa_WriteMsg;
};
#endif
