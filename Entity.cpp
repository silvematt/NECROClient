#include "NECROEngine.h"

#include "Entity.h"

//------------------------------------------------------------
// Sets the Image of this entity
//------------------------------------------------------------
void Entity::SetImg(Image* pImg)
{
	img = pImg;
}

//------------------------------------------------------------
// Initializes the Entity
//------------------------------------------------------------
void Entity::Init(Vector2 initialPos)
{
	pos = initialPos;
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
	isoPos.x += WORLD_RENDER_OFFSET_X;
	isoPos.y += WORLD_RENDER_OFFSET_Y;

	// Account for the Y offset of the image
	isoPos.y += img->GetYOffset();
}

//------------------------------------------------------------
// Draws the Entity
//------------------------------------------------------------
void Entity::Draw()
{
	SDL_Rect dstRect = { static_cast<int>(isoPos.x), static_cast<int>(isoPos.y), img->GetWidth(), img->GetHeight() };
	engine.GetRenderer().DrawImageDirectly(img->GetSrc(), NULL, &dstRect);
}