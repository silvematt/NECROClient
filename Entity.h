#ifndef NECROENTITY_H
#define NECROENTITY_H

#include "Vector2.h"
#include "Image.h"
#include "Collider.h"

class Cell;

//-----------------------------------------------------------------------------
// An Enity is the base unit of something that exists inside of a Cell.
//-----------------------------------------------------------------------------
class Entity
{
	friend class Prefab;
	friend class Animator; // Animator is a friend class of Entity

private:
	static uint32_t ENT_NEXT_ID; // EntityID static track

protected:
	uint32_t ID;				// EntityID
	Image* img;
	Cell* owner;				// Owner of this entity
	Cell* nextOwner;			// Used to TransferToCellQueue()

	Collider coll;

	// Used for entities that uses tilesets, index of X and Y, they will be multiplied by img->GetTileset().tileWidth and img->GetTileset().tileHeight
	int tilesetXOff, tilesetYOff;

	// DST used for occlusion testing
	int occlModifierX, occlModifierY; // used to help shape the occlusion box starting from the dst rect
	SDL_Rect occlusionRect;

	bool occludes = false;		// if true, it will be drawn with OCCLUDED_SPRITE_ALPHA_VALUE

public:
	virtual ~Entity();
	Entity();
	Entity(Vector2 pInitialPos, Image* pImg);

	Vector2 pos;				// orthographic pos
	int gridPosX, gridPosY;		// Position in the gridmap

	Vector2 isoPos;				// isometric pos (used only for rendering)

public:
	const uint32_t	GetID() const;
	Collider&		GetCollider();
	Cell*			GetOwner();
	bool			Occludes();

	void			SetImg(Image* pImg);
	void			SetOwner(Cell* c);
	void			ClearOwner();
	void			TransferToCellImmediately(Cell* c);		// Transfer this entity to exist in another cell 
	void			TransferToCellQueue(Cell* c);			// Transfer this entity to exist in another cell AFTER a world update completes
	void			SetOccludes(bool val);

	virtual void	Update();
	virtual void	Draw();
};

inline const uint32_t Entity::GetID() const
{
	return ID;
}

inline Cell* Entity::GetOwner()
{
	return owner;
}

inline Collider& Entity::GetCollider()
{
	return coll;
}

inline void Entity::SetOccludes(bool val)
{
	occludes = val;
}

inline bool Entity::Occludes()
{
	return occludes;
}

#endif
