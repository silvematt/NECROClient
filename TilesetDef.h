#ifndef NECROTILEDEF_H
#define NECROTILEDEF_H

#include "Image.h"

// TilesetDefs folder and extension
#define TILESET_DEFS_FOLDER "Data/maps/tiledefs/"
#define TILESET_DEFS_EXTENSION ".ntdef"

#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------
// A TilesetDef contains all the images of a tileset indexed by an int, used in the Mapfile.
// It can have multiple tileset images loaded and indexed sequentially.
//----------------------------------------------------------------------------------------------
class TilesetDef
{
public:
	bool loaded = false;

	std::string name;
	int nTilesets; // the number of tileset images in this definition


	std::vector<Image*> resources;						// The imgs ptrs to the actual image loaded in the assets manager
	std::vector<std::pair<int, int>> resourceEndMap;	// Maps the Resource ID and the last tile ID of that resource (the actual .png image), so we can know translate by tileID what resource contains that tile
	std::vector<std::pair<int, int>> tiles;				// All tiles of this TilesetDef, #row, #col of the corresponding tileset image (index of vector) - es tiles(145).first | tiles(145).second returns the X,Y offsets to the tile with ID 145 
	// TODO, per-tile data definition, like collision

public:
	bool		LoadFromFile(const std::string& filename);

	int			GetResourceIndexFromID(int ID);
};

//----------------------------------------------------------------------------------------------
// Given the ID of a tile, returns the ID of the resource the tile comes from
//----------------------------------------------------------------------------------------------
inline int TilesetDef::GetResourceIndexFromID(int ID)
{
	if (!loaded)
		return -1;

	for (int i = 0; i < nTilesets; i++)
	{
		if (ID <= resourceEndMap.at(i).second) // if the ID is less or equal the last element of the current iterating resource
		{
			return i;
		}
	}

	return 0;
}

#endif
