#include "Mapfile.h"
#include "NECROEngine.h"
#include "Utility.h"
#include "TilesetDef.h"

#include "SDL.h"

#include <fstream>

//----------------------------------------------------------------------------------------
// Loads a Map as the current world. TODO: this overrides the previous world,
// if in the we want to keep the exterior world after entering a dungeon this isn't ideal. 
//----------------------------------------------------------------------------------------
bool Mapfile::LoadMap(const std::string& filename)
{
	// Get reference to the world
	World* w = engine.GetGame().GetCurrentWorld();

	std::string fullPath = MAPFILES_DIRECTORY + filename;

	std::fstream stream(fullPath);

	SDL_Log("Loading Mapfile at: '%s'\n", filename.c_str());

	if (!stream.is_open())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load prefab at: %s\n", filename.c_str());
		return false;
	}

	std::string curLine;
	std::string curValStr;
	int curValInt;

	// World name
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	name = curValStr;

	// tilesetDefName
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	tilesetDefName = curValStr;

	// Load the tileset definition
	w->tileDef.LoadFromFile(tilesetDefName);

	// width
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	width = Utility::TryParseInt(curValStr);

	// height
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	height = Utility::TryParseInt(curValStr);

	// nLayers
	std::getline(stream, curLine);
	curValStr = curLine.substr(curLine.find("=") + 2); // key = value;
	curValStr = curValStr.substr(0, curValStr.find(";"));
	nLayers = Utility::TryParseInt(curValStr);

	// Load layers and add tiles as entities
	bool layersDone = false;
	int curLayer = 0;
	std::getline(stream, curLine); // layerX:
	while (!layersDone)
	{
		std::getline(stream, curLine); // {

		// Load the matrix
		for (int i = 0; i < height; i++)
		{
			std::getline(stream, curLine); // get row
			// StartPos to read matrix values
			int startPos = 0; 
			for (int j = 0; j < width; j++)
			{
				int endPos = curLine.find(',', startPos+1);  // read until the ','

				// Get the TileID
				int curVal = Utility::TryParseInt(curLine.substr(startPos, endPos - startPos));
				if (curVal != 0) // if not empty
				{
					// Get the resource img this value comes from
					int resIndex = w->tileDef.GetResourceIndexFromID(curVal); 

					// Spawn the Tile as an Entity
					TilesetDef::TileData* data = w->tileDef.GetTileData(curVal);

					std::unique_ptr<Entity> e(new Entity(Vector2(static_cast<float>(CELL_WIDTH * j), static_cast<float>(CELL_HEIGHT * i)), w->tileDef.GetResource(resIndex)));
					e->SetLayer(curLayer);
					e->SetTilesetOffset(w->tileDef.GetTile(curVal).first, w->tileDef.GetTile(curVal).second);

					// If per-tile data is defined, apply it
					if (data)
					{
						e->zPos = data->zOffset;

						if (data->colliderEnabled)
						{
							e->CreateCollider();
							e->GetCollider()->Init(1, e.get(), data->collOffsetX, data->collOffsetY, data->collWidth, data->collHeight);
						}

						if (data->occlusionEnabled)
						{
							e->SetFlag(Entity::Flags::FCanOccludePlayer);
							e->SetOcclusionModifierValues(data->occlOffX, data->occlOffY);
						}

						e->SetZCellModifier(data->zCellModifier);
					}

					w->AddEntity(std::move(e));
				}

				startPos = endPos+1; // adjust startPos for next reading
			}
		}

		std::getline(stream, curLine); // };

		std::getline(stream, curLine); // either a new layer, or end of file

		if (curLine.find("layer") != std::string::npos)
		{
			layersDone = false;
			curLayer++;
		}
		else
			layersDone = true;
	}

	return true;
}
