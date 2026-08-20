// sprite.h
//
// (C) 1987, MTS Associates, Inc.
#ifndef GRAPHICS_SPRITE_H
#define GRAPHICS_SPRITE_H
#define SPRITE_ATTACHED 0x80

#ifndef GRAPHICS_RASTPORT_H
#include <graphics/rastport.h>
#endif
#ifndef GRAPHICS_VIEW_H
#include <graphics/view.h>
#endif

struct SimpleSprite
{
    UWORD *posctldata;
    UWORD height;
    UWORD   x,y;
    UWORD   num;
};

void ChangeSprite(ViewPort *vp,  SimpleSprite *s, APTR x);
SHORT GetSprite(SimpleSprite *s, const SHORT num);
void MoveSprite(RastPort *r,  SimpleSprite *s, const SHORT dx,
	const SHORT dy);

#endif
