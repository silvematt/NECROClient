#include "NECROEngine.h"

#include "Entity.h"
#include "Utility.h"

uint32_t Entity::ENT_NEXT_ID = 0;

Entity::Entity()
{
	ID = ENT_NEXT_ID;
	ENT_NEXT_ID++;

	img = nullptr;
	owner = nullptr;
	tilesetXOff = tilesetYOff = 0;
	gridPosX = gridPosY = 0;
}

Entity::Entity(Vector2 pInitialPos, Image* pImg)
{
	ID = ENT_NEXT_ID;
	ENT_NEXT_ID++;

	pos = pInitialPos;
	gridPosX = pos.x / CELL_WIDTH;
	gridPosY = pos.y / CELL_HEIGHT;

	SetImg(pImg);
}

//------------------------------------------------------------
// Sets the Image of this Entity
//------------------------------------------------------------
void Entity::SetImg(Image* pImg)
{
	img = pImg;
}

//------------------------------------------------------------
// Sets the owner of this Entity
//------------------------------------------------------------
void Entity::SetOwner(Cell* c)
{
	owner = c;
}

//------------------------------------------------------------
// Clears the owner of this entity
//------------------------------------------------------------
void Entity::ClearOwner()
{
	owner = nullptr;
}

void Entity::TransferToCell(Cell* c)
{
	Cell* previous = owner;
	int previousPos = owner->GetEntityPos(this->ID);
	c->AddEntity(std::move(owner->GetEntities().at(previousPos)));
	SetOwner(c);
	
	Utility::RemoveFromVector(previous->GetEntities(), previousPos);
}

//------------------------------------------------------------
// Updates the Entity
//------------------------------------------------------------
void Entity::Update()
{
	// Update grid position
	gridPosX = pos.x / CELL_WIDTH;
	gridPosY = pos.y / CELL_HEIGHT;

	// Update ISO coordinates
	NMath::CartToIso(pos.x/CELL_WIDTH, pos.y/CELL_HEIGHT, isoPos.x, isoPos.y);

	isoPos.x -= HALF_CELL_WIDTH;

	// Adjust isoX and isoY to the world offset
	isoPos.x += engine.GetGame().GetMainCamera()->pos.x;
	isoPos.y += engine.GetGame().GetMainCamera()->pos.y;

	if (!img->IsTileset())
		isoPos.y -= img->GetHeight();
	else
		isoPos.y -= img->GetTilesetHeight();

	// Account for the Y offset of the image
	isoPos.x += img->GetXOffset();
	isoPos.y += img->GetYOffset();

	// TODO: may not have to update collider here for static entities, but need to properly set them
	if (coll.enabled)
		coll.Update();
}

//------------------------------------------------------------
// Draws the Entity
//------------------------------------------------------------
void Entity::Draw()
{
	if (!img->IsTileset())
	{
		SDL_Rect dstRect = { static_cast<int>(isoPos.x), static_cast<int>(isoPos.y), img->GetWidth(), img->GetHeight() };
		engine.GetRenderer().DrawImageDirectly(img->GetSrc(), NULL, &dstRect);
	}
	else
	{
		Image::Tileset tset = img->GetTileset();
		SDL_Rect srcRect = { tilesetXOff * tset.tileWidth, tilesetYOff * tset.tileHeight, tset.tileWidth, tset.tileHeight };
		SDL_Rect dstRect = { static_cast<int>(isoPos.x), static_cast<int>(isoPos.y), tset.tileWidth, tset.tileHeight };
		engine.GetRenderer().DrawImageDirectly(img->GetSrc(), &srcRect, &dstRect);
	}

	if (coll.enabled) // && debug collider
		coll.DebugDraw();
}
