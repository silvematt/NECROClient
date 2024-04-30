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

	lColor = w->GetBaseLightColor();
	lIntensity = w->GetBaseLightIntensity();
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
		if (ent)
			ent->Update();
}

//-----------------------------------------------------------------------
// Adds an Entity to the entities ptrs vector
//-----------------------------------------------------------------------
void Cell::AddEntityPtr(Entity* e)
{
	e->SetOwner(this); // TODO: make sure adding the entityptr should set ownership, in the future one entity may occupy more than one cell
	entities.push_back(e);
}

//--------------------------------------
// Removes an entity from the vector
//--------------------------------------
void Cell::RemoveEntityPtr(uint32_t remID)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetID() == remID)
		{
			RemoveEntityPtr(i);
			return;
		}
	}
}

//--------------------------------------
// Removes an entity from the vector
//--------------------------------------
void Cell::RemoveEntityPtr(size_t idx)
{
	entities.at(idx)->ClearOwner();
	Utility::RemoveFromVector(entities, idx);
}


Entity* Cell::GetEntityPtr(uint32_t atID)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetID() == atID)
		{
			return GetEntityPtrAt(i);
		}
	}

	return nullptr;
}

Entity* Cell::GetEntityPtrAt(size_t indx)
{
	if (indx < entities.size())
		return entities.at(indx);

	return nullptr;
}

//--------------------------------------
// Draws this cell
//--------------------------------------
void Cell::DrawCell()
{
	// Save texture's color
	Uint8 previousR, previousG, previousB;
	SDL_GetTextureColorMod(baseTexture, &previousR, &previousG, &previousB);

	// Update Color with color data
	SDL_SetTextureColorMod(baseTexture, lColor.r * lIntensity, lColor.g * lIntensity, lColor.b * lIntensity);

	engine.GetRenderer().DrawImageDirectly(baseTexture, NULL, &dstRect);

	// Resotre previous color mod
	SDL_SetTextureColorMod(baseTexture, previousR, previousG, previousB);
}

//--------------------------------------
// Draws all the entities of this cell
//--------------------------------------
void Cell::DrawEntities()
{
	for (auto& ent : entities)
		if (ent)
			ent->Draw();
}
