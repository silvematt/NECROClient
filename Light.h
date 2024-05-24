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
public:
	Vector2 pos;
	float intensity;
	float radius;
	float dropoffMultiplier; // how much reduction of light there is for cells that are far from the source
	float farDropoffThreshold; // the dropoff distance (in cells) from which the lightFarDropoffMultiplier is applied on top of the base dropoff
	float farDropoffMultiplier;
	SDL_Color color;

	// Anim parameters
	float animSpeed;
	float minIntensity;
	float maxIntensity;
	float minIntensityDivider;

private:
	// Settings to animate lights
	bool doAnim = false;

	// For pulse lighting effect
	bool goUp = false;
	bool goDown = true;

private:
	void Animate();

public:
	void Init();
	void Update();

	void SetAnim(bool v);
};

#endif
