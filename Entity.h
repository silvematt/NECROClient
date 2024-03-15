#ifndef NECROENTITY_H
#define NECROENTITY_H

#include "Vector2.h"
#include "Image.h"

class Cell;

//-----------------------------------------------------------------------------
// An Enity is the base unit of something that exists inside of a Cell.
//-----------------------------------------------------------------------------
class Entity
{
private:
	Image* img;
	Cell* owner;				// Owner of this entity
	
public:
	Vector2 pos;				// orthographic pos
	Vector2 isoPos;				// isometric pos (used only for rendering)

private:

public:
	void			SetImg(Image* pImg);
	void			SetOwner(Cell* c);
	void			ClearOwner();

	void			Init(Vector2 pInitialPos, Image* pImg);
	void			Update();
	void			Draw();
};

#endif
