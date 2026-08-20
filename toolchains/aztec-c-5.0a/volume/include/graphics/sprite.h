#ifndef	GRAPHICS_SPRITE_H
#define	GRAPHICS_SPRITE_H
#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif
#define	SPRITE_ATTACHED	0x80
struct	SimpleSprite
{
UWORD	*posctldata;
UWORD	height;
UWORD	x,y;
UWORD	num;
};
#endif
