
#include "exec/types.h"
#include "exec/exec.h" 
#include "exec/execbase.h" 

extern struct ExecBase *SysBase;

ULONG AvailMem ();

/* maxsize -- determine the total maximum size for all regions
 *	of the given type.  This code must be executed while
 *	FORBIDDEN (it accesses shared system structures).
 */
ULONG
maxsize (t)
    unsigned long t;
{
    /* THIS CODE MUST ALWAYS BE CALLED WHILE FORBIDDEN */
    ULONG size = 0;
    struct MemHeader *mem;
    struct ExecBase *eb = SysBase;

    for (mem = (struct MemHeader *)eb->MemList.lh_Head;
	    mem->mh_Node.ln_Succ;
	    mem = (struct MemHeader *)mem->mh_Node.ln_Succ) {
	if (mem -> mh_Attributes & t) {
	    size += ((ULONG) mem -> mh_Upper - (ULONG) mem -> mh_Lower);
	}
    }

    return size;
}


char *fmt = "%-5s  %8ld  %8ld  %8ld  %8ld\n";

main()
{
    ULONG   max1,
            max2,
            avail1,
            avail2,
            largest1,
            largest2;

    /*       ssss   dddddddd  dddddddd  dddddddd  dddddddd  */
    printf ("Type  Available    In-Use   Maximum   Largest\n");

    Forbid ();
    max1 = maxsize (MEMF_CHIP);
    avail1 = AvailMem (MEMF_CHIP);
    largest1 = AvailMem (MEMF_CHIP | MEMF_LARGEST);
    Permit ();

    printf (fmt, "chip", avail1, max1-avail1, max1, largest1);

    Forbid ();
    max2 = maxsize (MEMF_FAST);
    avail2 = AvailMem (MEMF_FAST);
    largest2 = AvailMem (MEMF_FAST | MEMF_LARGEST);
    Permit ();

    printf (fmt, "fast", avail2, max2-avail2, max2, largest2);

    printf (fmt, "total", avail1+avail2, (max1+max2)-(avail1+avail2),
	 max1+max2, largest1+largest2);
}
