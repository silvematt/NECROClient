#include "NECROEngine.h"

#include "Entity.h"

Entity::Entity(Vector2 pInitialPos, Image* pImg)
{
	pos = pInitialPos;
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

//------------------------------------------------------------
// Updates the Entity
//------------------------------------------------------------
void Entity::Update()
{
	// Update coordinates
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
}
