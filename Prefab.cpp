#include "Prefab.h"

#include <fstream>

#include "NECROEngine.h"
#include "Entity.h"

// TODO: Organize loading better, if the prefabImage is not loaded, load it
bool Prefab::LoadFromFile(const std::string& filename)
{
	std::ifstream stream(filename);

	if (!stream.is_open())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load prefab at: %s\n", filename.c_str());
		return false;
	}

	std::string curLine;
	std::string curValStr;
	int curValInt;

	// Prefab Name
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	pName = curValStr;

	// Prefab Img
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	pImgFile = curValStr;

	// isStatic
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	isStatic = std::stoi(curValStr);

	// ColliderEnabled
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	colliderEnabled = std::stoi(curValStr);

	// colliderRectX
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	collRect.x = std::stoi(curValStr);

	// colliderRectY
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	collRect.y = std::stoi(curValStr);

	// colliderRectW
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	collRect.w = std::stoi(curValStr);

	// colliderRectH
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	collRect.h = std::stoi(curValStr);

	// ifstream is closed by destructor
	return true;
}

std::unique_ptr<Entity> Prefab::InstantiatePrefab(const std::string& prefabName, Vector2 pos)
{
	Prefab* p = engine.GetAssetsManager().GetPrefab(prefabName);

	if (p)
	{
		std::unique_ptr<Entity> e(new Entity(Vector2(static_cast<float>(pos.x), static_cast<float>(pos.y)), engine.GetAssetsManager().GetImage(p->pImgFile)));
		e->GetCollider().Init(p->colliderEnabled, e.get(), p->collRect.x, p->collRect.y, p->collRect.w, p->collRect.h); // set collision

		return std::move(e);
	}
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Prefab instantiation failed, %s doesn't exist.", prefabName.c_str());
		return NULL;
	}
}

void Prefab::Log()
{
	SDL_Log("LOGGING PREFAB:");
	SDL_Log("Prefab Name: %s", pName.c_str());
	SDL_Log("Prefab File: %s", pImgFile.c_str());
	SDL_Log("IsStatic:    %d", isStatic);
	SDL_Log("CollEnabled: %d", colliderEnabled);
	SDL_Log("CollRect.x:  %d", collRect.x);
	SDL_Log("CollRect.y:  %d", collRect.y);
	SDL_Log("CollRect.w:  %d", collRect.w);
	SDL_Log("CollRect.h:  %d", collRect.h);
	SDL_Log("END");
}