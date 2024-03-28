#ifndef NECROCELL_H
#define NECROCELL_H

#include <vector>
#include <memory>

#include "SDL.h"

#include "Entity.h"

const int CELL_WIDTH = 64;
const int CELL_HEIGHT = 32;

const int HALF_CELL_WIDTH = 32;
const int HALF_CELL_HEIGHT = 16;

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
	int isoX, isoY;

	SDL_Rect dstRect;				// dstRect used to DrawImageDirectly
	SDL_Texture* baseTexture;		// TODO this is just for quick testing, will have to structure this properly

	// Entities set in this cell
	std::vector<std::unique_ptr<Entity>> entities;

public:
	Cell();

	int				GetCellX() const;
	int				GetCellY() const;

	SDL_Rect&		GetDstRect();
	World*			GetWorld();

	void			SetWorld(World* w);
	void			SetCellCoordinates(const int x, const int y);
	void			SetBaseTexture(SDL_Texture* texture);

	void			Update();
	size_t			GetEntitiesSize() const;
	void			AddEntity(std::unique_ptr<Entity>&& e); // transfer ownership
	void			RemoveEntity(size_t indx);

	std::vector<std::unique_ptr<Entity>>&		GetEntities();
	size_t										GetEntityPos(uint32_t entID);

	void			DrawCell();
	void			DrawEntities();
};


inline int Cell::GetCellX() const
{
	return cellX;
}

inline int Cell::GetCellY() const
{
	return cellY;
}

inline SDL_Rect& Cell::GetDstRect()
{
	return dstRect;
}

inline size_t Cell::GetEntitiesSize() const
{
	return entities.size();
}

inline std::vector<std::unique_ptr<Entity>>& Cell::GetEntities()
{
	return entities;
}

inline World* Cell::GetWorld()
{
	return world;
}
#endif
