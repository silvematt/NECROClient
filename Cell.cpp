#include "Cell.h"

#include "NECROEngine.h"
#include "Utility.h"
#include "Math.h"

//--------------------------------------
// Constructor
//--------------------------------------
Cell::Cell()
{
	world = nullptr;
	cellX = cellY = 0;
	isoX = isoY = 0,
	dstRect = { 0,0,0,0 };
	baseTexture = nullptr;
}

//--------------------------------------
// Sets the coordinates of this cells 
// relative to its world
//--------------------------------------
void Cell::SetCellCoordinates(const int x, const int y)
{
	cellX = x;
	cellY = y;

	NMath::CartToIso(cellX, cellY, isoX, isoY);

	// Adjust isoX so that the top of the image becomes the origin
	isoX -= HALF_CELL_WIDTH;

	// Adjust isoX and isoY to the world offset
	isoX += engine.GetGame().GetMainCamera()->pos.x;
	isoY += engine.GetGame().GetMainCamera()->pos.y;
	isoY -= dstRect.h; // bottom-left origin

	dstRect = { isoX, isoY, CELL_WIDTH, CELL_HEIGHT };
}

//--------------------------------------
// Sets the world pointer 
//--------------------------------------
void Cell::SetWorld(World* w)
{
	world = w;
}

//--------------------------------------
// Sets the baseTexture 
//--------------------------------------
void Cell::SetBaseTexture(SDL_Texture* texture)
{
	baseTexture = texture;
}

//--------------------------------------
// Updates the Cell and its content
//--------------------------------------
void Cell::Update()
{
	NMath::CartToIso(cellX, cellY, isoX, isoY);

	// Adjust isoX so that the top of the image becomes the origin
	isoX -= HALF_CELL_WIDTH;

	// Adjust isoX and isoY to the world offset
	isoX += engine.GetGame().GetMainCamera()->pos.x;
	isoY += engine.GetGame().GetMainCamera()->pos.y;
	isoY -= dstRect.h; // bottom-left origin

	dstRect = { isoX, isoY, CELL_WIDTH, CELL_HEIGHT };
	for (auto& ent : entities)
		if(ent)
			ent->Update();
}

//-----------------------------------------------------------------------
// Adds an Entity to the entities vector, transferring ownership
//-----------------------------------------------------------------------
void Cell::AddEntity(std::unique_ptr<Entity>&& e)
{
	e->SetOwner(this);
	entities.emplace_back(std::move(e));
}

//--------------------------------------
// Removes an entity from the vector
//--------------------------------------
void Cell::RemoveEntity(size_t idx)
{
	entities.at(idx)->ClearOwner();
	Utility::RemoveFromVector(entities, idx);
}

//------------------------------------------------------------------------------------------------------------
// Returns the position of the entity with id = entID inside the entities vector, or size_t::max if not found
//------------------------------------------------------------------------------------------------------------
size_t Cell::GetEntityPos(uint32_t entID)
{
	for (int i = 0; i < entities.size(); i++)
	{
		auto& e = entities.at(i);
		if (e && e->GetID() == entID)
			return i;
	}

	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Trying to get entity pos of ID: %d, but it is not present in the cell the call was made in.\n", entID);
	return std::numeric_limits<size_t>::max();
}

//--------------------------------------
// Draws this cell
//--------------------------------------
void Cell::DrawCell()
{
	engine.GetRenderer().DrawImageDirectly(baseTexture, NULL, &dstRect);
}

//--------------------------------------
// Draws all the entities of this cell
//--------------------------------------
void Cell::DrawEntities()
{
	for (auto& ent : entities)
		if(ent)
			ent->Draw();
}
