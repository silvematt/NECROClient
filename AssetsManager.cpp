#include "AssetsManager.h"
#include "NECROEngine.h"

const char* IMGS_FOLDER		 = "Data/imgs/";
const char* FONTS_FOLDER	 = "Data/fonts/";
const char* PREFABS_FOLDER	 = "Data/prefabs/";
const char* ANIMATORS_FOLDER = "Data/prefabs/animators/";

//-------------------------------------------------
// Initializing the AssetsManager loads everything
//-------------------------------------------------
int NECROAssetsManager::Init()
{
	LoadAllImages();
	LoadAllFonts();
	LoadAllPrefabs();

	return 0;
}

//----------------------------------------------------------
// Load all the images 
// 
// TODO: This is for quick testing, actual loading
// will be done through a proper file or as we load prefabs
//----------------------------------------------------------
void NECROAssetsManager::LoadAllImages()
{
	// General
	LoadImage("null_img.png", 0, 0);
	LoadImage("tile.png", 0, 0);
	LoadImage("tile_highlighted.png", 0, 0);
	LoadImage("tree.png", -15, -5);
	LoadImage("tile_debug_coll.png", 0, 0);

	// Player tilesets
	LoadTilesetImage("player_war_idle.png", -30, 12, 128, 128, 1, 8);
	LoadTilesetImage("player_war_run.png", -30, 12, 128, 128, 12, 8);
	LoadTilesetImage("player_war_aim_stand.png", -30, 12, 128, 128, 1, 8);
	LoadTilesetImage("player_war_aim_walk.png", -30, 12, 128, 128, 11, 8);
	LoadTilesetImage("player_war_aim_strafe.png", -30, 12, 128, 128, 11, 8);
	LoadTilesetImage("campfire01.png", 0, 0, 46, 46, 4, 1);
}

//-------------------------------------------------
// Load all the fonts 
// 
// TODO: This is for quick testing, actual loading
// will be done through a proper file.
//-------------------------------------------------
void NECROAssetsManager::LoadAllFonts()
{
	LoadFont("montserrat.regular.ttf", FONT_DEFAULT_PTSIZE, "defaultFont");
}

// TODO: Prefab loading should be done only if necessary, when loading the map we'll look at the content and only load the needed prefabs, same thing for the images
void NECROAssetsManager::LoadAllPrefabs()
{
	LoadPrefab("tree01.nprfb");
	LoadPrefab("campfire01.nprfb");
}

void NECROAssetsManager::LoadImage(const std::string& filename, int xOffset, int yOffset, const std::string& shortname)
{
	std::string fullPath = IMGS_FOLDER;
	fullPath += filename;
	
	Image img(LoadSDLTexture(fullPath.c_str()), xOffset, yOffset);
	if(img.GetSrc() != NULL)
		images.insert({ shortname.empty() ? filename : shortname, img });
}

void NECROAssetsManager::LoadTilesetImage(const std::string& filename, int xOffset, int yOffset, int tWidth, int tHeight, int tNumX, int tNumY, const std::string& shortname)
{
	std::string fullPath = IMGS_FOLDER;
	fullPath += filename;

	Image img(LoadSDLTexture(fullPath.c_str()), xOffset, yOffset, tWidth, tHeight, tNumX, tNumY);
	if (img.GetSrc() != NULL)
		images.insert({ shortname.empty() ? filename : shortname, img });
}

//-------------------------------------------------
// Loads a single font into the fonts vector
//-------------------------------------------------
void NECROAssetsManager::LoadFont(const std::string& filename, int ptsize, const std::string& shortname)
{
	std::string fullPath = FONTS_FOLDER;
	fullPath += filename;

	TTF_Font* font = nullptr;

	font = TTF_OpenFont(fullPath.c_str(), ptsize);

	if (font != nullptr)
	{
		fonts.insert({ shortname.empty() ? filename : shortname, font });
	}
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "NECROAssetsManager: Failed to LoadFont(%s)\n", filename.c_str());
	}
}

//-------------------------------------------------
// Loads and returns an SDL_Texture
//-------------------------------------------------
SDL_Texture* NECROAssetsManager::LoadSDLTexture(const char* filename)
{
	SDL_Texture* texture;

	texture = IMG_LoadTexture(engine.GetRenderer().GetInnerRenderer(), filename);

	if (texture != NULL) // IMG_LoadTexture returns NULL on failure
	{
		return texture;
	}
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,"NECROAssetsManager: Failed to LoadImage(%s)\n", filename);
		return NULL;
	}
}

void NECROAssetsManager::LoadPrefab(const std::string& filename)
{
	std::string fullPath = PREFABS_FOLDER;
	fullPath += filename;

	Prefab p;
	p.LoadFromFile(fullPath);
	prefabs.insert({ p.GetName(), p });
}

//-------------------------------------------------
// Returns an SDL_Texture*
//-------------------------------------------------
Image* NECROAssetsManager::GetImage(const std::string& filename)
{
	auto it = images.find(filename);
	if (it != images.end())
	{
		return &it->second;
	}

	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "GetImage: %s not found! Returning null_img.", filename.c_str());
	return &images.at("null_img.png");
}


//-------------------------------------------------
// Returns an TTF_Font*
//-------------------------------------------------
TTF_Font* NECROAssetsManager::GetFont(const std::string& filename)
{
	auto it = fonts.find(filename);
	if (it != fonts.end())
	{
		return it->second;
	}

	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "GetFont: %s not found!", filename.c_str());
	return nullptr;
}

Prefab* NECROAssetsManager::GetPrefab(const std::string& prefabName)
{
	auto it = prefabs.find(prefabName);
	if (it != prefabs.end()) 
	{
		return &it->second;
	}

	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "GetPrefab: %s not found!", prefabName.c_str());
	return nullptr;
}
