/* device - list devices */

// Devices: Lists the devices in AmigaDOS.  Uses the accessor functions
// provided in dosextens.h - hardly a BPTR to be seen anywhere.

// This is really the only "safe" way, because otherwise all sorts of
// pointer conversions and casts have to be used ... and if you mis-cast,
// down the tubes it goes!

// (c) 1987, Tim Holloway

#include <exec/types.h>
#include <exec/execbase.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <stream.h>

#define nl "\n"
#define addr(z) hex( (LONG) z)

// Note: DosLibrary is not automatically opened by the present version
// of Lattice C++ because the Lattice compiler support functions
// do so in their startup/shutdown modules.  We do a second open to
// get a compiler-independent base vector.

struct DosLibrary *DosBase;

void
pbstr (char *tl, BSTR ch)	// print B-string
{
	register unsigned char *x = (unsigned char *) BADDR(ch);
	cout << form("%s [%lx/%lx]: ", tl, ch, x);

	if (x == 0) {cout << "address is null\n"; return;}
	for (register int i = *x; i > 0; i--)
	{
		x++;
		if ((*x < 0x20) || (*x > 127)) cout << form("<%x>", *x);
		else cout << chr(*x);
	}
	cout << nl;
}

const char *const dev_typenames[] = { "device", "directory", "file"};

void
main()
{
   if ( (DosBase = (struct DosLibrary *)
		OpenLibrary ("dos.library", 0)) == NULL)
   {
	cout << "Unable to open \"dos.library\"!\n";
	exit(10);
   }

   RootNode *rn = DosBase->dl_Root;
   DosInfo *di = rn->DosInfo();
   cout << "Dos base=" << addr(DosBase) <<
	" rootNode=" << addr(rn) << " Info=" << addr(di) << nl;

   for (register DeviceList *x = di->DevInfo(); x != NULL; x= x->next())
   {
	cout << "\n\nDevice List Entry at: " << addr(x) <<
		" is a " << dev_typenames[x->dl_Type] << nl;
	pbstr ("Name", x->dl_Name);
   }

   cout << nl;
   CloseLibrary (DosBase);
}
