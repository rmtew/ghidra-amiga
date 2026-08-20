#ifndef  DEVICES_KEYMAP_H
#define  DEVICES_KEYMAP_H






struct   KeyMap {
   APTR  km_LoKeyMapTypes;
   APTR  km_LoKeyMap;
   APTR  km_LoCapsable;
   APTR  km_LoRepeatable;
   APTR  km_HiKeyMapTypes;
   APTR  km_HiKeyMap;
   APTR  km_HiCapsable;
   APTR  km_HiRepeatable;
};

#define  KCB_NOP     7
#define  KCF_NOP     0x80

#define  KC_NOQUAL   0
#define  KC_VANILLA  7        
#define  KCF_SHIFT   0x01
#define  KCF_ALT     0x02
#define  KCB_CONTROL 2
#define  KCF_CONTROL 0x04
#define  KCB_DOWNUP  3
#define  KCF_DOWNUP  0x08

#define  KCB_STRING  6
#define  KCF_STRING  0x40

#endif
