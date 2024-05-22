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

	// Entity ptrs logically owned by this cell
	std::vector<Entity*> entities;

	// Lighting
	SDL_Color lColor;
	float lIntensity;

public:
	Cell();

	int				GetCellX() const;
	int				GetCellY() const;

	SDL_Rect&		GetDstRect();
	World*			GetWorld();

	SDL_Color*		GetLightingColor();
	float&			GetLightingIntensity();

	void			SetWorld(World* w);
	void			SetCellCoordinates(const int x, const int y);
	void			SetBaseTexture(SDL_Texture* texture);

	void			Update();

	void			AddEntityPtr(Entity* e);
	void			RemoveEntityPtr(uint32_t remID);
	void			RemoveEntityPtr(size_t indx);
	size_t			GetEntitiesPtrSize() const;

	Entity*			GetEntityPtr(uint32_t atID);
	Entity*			GetEntityPtrAt(size_t indx);

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

inline World* Cell::GetWorld()
{
	return world;
}

inline size_t Cell::GetEntitiesPtrSize() const
{
	return entities.size();
}

inline SDL_Color* Cell::GetLightingColor()
{
	return &lColor;
}

inline float& Cell::GetLightingIntensity()
{
	return lIntensity;
}

#endif
