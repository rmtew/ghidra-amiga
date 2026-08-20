// Graphics/text.h
//
// (C) 1987, MTS Associates
//
#ifndef  GRAPHICS_TEXT_H
#define  GRAPHICS_TEXT_H
#ifndef     EXEC_PORTS_H
#include "exec/ports.h"
#endif
#ifndef  GRAPHICS_RASTPORT_H
#include "graphics/rastport.h"
#endif

#define FS_NORMAL    0
#define FSB_EXTENDED 3
#define FSF_EXTENDED (1<<3)
#define FSB_ITALIC   2
#define FSF_ITALIC   (1<<2)
#define FSB_BOLD     1
#define FSF_BOLD     (1<<1)
#define FSB_UNDERLINED  0
#define FSF_UNDERLINED  (1<<0)
#define FPB_ROMFONT  0
#define FPF_ROMFONT  (1<<0)
#define FPB_DISKFONT 1
#define FPF_DISKFONT (1<<1)
#define FPB_REVPATH  2
#define FPF_REVPATH  (1<<2)
#define  FPB_TALLDOT 3
#define  FPF_TALLDOT (1<<3)
#define  FPB_WIDEDOT 4
#define  FPF_WIDEDOT (1<<4)
#define FPB_PROPORTIONAL   5
#define FPF_PROPORTIONAL   (1<<5)
#define FPB_DESIGNED 6
#define FPF_DESIGNED (1<<6)
#define FPB_REMOVED  7
#define FPF_REMOVED  (1<<7)

struct TextAttr {
   STRPTR   ta_Name;
   UWORD    ta_YSize;
   UBYTE    ta_Style;
   UBYTE    ta_Flags;
};

struct TextFont : public Message {
   UWORD    tf_YSize;
   UBYTE    tf_Style;
   UBYTE    tf_Flags;
   UWORD    tf_XSize;
   UWORD    tf_Baseline;
   UWORD    tf_BoldSmear;
   UWORD    tf_Accessors;
   UBYTE    tf_LoChar;
   UBYTE    tf_HiChar;
   APTR     tf_CharData;
   UWORD    tf_Modulo;
   APTR     tf_CharLoc;
   APTR     tf_CharSpace;
   APTR     tf_CharKern;
};

void AddFont (TextFont *tf);
void AskFont (const RastPort *rp, TextAttr *ta);
int AskSoftStyle(const RastPort *rp);
void ClearEOL (const RastPort *rp);
void ClearScreen (const RastPort *rp);
void CloseFont (TextFont *t);
TextFont *OpenFont(const TextAttr *t);
int RemFont(TextFont *tf);
int SetFont(RastPort *rp, TextFont *tf);
int SetSoftStyle(RastPort *rp, const int style, const int enable);
int numPixels(RastPort *rp, const TEXT *t, int chars);
#endif
