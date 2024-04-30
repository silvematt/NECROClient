#ifndef NECROLIGHT_H
#define NECROLIGHT_H

#include "SDL.h"
#include "Vector2.h"

//-----------------------------------------------------------------------------------------------------------------
// A light is attached to an Entity (or classes derived from Entity)
// 
// Lighting works this way: each World has a base color and intensity, each Cell in the world has its own
// color and intensity (initially derived from the world) that defines the color of the entities in it. 
// 
// Each light modify the Cells' color and intensity in their radius.
//-----------------------------------------------------------------------------------------------------------------
class Light
{
	Vector2 pos;
	float intensity;
	float radius;

	SDL_Color color;
};

#endif
