#ifndef GRAPHICS_SPRITE_H
#define GRAPHICS_SPRITE_H





#define SPRITE_ATTACHED 0x80

struct SimpleSprite
{
    UWORD *posctldata;
    UWORD height;
    UWORD   x,y;    
    UWORD   num;
};
#endif
