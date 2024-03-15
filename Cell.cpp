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
	isoX += WORLD_RENDER_OFFSET_X;
	isoY += WORLD_RENDER_OFFSET_Y;

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
	for (auto& ent : entities)
		ent.Update();
}

//--------------------------------------
// Adds an Entity to the entities vector 
//--------------------------------------
void Cell::AddEntity(Entity e)
{
	e.SetOwner(this);
	entities.push_back(e);
}

//--------------------------------------
// Removes an entity from the vector
//--------------------------------------
void Cell::RemoveEntity(size_t idx)
{
	entities.at(idx).ClearOwner();
	Utility::RemoveFromVector(entities, idx);
}

Entity* Cell::GetEntityAt(size_t indx)
{
	if (entities.size() > indx)
		return &entities[indx];
	else
		return nullptr;
}

//--------------------------------------
// Draws this cells TODO: will not have 
// to draw directly to the innerRenderer
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
		ent.Draw();
}
