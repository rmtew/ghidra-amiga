ÄàEXEC_MEMORY_HÄEXEC_MEMORY_Hàµå"exec/nodes.h"á
ÉMemChunk{
ÉMemChunk*mc_Next;
ómc_Bytes;
};
ÉMemHeader{
É¨mh_Node;
âmh_Attributes;
ÉMemChunk*mh_First;
îmh_Lower;
îmh_Upper;
ómh_Free;
};
ÉMemEntry{
´{
ómeu_Reqs;
îmeu_Addr;
}me_Un;
óme_Length;
};Äme_un me_UnÄme_Reqs me_Un.meu_ReqsÄme_Addr me_Un.meu_Addr
ÉMemList{
É¨ml_Node;
âml_NumEntries;
ÉMemEntry ml_ME[1];
};Äml_me ml_MEÄMEMF_ANY (0L)ÄMEMF_PUBLIC (1L<<0)ÄMEMF_CHIP (1L<<1)ÄMEMF_FAST (1L<<2)ÄMEMF_LOCAL (1L<<8)ÄMEMF_24BITDMA (1L<<9)ÄMEMF_KICK (1L<<10)ÄMEMF_CLEAR (1L<<16)ÄMEMF_LARGEST (1L<<17)ÄMEMF_REVERSE (1L<<18)ÄMEMF_TOTAL (1L<<19)ÄMEMF_NO_EXPUNGE (1L<<31)ÄMEM_BLOCKSIZE 8LÄMEM_BLOCKMASK (MEM_BLOCKSIZE-1)
ÉMemHandlerData
{
ómemh_RequestSize;
ómemh_RequestFlags;
ómemh_Flags;
};ÄMEMHF_RECYCLE (1L<<0)ÄMEM_DID_NOTHING (0)ÄMEM_ALL_DONE (-1)ÄMEM_TRY_AGAIN (1)á