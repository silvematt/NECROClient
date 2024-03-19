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
protected:
	Image* img;
	Cell* owner;				// Owner of this entity

	// Used for entities that uses tilesets, index of X and Y, they will be multiplied by img->GetTileset().tileWidth and img->GetTileset().tileHeight
	int tilesetXOff, tilesetYOff;
	
public:
	Entity() = default;
	Entity(Vector2 pInitialPos, Image* pImg);

	Vector2 pos;				// orthographic pos
	Vector2 isoPos;				// isometric pos (used only for rendering)

public:
	void			SetImg(Image* pImg);
	void			SetOwner(Cell* c);
	void			ClearOwner();

	virtual void	Update();
	void			Draw();
};

#endif
