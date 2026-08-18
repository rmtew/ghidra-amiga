// Fonts.c - list available fonts

#include <stream.h>

#include <exec/types.h>
#include <graphics/text.h>
#include <graphics/diskfont.h>

// A custom version of Gripe:

void Gripe(const char *s, const char *s1, const char *s2)
{
	cout << s;
	if (s1) cout << s1;
	if (s2) cout << s2;
}

const MAXFONTS = 20;

struct {
	AvailFontsHeader ah;
	AvailFontsDesc af[MAXFONTS];
} fonts = {{MAXFONTS}};

main()
{
	cout << "List available fonts.\n\n";
	cout << "AvailFonts error code: " <<
		AvailFonts(&fonts, sizeof(fonts), AFF_MEMORY | AFF_DISK)
	     << "\n";

	register int i;
	for (i=0; i < MAXFONTS; i++)
	{
		if (fonts.af[i].af_FontLoc == 0) break;

		cout << "Font: " << 
			fonts.af[i].af_Attributes.ta_Name << ", " <<
			fonts.af[i].af_Attributes.ta_YSize <<
			" points, residence: " <<
			(fonts.af[i].af_FontLoc == AFF_MEMORY) ?
				"RAM" : "Disk"
		;
		cout << "\n";
	}

	cout << "\nEnd of Listing.\n";
}
