
#include "exec/types.h"
#include "exec/exec.h" 
#include "exec/execbase.h" 

extern struct ExecBase *SysBase;

#define SIZES 16
#define MINSIZE 3
#define MAXSIZE SIZES+MINSIZE-1

main(argc,argv)
long argc;
long argv;
{
    short  b;
    short  counts[SIZES];
    struct MemChunk *chunk;
    struct MemHeader *mem;
    struct ExecBase *eb = SysBase;

    for (b = 0; b < SIZES; b++) {
	counts[b] = 0;
    }

    Forbid();
    for (mem = (struct MemHeader *)eb->MemList.lh_Head;
	    mem -> mh_Node.ln_Succ;
	    mem = (struct MemHeader *)mem->mh_Node.ln_Succ) {
	for (chunk = mem -> mh_First; chunk; chunk = chunk -> mc_Next) {
	    for (b = MAXSIZE; b >= MINSIZE; b--) {
		if ((1 << b) & chunk -> mc_Bytes) {
		    ++counts[b - MINSIZE];
		    break;
		}
	    }
	}
    }
    Permit();

    printf (" Free memory size distribution:\n");
    printf ("   Size  Count\n");

    for (b = MINSIZE; b <= MAXSIZE; b++) {
	printf (" %6ld: %4ld\n", (1 << b), counts[b - MINSIZE]);
    }
}
