#ifndef NECROCOLLIDER_H
#define NECROCOLLIDER_H

#include "SDL.h"

#include "Vector2.h"
#include "Image.h"

class Entity;

//-------------------------------------------------------------------------------------
// A collider is attached to an Entity (or classes derived from Entity) and is used to
// define the collision box of the entities.
// 
// TODO: For dynamic entities that move such as the player and projectiles, we can test
// collision against all entities that are near (in terms of the gridmap)
// so we test the collision ONLY against entities that are close to the dynamic entity
//-------------------------------------------------------------------------------------
class Collider
{
public:
	Collider() = default;

public:
	void Init(bool pEnabled, Entity* own, int x, int y, int w, int h);
	void Update();

private:
	Entity* owner;
	Image* debugImg;

	Vector2 collOffset;			// the offset of the collision box 'r' added to r.x and r.y (which represent the position of the entity owning this collider)
	SDL_Rect r;

	float isoPosX, isoPosY;

public:
	bool enabled = false;

	void			SetOffset(float x, float y);
	void			DebugDraw();
	const SDL_Rect*	GetRect() const;

	bool			TestIntersection(const Collider* other);
};


inline const SDL_Rect* Collider::GetRect() const
{
	return &r;
}

#endif
