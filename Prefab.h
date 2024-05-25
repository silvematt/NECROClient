#ifndef NECROPREFAB_H
#define NECROPREFAB_H

#include <string>
#include <memory>

#include "SDL.h"
#include "Entity.h"

class Prefab
{
private:
	std::string pName;
	std::string pImgFile;
	bool isStatic;

	bool hasCollider;
	SDL_Rect collRect;
	int collOffsetX;
	int collOffsetY;

	bool occlCheck;
	int occlModX;
	int occlModY;

	bool blocksLight;
	float blocksLightValue;

	bool emitsLight;
	int lightPropagationType; // flat, raycast, etc
	float lightRadius;
	float lightIntensity;
	float lightDropoffMultiplier; // how much reduction of light there is for cells that are far from the source
	float lightFarDropoffThreshold; // the dropoff distance (in cells) from which the lightFarDropoffMultiplier is applied on top of the base dropoff
	float lightFarDropoffMultiplier;
	int lightR;
	int lightG;
	int lightB;
	bool lightAnimated;
	float lightMinIntensityDivider;
	float lightAnimSpeed;

	bool hasAnimator;
	std::string animFile;

public:
	bool		LoadFromFile(const std::string& filename);
	void		Log();


	std::string GetName();

	static std::unique_ptr<Entity> InstantiatePrefab(const std::string& prefabName, Vector2 pos);
};

inline std::string Prefab::GetName()
{
	return pName;
}

#endif
