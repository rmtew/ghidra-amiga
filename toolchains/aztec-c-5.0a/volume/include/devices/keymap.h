#ifndef	DEVICES_KEYMAP_H
#define	DEVICES_KEYMAP_H
#ifndef	EXEC_NODES_H
#include	"exec/nodes.h"
#endif
#ifndef	EXEC_LISTS_H
#include	"exec/lists.h"
#endif
struct	KeyMap	{
UBYTE	*km_LoKeyMapTypes;
ULONG	*km_LoKeyMap;
UBYTE	*km_LoCapsable;
UBYTE	*km_LoRepeatable;
UBYTE	*km_HiKeyMapTypes;
ULONG	*km_HiKeyMap;
UBYTE	*km_HiCapsable;
UBYTE	*km_HiRepeatable;
};
struct	KeyMapNode	{
struct	Node	kn_Node;
struct	KeyMap	kn_KeyMap;
};
struct	KeyMapResource	{
struct	Node	kr_Node;
struct	List	kr_List;
};
#define	KC_NOQUAL	0L
#define	KC_VANILLA	7L
#define	KCB_SHIFT	0L
#define	KCF_SHIFT	0x01L
#define	KCB_ALT	1L
#define	KCF_ALT	0x02L
#define	KCB_CONTROL	2L
#define	KCF_CONTROL	0x04L
#define	KCB_DOWNUP	3L
#define	KCF_DOWNUP	0x08L
#define	KCB_DEAD	5L
#define	KCF_DEAD	0x20L
#define	KCB_STRING	6L
#define	KCF_STRING	0x40L
#define	KCB_NOP	7L
#define	KCF_NOP	0x80L
#define	DPB_MOD	0L
#define	DPF_MOD	0x01L
#define	DPB_DEAD	3L
#define	DPF_DEAD	0x08L
#define	DP_2DINDEXMASK	0x0fL
#define	DP_2DFACSHIFT	4L
#endif
