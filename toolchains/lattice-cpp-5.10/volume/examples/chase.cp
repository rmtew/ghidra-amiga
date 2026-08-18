// Chase.cp - Run dile tree
// (C) 1988, Tim Holloway

// This program illustrates walking a directory tree via AmigaDOS
// I/O packets.  By defining a class dospacket, we eliminate some of
// the ickiest of the problems of BCPL-style message packets.
// Naturally, the simplest way to chase the file tree is to use recursion.

#include <exec/types.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <stream.h>
#include <string.h>

#define EOS 0	/* end of string */

#define nl "\n";

MsgPort *fhi;			// File handler's message port
StdPort *dospacket_reply;	// our reply port

class dospacket : public StandardPacket {
public:
	LONG examine(ULONG lock, FileInfoBlock *fib);
	LONG exnext(ULONG lock, FileInfoBlock *fib);
	LONG lock(ULONG lock, char *name, LONG mode);
	LONG unlock(ULONG);
	LONG ioerr() { 	return sp_Pkt.dp_Res2; }
};

LONG
dospacket::examine(ULONG lock, FileInfoBlock *fib)
{
	sendio (fhi, dospacket_reply,
		ACTION_EXAMINE_OBJECT,
		lock,
		ULONG( fib) >> 2  // BPTR
		);
	dospacket_reply->wait();
	dospacket_reply->get();
	return sp_Pkt.dp_Res1;
}

LONG
dospacket::exnext(ULONG lock, FileInfoBlock *fib)
{
	sendio (fhi, dospacket_reply,
		ACTION_EXAMINE_NEXT,
		lock,
		ULONG( fib) >> 2  // BPTR
		);
	dospacket_reply->wait();
	dospacket_reply->get();
	return sp_Pkt.dp_Res1;
}

LONG
dospacket::lock(ULONG lock, char *name, LONG mode)
{
	sp_Pkt.dp_Res1 = 0;		// safeguard against sloppy DOS
	sendio (fhi, dospacket_reply,
		ACTION_LOCATE_OBJECT,
		lock,
		ULONG( name) >> 2,  // BPTR
		mode
		);
	dospacket_reply->wait();
	dospacket_reply->get();
	return sp_Pkt.dp_Res1;
}

LONG
dospacket::unlock(ULONG lock)
{
	sendio (fhi, dospacket_reply,
		ACTION_FREE_LOCK,
		lock,
		);
	dospacket_reply->wait();
	dospacket_reply->get();
	return sp_Pkt.dp_Res1;
}

//*************************************************************************//

// concatenate a path level

char *
concat (register char *basis, register unsigned char *name)
{
	register int i = *name++;
	while (i-- > 0)
	{
		*basis++ = *name++;
	}
	*basis = EOS;
	return basis;
}

// process a directory level

char path_name[129] = "df0:";

BOOL
plunge(BPTR dlock, char *addon)
{
	dospacket *seekpak = new dospacket;
	FileInfoBlock *fib = new FileInfoBlock;

	int retcode = seekpak->examine(dlock, fib);

	while (TRUE)
	{
		retcode = seekpak->exnext(dlock, fib);
		if (!retcode) break;
		char *endstr = concat (addon, fib->fib_FileName);

		if (fib->is_directory())
		{
			cout << path_name << " [dir]\n"; 
			BPTR alock = seekpak->lock(dlock,
				fib->fib_FileName,
				ACCESS_READ);
			if (alock == 0)
			{
				cout << "ERROR: couldn't get lock!\n";
			}
			else
			{
				*endstr++ = '/';
				plunge (alock, endstr);
				seekpak->unlock(alock);
			}
		}
		else
			cout << "  " << path_name << nl; 
	}

	retcode = seekpak->ioerr();

	delete fib;
	delete seekpak;
	if (retcode != ERROR_NO_MORE_ENTRIES)
	{
		cout << "ERROR: reading file system.  Code=" <<
			retcode << nl;
		return FALSE;
	}
	return TRUE;
}

main()
{
	dospacket_reply = new StdPort;

	fhi = DeviceProc ("df0:");
	BPTR dlock = IoErr();
 
	cout << "Device Proc is at " << hex( (LONG) fhi) << ", lock is " <<
		hex( (LONG) dlock) << nl;
	cout.flush();
	plunge (dlock, path_name+4);

	dospacket *apack = new dospacket;
	apack->unlock(dlock);

	delete apack;

	delete dospacket_reply;
}
