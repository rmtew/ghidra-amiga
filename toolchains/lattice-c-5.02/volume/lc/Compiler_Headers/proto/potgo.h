extern struct Library *PotgoBase;
long AllocPotBits(long);
void FreePotBits(long);
void WritePotgo(long, long);
#ifndef  NO_PRAGMAS
#pragma libcall PotgoBase AllocPotBits 6 1
#pragma libcall PotgoBase FreePotBits c 1
#pragma libcall PotgoBase WritePotgo 12 1002
#endif
