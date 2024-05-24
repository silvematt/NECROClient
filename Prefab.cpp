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
	hasCollider = std::stoi(curValStr);

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

	// collOffsetX
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	collOffsetX = std::stoi(curValStr);

	// collOffsetY
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	collOffsetY = std::stoi(curValStr);

	// occlCheck
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	occlCheck = std::stoi(curValStr);

	// occlModX
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	occlModX = std::stoi(curValStr);

	// occlModX
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	occlModY = std::stoi(curValStr);

	// BlocksLight
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	blocksLight = std::stoi(curValStr);

	// blocksLightValue
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	blocksLightValue = std::stof(curValStr);

	// EmitsLight
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	emitsLight = std::stoi(curValStr);

	// LightRadius
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightRadius = std::stof(curValStr);

	// Light Intensity
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightIntensity = std::stof(curValStr);

	// Dropoff multiplier
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightDropoffMultiplier = std::stof(curValStr);

	// lightFarDropoffThreshold
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightFarDropoffThreshold = std::stof(curValStr);

	// lightFarDropoffMultiplier
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightFarDropoffMultiplier = std::stof(curValStr);

	// LightR
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightR = std::stoi(curValStr);

	// LightG
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightG = std::stoi(curValStr);

	// LightB
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightB = std::stoi(curValStr);

	// lightAnimated
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightAnimated = std::stoi(curValStr);

	// lightMinIntensityDivider
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightMinIntensityDivider = std::stof(curValStr);

	// lightAnimSpeed
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	lightAnimSpeed = std::stof(curValStr);


	// ifstream is closed by destructor
	return true;
}

std::unique_ptr<Entity> Prefab::InstantiatePrefab(const std::string& prefabName, Vector2 pos)
{
	Prefab* p = engine.GetAssetsManager().GetPrefab(prefabName);

	if (p)
	{
		// Create the entity
		std::unique_ptr<Entity> e(new Entity(Vector2(static_cast<float>(pos.x), static_cast<float>(pos.y)), engine.GetAssetsManager().GetImage(p->pImgFile)));

		// Check if the prefab has a collider
		if (p->hasCollider)
		{
			// If so, create and set it
			e->CreateCollider();
			e->GetCollider()->Init(true, e.get(), p->collRect.x, p->collRect.y, p->collRect.w, p->collRect.h);
			e->GetCollider()->SetOffset(p->collOffsetX, p->collOffsetY);
		}
		
		// Check occlusion
		if(p->occlCheck)
			e->SetFlag(Entity::Flags::FCanOccludePlayer);

		e->occlModifierX = p->occlModX;
		e->occlModifierY = p->occlModY;

		// Light block
		if (p->blocksLight)
			e->SetFlag(Entity::Flags::FBlocksLight);

		e->blocksLightValue = p->blocksLightValue;

		// Check Lighting
		if (p->emitsLight)
		{
			e->CreateLight();

			Light* thisLight = e->GetLight();

			thisLight->color.r = p->lightR;
			thisLight->color.g = p->lightG;
			thisLight->color.b = p->lightB;
			thisLight->intensity = p->lightIntensity;
			thisLight->radius = p->lightRadius;
			thisLight->dropoffMultiplier = p->lightDropoffMultiplier;
			thisLight->farDropoffThreshold = p->lightFarDropoffThreshold;
			thisLight->farDropoffMultiplier = p->lightFarDropoffMultiplier;

			// Pos relative to entity
			thisLight->pos.x = 0;
			thisLight->pos.y = 0;

			// Animated
			thisLight->SetAnim(p->lightAnimated);
			thisLight->minIntensityDivider = p->lightMinIntensityDivider;
			thisLight->animSpeed = p->lightAnimSpeed;

			// Init
			thisLight->Init(e.get());
		}

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
	SDL_Log("CollEnabled: %d", hasCollider);
	SDL_Log("CollRect.x:  %d", collRect.x);
	SDL_Log("CollRect.y:  %d", collRect.y);
	SDL_Log("CollRect.w:  %d", collRect.w);
	SDL_Log("CollRect.h:  %d", collRect.h);
	SDL_Log("CollOff.x:   %d", collOffsetX);
	SDL_Log("CollOff.y:   %d", collOffsetY);
	SDL_Log("OcclCheck:   %d", occlCheck);
	SDL_Log("OcclModX:	  %d", occlModX);
	SDL_Log("OcclModY:	  %d", occlModY);
	SDL_Log("EmitsLight:  %d", emitsLight);
	SDL_Log("LightRadius: %f", lightRadius);
	SDL_Log("LIntensity:  %f", lightIntensity);
	SDL_Log("LightColorR: %d", lightR);
	SDL_Log("LightColorG: %d", lightG);
	SDL_Log("LightColorB: %d", lightB);
	SDL_Log("LightAnim:   %d", lightAnimated);
	SDL_Log("LMinIntDivid:%f", lightMinIntensityDivider);
	SDL_Log("LAnimSpeed:  %f", lightAnimSpeed);
	SDL_Log("END");
}
