#ifndef  DEVICES_INPUTEVENT_H
#include <devices/inputevent.h>
#endif
#ifndef EXEC_DEVICES_H
#include <exec/devices.h>
#endif
#ifndef  DEVICES_KEYMAP_H
#include <devices/keymap.h>
#endif
extern struct Library *ConsoleDevice;
struct InputEvent *CDInputHandler(struct InputEvent *, struct Device *);
long RawKeyConvert(struct InputEvent *, char *, long, struct KeyMap *);
#ifndef  NO_PRAGMAS
#pragma libcall ConsoleDevice CDInputHandler 2a 9802
#pragma libcall ConsoleDevice RawKeyConvert 30 a19804
#endif
