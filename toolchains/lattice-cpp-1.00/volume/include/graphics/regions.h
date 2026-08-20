// graphics/regions.h
//
// (C) 1987, MTS Associates
//
#ifndef  GRAPHICS_REGIONS_H
#define GRAPHICS_REGIONS_H
#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif


struct RegionRectangle
{
    RegionRectangle *Next,*Prev;
    Rectangle bounds;

    RegionRectangle *next() { return Next; }
    RegionRectangle *prev() { return Prev; }
};

struct Region : public Rectangle
{
//    Rectangle bounds;
    RegionRectangle *RegionRectangle;

    friend Region * NewRegion();
    friend void DisposeRegion( Region *r);
    Region() { this = NewRegion(); }
    ~Region() { DisposeRegion(this); this = NULL; }


	friend void AndRectRegion( Region *r,  Rectangle *k);
	friend void AndRegionRegion( Region *r1,  Region *r2);

	friend BOOL ClearRectRegion( Region *r,  Rectangle *k);
	friend void ClearRegion( Region *r);
	void clear() { ClearRegion(this); }

	void OrRectRegion( Region *r,  Rectangle *k);
	void OrRegionRegion( Region *r1,  Region *r2);

	void XorRectRegion( Region *r,  Rectangle *k);
	void XorRegionRegion( Region *r1,  Region *r2);

	Region& operator &=(Rectangle *r) { AndRectRegion(this, r); return *this; }
	Region& operator |=(Rectangle *r) { OrRectRegion(this, r); return *this; }
	Region& operator ^=(Rectangle *r) { XorRectRegion(this, r); return *this; }

	Region& operator &=(Region *r) { AndRegionRegion(this, r); return *this; }
	Region& operator |=(Region *r) { OrRegionRegion(this, r); return *this; }
	Region& operator ^=(Region *r) { XorRegionRegion(this, r); return *this; }
};

#endif
