#ifndef NECROENTITY_H
#define NECROENTITY_H

#include "Vector2.h"
#include "Image.h"

//-----------------------------------------------------------------------------
// An Enity is the base unit of something that exists inside of a Cell.
//-----------------------------------------------------------------------------
class Entity
{
private:
	Image* img;

public:
	Vector2 pos;		// orthographic pos
	Vector2 isoPos;		// isometric pos (used only for rendering)

private:

public:
	void			SetImg(Image* pImg);

	void			Init(Vector2 initialPos);
	void			Update();
	void			Draw();
};

#endif
