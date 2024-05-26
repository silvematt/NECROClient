#include "Prefab.h"

#include <fstream>

#include "NECROEngine.h"
#include "Entity.h"

// Methods to read lines from the Prefab file
void Prefab::GetIntFromFile(int* v, std::ifstream* stream, std::string* curLine, std::string* curValStr)
{
	std::getline(*stream, *curLine);
	*curValStr = curLine->substr(curLine->find("=") + 2); // key = value;
	*curValStr = curValStr->substr(0, curValStr->find(";"));
	*v = Utility::TryParseInt(*curValStr);
}

void Prefab::GetBoolFromFile(bool* v, std::ifstream* stream, std::string* curLine, std::string* curValStr)
{
	std::getline(*stream, *curLine);
	*curValStr = curLine->substr(curLine->find("=") + 2); // key = value;
	*curValStr = curValStr->substr(0, curValStr->find(";"));
	*v = Utility::TryParseInt(*curValStr);
}

void Prefab::GetFloatFromFile(float* v, std::ifstream* stream, std::string* curLine, std::string* curValStr)
{
	std::getline(*stream, *curLine);
	*curValStr = curLine->substr(curLine->find("=") + 2); // key = value;
	*curValStr = curValStr->substr(0, curValStr->find(";"));
	*v = Utility::TryParseFloat(*curValStr);
}

void Prefab::GetStringFromFile(std::string* v, std::ifstream* stream, std::string* curLine, std::string* curValStr)
{
	std::getline(*stream, *curLine);
	*curValStr = curLine->substr(curLine->find("=") + 2); // key = value;
	*curValStr = curValStr->substr(0, curValStr->find(";"));
	*v = *curValStr;
}

//--------------------------------------------------------
// Loads a Prefab from the file specified in 'filename'
//--------------------------------------------------------
bool Prefab::LoadFromFile(const std::string& filename)
{
	std::ifstream stream(filename);

	SDL_Log("Loading Prefab: '%s'\n", filename.c_str());

	if (!stream.is_open())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load prefab at: %s\n", filename.c_str());
		return false;
	}

	std::string curLine;
	std::string curValStr;
	int curValInt;

	// Prefab Name
	GetStringFromFile(&pName, &stream, &curLine, &curValStr);

	// Prefab Img
	GetStringFromFile(&pImgFile, &stream, &curLine, &curValStr);

	// isStatic
	GetBoolFromFile(&isStatic, &stream, &curLine, &curValStr);

	// PosOffsetX
	GetFloatFromFile(&posOffset.x, &stream, &curLine, &curValStr);

	// PosOffsetY
	GetFloatFromFile(&posOffset.y, &stream, &curLine, &curValStr);

	std::getline(stream, curLine); // line break

	// ColliderEnabled
	GetBoolFromFile(&hasCollider, &stream, &curLine, &curValStr);

	// colliderRectX
	GetIntFromFile(&collRect.x, &stream, &curLine, &curValStr);

	// colliderRectY
	GetIntFromFile(&collRect.y, &stream, &curLine, &curValStr);

	// colliderRectW
	GetIntFromFile(&collRect.w, &stream, &curLine, &curValStr);

	// colliderRectH
	GetIntFromFile(&collRect.h, &stream, &curLine, &curValStr);

	// collOffsetX
	GetIntFromFile(&collOffsetX, &stream, &curLine, &curValStr);

	// collOffsetY
	GetIntFromFile(&collOffsetY, &stream, &curLine, &curValStr);

	std::getline(stream, curLine); // line break

	// occlCheck
	GetBoolFromFile(&occlCheck, &stream, &curLine, &curValStr);

	// occlModX
	GetIntFromFile(&occlModX, &stream, &curLine, &curValStr);

	// occlModY
	GetIntFromFile(&occlModY, &stream, &curLine, &curValStr);

	std::getline(stream, curLine); // line break

	// BlocksLight
	GetBoolFromFile(&blocksLight, &stream, &curLine, &curValStr);

	// blocksLightValue
	GetFloatFromFile(&blocksLightValue, &stream, &curLine, &curValStr);

	std::getline(stream, curLine); // line break

	// EmitsLight
	GetBoolFromFile(&emitsLight, &stream, &curLine, &curValStr);

	// lightPropagationType
	GetIntFromFile(&lightPropagationType, &stream, &curLine, &curValStr);

	// LightRadius
	GetFloatFromFile(&lightRadius, &stream, &curLine, &curValStr);

	// Light Intensity
	GetFloatFromFile(&lightIntensity, &stream, &curLine, &curValStr);

	// Dropoff multiplier
	GetFloatFromFile(&lightDropoffMultiplier, &stream, &curLine, &curValStr);

	// lightFarDropoffThreshold
	GetFloatFromFile(&lightFarDropoffThreshold, &stream, &curLine, &curValStr);

	// lightFarDropoffMultiplier
	GetFloatFromFile(&lightFarDropoffMultiplier, &stream, &curLine, &curValStr);

	// LightR
	GetIntFromFile(&lightR, &stream, &curLine, &curValStr);

	// LightG
	GetIntFromFile(&lightG, &stream, &curLine, &curValStr);

	// LightB
	GetIntFromFile(&lightB, &stream, &curLine, &curValStr);

	// lightAnimated
	GetBoolFromFile(&lightAnimated, &stream, &curLine, &curValStr);

	// lightMinIntensityDivider
	GetFloatFromFile(&lightMinIntensityDivider, &stream, &curLine, &curValStr);

	// lightAnimSpeed
	GetFloatFromFile(&lightAnimSpeed, &stream, &curLine, &curValStr);

	std::getline(stream, curLine); // line break

	// HasAnim
	GetBoolFromFile(&hasAnimator, &stream, &curLine, &curValStr);

	// Anim File name
	GetStringFromFile(&animFile, &stream, &curLine, &curValStr);

	// ifstream is closed by destructor
	return true;
}

//-------------------------------------------------------------------
// Instantiates and returns an Entity unique_ptr from a Prefab
//-------------------------------------------------------------------
std::unique_ptr<Entity> Prefab::InstantiatePrefab(const std::string& prefabName, Vector2 pos)
{
	Prefab* p = engine.GetAssetsManager().GetPrefab(prefabName);

	if (p)
	{
		// Create the entity
		std::unique_ptr<Entity> e(new Entity(Vector2(static_cast<float>(pos.x + p->posOffset.x), static_cast<float>(pos.y + p->posOffset.y)), engine.GetAssetsManager().GetImage(p->pImgFile)));

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

			thisLight->SetPropagationSetting(static_cast<Light::PropagationSetting>(p->lightPropagationType));

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

		// Check Animator
		if (p->hasAnimator)
		{
			e->CreateAnimator();
			e->GetAnimator()->Init(e.get());
			
			if (!p->animFile.empty() && p->animFile.compare("NULL") != 0)
			{
				Animator* anim = e->GetAnimator();
				Animator* other = engine.GetAssetsManager().GetAnimator(p->animFile);
				if (other)
				{
					*anim = *other; // Sets *anim to be equal to the one in the asset manager (copy-assignment)
					anim->PlayDefaultIfNotNull();
				}
			}
			else
			{
				SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Prefab %s has Animator enabled, but no Animator file was specified.", p->pName.c_str());
			}
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
	SDL_Log("LPropagation:%d", lightPropagationType);
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
