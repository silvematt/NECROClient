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
	bool colliderEnabled;
	SDL_Rect collRect;
	int collOffsetX;
	int collOffsetY;
	bool occlCheck;
	int occlModX;
	int occlModY;

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
