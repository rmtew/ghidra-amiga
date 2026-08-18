#ifndef	EXEC_MEMORY_H
#define	EXEC_MEMORY_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif	/* !EXEC_NODES_H */
struct	MemChunk	{
struct	MemChunk	*mc_Next;
ULONG	mc_Bytes;
};
struct	MemHeader	{
struct	Node	mh_Node;
UWORD	mh_Attributes;
struct	MemChunk	*mh_First;
APTR	mh_Lower;
APTR	mh_Upper;
ULONG	mh_Free;
};
struct	MemEntry	{
union	{
ULONG	meu_Reqs;
APTR	meu_Addr;
}	me_Un;
ULONG	me_Length;
};
#define	me_un	me_Un
#define	me_Reqs	me_Un.meu_Reqs
#define	me_Addr	me_Un.meu_Addr
struct	MemList	{
struct	Node	ml_Node;
UWORD	ml_NumEntries;
struct	MemEntry	ml_ME[1];
};
#define	ml_me	ml_ME
#define	MEMF_PUBLIC	(1L<<0)
#define	MEMF_CHIP	(1L<<1)
#define	MEMF_FAST	(1L<<2)
#define	MEMF_CLEAR	(1L<<16)
#define	MEMF_LARGEST	(1L<<17)
#define	MEM_BLOCKSIZE	8L
#define	MEM_BLOCKMASK	7L
#endif
