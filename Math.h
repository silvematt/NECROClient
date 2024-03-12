#ifndef NECROMATH_H
#define NECROMATH_H

#include "Cell.h"

class NMath
{
public:
	static void IsoToCart(int screenx, int screeny, int& mapx, int& mapy);
	static void CartToIso(int mapx, int mapy, int& screenx, int& screeny);
};

inline void NMath::CartToIso(int mapx, int mapy, int& screenx, int& screeny)
{
	screenx = (mapx - mapy) * HALF_CELL_WIDTH;
	screeny = (mapx + mapy) * HALF_CELL_HEIGHT;
}

inline void NMath::IsoToCart(int screenx, int screeny , int& mapx, int& mapy)
{
	// Convert to calculate in floating point
	float screenxf = static_cast<float>(screenx);
	float screenyf = static_cast<float>(screeny);

	// Decimal part is truncated
	mapx = static_cast<int>(screenxf / CELL_WIDTH + screenyf / CELL_HEIGHT);
	mapy = static_cast<int>(screenyf / CELL_HEIGHT - screenxf / CELL_WIDTH);
}


#endif
