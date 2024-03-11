#ifndef NECROCELL_H
#define NECROCELL_H

#include "SDL.h"

const int CELL_WIDTH = 64;
const int CELL_HEIGHT = 32;

// Forward Declaration
class World;

//-------------------------------------------------
// A Cell represents a container for each tile in
// the World. It has a base object (tile), collision
// and entity information.
// 
// We can flag as composite Objects that will occupy 
// more than one cell, and return all the cells that 
// they occupy.
//-------------------------------------------------

class Cell
{

private:
	World* world; // refers to this cell's world

	int cellX, cellY;
	SDL_Rect cellRect;
	SDL_Texture* baseTexture;		// TODO this is just for quick testing, will have to structure this properly


public:
	Cell();

	int				GetX() const;
	int				GetY() const;

	void			SetWorld(World* w);
	void			SetCellCoordinates(const int x, const int y);
	void			SetBaseTexture(SDL_Texture* texture);

	void			DrawCell();
};


inline int Cell::GetX() const
{
	return cellX;
}

inline int Cell::GetY() const
{
	return cellY;
}

#endif
