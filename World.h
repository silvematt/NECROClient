#ifndef NECROWORLD_H
#define NECROWORLD_H

#include <unordered_map>
#include <memory>

#include "Cell.h"

const int WORLD_WIDTH  = 32;
const int WORLD_HEIGHT = 32;

//-------------------------------------------------
// A World represents a level or a map with its
// cells and entities that lives in them.
// 
// The World is composed of a 2D array of Cells.
//-------------------------------------------------
class World
{
private:
	Cell worldmap[WORLD_WIDTH][WORLD_HEIGHT];
	Cell* worldCursor = nullptr;					// The cell the mouse is currently hovering on (if any)
	SDL_Texture* worldCursorTexture;

	//--------------------------------------------------------------------------------------
	// Entities physically exist and are phyiscally owned by the World they're spawned in.
	// Every entity exists inside the allEntities unordered_map, indexed by EntityID.
	// 
	// Entities are logically owned by Cells, where each Cell stores a vector of raw pointers
	// to the allEntities they logically own. That means we can quickly get any entity by ID
	// and given the Cell, we can quickly get all the entities contained in that cell.
	//--------------------------------------------------------------------------------------
	std::unordered_map<uint32_t, std::unique_ptr<Entity>> allEntities;

	// See Entity::TransferToCellImmediately
	std::vector<Entity*> entitiesWaitingForTransfer;

public:
	Cell* GetCellAt(int x, int y);

	const std::unordered_map<uint32_t, std::unique_ptr<Entity>>& GetEntities();

	void			AddEntity(std::unique_ptr<Entity>&& e);
	void			RemoveEntity(uint32_t atID);

	void			InitializeWorld();
	void			Update();

	void			AddPendingEntityToTransfer(Entity* e);
	void			TransferPendingEntities();		// Transferring the entity is done AFTER a world update, otherwise if an entity is fast enough (or framerate is low enough) 
													// the entity can be udated multiple times if the update is done through iterating over the worldmap[][] instead of the allEntities map
	void			Draw();
};

inline const std::unordered_map<uint32_t, std::unique_ptr<Entity>>& World::GetEntities()
{
	return allEntities;
}

#endif
