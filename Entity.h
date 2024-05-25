#ifndef NECROENTITY_H
#define NECROENTITY_H

#include <memory>

#include "Vector2.h"
#include "Image.h"
#include "Collider.h"
#include "Light.h"
#include "Animator.h"

class Cell;

//-----------------------------------------------------------------------------
// An Enity is the base unit of something that exists inside of a Cell.
//-----------------------------------------------------------------------------
class Entity
{
	// Flags that describe entities
	enum Flags
	{
		FCanOccludePlayer = 1,
		FBlocksLight = 2
	};

	friend class Prefab;
	friend class Animator; // Animator is a friend class of Entity

private:
	static uint32_t ENT_NEXT_ID; // EntityID static track

protected:
	uint32_t ID;				// EntityID
	Image* img;
	Cell* owner;				// Owner of this entity
	Cell* nextOwner;			// Used to TransferToCellQueue()

	uint16_t eFlags = 0;		// this entityflags value

	// Used for entities that uses tilesets, index of X and Y, they will be multiplied by img->GetTileset().tileWidth and img->GetTileset().tileHeight
	int tilesetXOff, tilesetYOff;

	int occlModifierX, occlModifierY; // used to help shape the occlusion box starting from the dst rect
	SDL_Rect occlusionRect;
	bool occludes = false;		// if true, it will be drawn with OCCLUDED_SPRITE_ALPHA_VALUE

	SDL_Color lightingColor;	// Calculated in UpdateLighting(), it is the color the entity when drawn (not the color of the light it emits)

	float blocksLightValue = 0.0f;

	// "Components", they are created or not in base of prefab options. An alternative could be std::optional
	std::unique_ptr<Collider> coll;
	std::unique_ptr<Light> emittingLight;
	std::unique_ptr<Animator> anim;
	
public:
	virtual ~Entity();
	Entity();
	Entity(Vector2 pInitialPos, Image* pImg);

	Vector2 pos;				// orthographic pos
	int gridPosX, gridPosY;		// Position in the gridmap

	Vector2 isoPos;				// isometric pos (used only for rendering)

public:
	const uint32_t	GetID() const;
	Cell*			GetOwner();

	void			SetFlag(Flags flag);	// Manage flags
	void			ClearFlag(Flags flag);
	bool			TestFlag(Flags flag);

	// Optional components functions
	void			CreateLight();
	bool			HasLight() const;
	Light*			GetLight() const;

	void			CreateCollider();
	bool			HasCollider() const;
	Collider*		GetCollider() const;

	void			CreateAnimator();
	bool			HasAnimator() const;
	Animator*		GetAnimator() const;

	bool			Occludes();

	void			SetImg(Image* pImg);
	void			SetOwner(Cell* c);
	void			ClearOwner();
	void			TransferToCellImmediately(Cell* c);		// Transfer this entity to exist in another cell 
	void			TransferToCellQueue(Cell* c);			// Transfer this entity to exist in another cell AFTER a world update completes
	void			SetOccludes(bool val);
	void			UpdateLighting();
	bool			BlocksLight();
	float			GetLightBlockValue();

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

inline void Entity::CreateCollider()
{
	if (!HasCollider())
		coll = std::make_unique<Collider>();
}

inline bool Entity::HasCollider() const
{
	return coll != nullptr;
}

inline Collider* Entity::GetCollider() const
{
	return coll.get();
}

inline void Entity::CreateLight()
{
	if (!HasLight())
		emittingLight = std::make_unique<Light>();
}

inline bool Entity::HasLight() const
{
	return emittingLight != nullptr;
}

inline Light* Entity::GetLight() const
{
	return emittingLight.get();
}

inline void Entity::CreateAnimator()
{
	if (!HasAnimator())
		anim = std::make_unique<Animator>();
}

inline bool Entity::HasAnimator() const
{
	return anim != nullptr;
}

inline Animator* Entity::GetAnimator() const
{
	return anim.get();
}

inline void Entity::SetFlag(Flags flag)
{
	eFlags |= flag;
}

inline void Entity::ClearFlag(Flags flag)
{
	eFlags &= ~flag;
}

inline bool Entity::TestFlag(Flags flag)
{
	return ((eFlags & flag) > 0) ? true : false;
}

inline void Entity::SetOccludes(bool val)
{
	occludes = val;
}

inline bool Entity::Occludes()
{
	return occludes;
}

inline bool Entity::BlocksLight()
{
	return TestFlag(FBlocksLight);
}

inline float Entity::GetLightBlockValue()
{
	return blocksLightValue;
}

#endif
