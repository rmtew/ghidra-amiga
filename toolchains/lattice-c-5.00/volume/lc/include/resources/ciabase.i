********************************************************************
*               Commodore-Amiga, Inc.                              *
*               ciabase.i                                          *
********************************************************************

*----------------------------------------------------------------
*
*  CIA Resource Data Definition
*
*----------------------------------------------------------------


 STRUCTURE  CIAR,LIB_SIZE
    APTR    CR_HWADDR
    UWORD   CR_IntMask
    UBYTE   CR_IEnable
    UBYTE   CR_IActive
    STRUCT  CR_INTNODE,IS_SIZE
    STRUCT  CR_IVTA,IV_SIZE
    STRUCT  CR_IVTB,IV_SIZE
    STRUCT  CR_IVALRM,IV_SIZE
    STRUCT  CR_IVSP,IV_SIZE
    STRUCT  CR_IVFLG,IV_SIZE
    LABEL   CR_SIZE
