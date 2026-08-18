//
// devices/keymap.h
//
// (C) 1987, MTS Associates
//
#ifndef  DEVICES_KEYMAP_H
#define  DEVICES_KEYMAP_H
#ifndef EXEC_NODES_H
#include        "exec/nodes.h"
#endif
#ifndef EXEC_LISTS_H
#include        "exec/lists.h"
#endif
struct   KeyMap {
    UBYTE   *km_LoKeyMapTypes;
    ULONG   *km_LoKeyMap;
    UBYTE   *km_LoCapsable;
    UBYTE   *km_LoRepeatable;
    UBYTE   *km_HiKeyMapTypes;
    ULONG   *km_HiKeyMap;
    UBYTE   *km_HiCapsable;
    UBYTE   *km_HiRepeatable;
};

struct  KeyMapNode : public Node {
    KeyMap kn_KeyMap;
};

struct  KeyMapResource : public Node {
    List kr_List;
};
#define  KC_NOQUAL   0
#define  KC_VANILLA  7
#define  KCB_SHIFT   0
#define  KCF_SHIFT   0x01
#define  KCB_ALT     1
#define  KCF_ALT     0x02
#define  KCB_CONTROL 2
#define  KCF_CONTROL 0x04
#define  KCB_DOWNUP  3
#define  KCF_DOWNUP  0x08
#define  KCB_DEAD    5
#define  KCF_DEAD    0x20
#define  KCB_STRING  6
#define  KCF_STRING  0x40
#define  KCB_NOP     7
#define  KCF_NOP     0x80
#define DPB_MOD         0
#define DPF_MOD         0x01
#define DPB_DEAD        3
#define DPF_DEAD        0x08
#define DP_2DINDEXMASK  0x0f
#define DP_2DFACSHIFT   4
#endif
