#include "AssetsManager.h"
#include "NECROEngine.h"

const char* IMGS_FOLDER		= "Data/imgs/";
const char* FONTS_FOLDER	= "Data/fonts/";

//-------------------------------------------------
// Initializing the AssetsManager loads everything
//-------------------------------------------------
int NECROAssetsManager::Init()
{
	LoadAllImages();
	LoadAllFonts();

	return 0;
}

//-------------------------------------------------
// Load all the images 
// 
// TODO: This is for quick testing, actual loading
// will be done through a proper file
//-------------------------------------------------
void NECROAssetsManager::LoadAllImages()
{
	LoadImage("tile.png", 0, 0);
	LoadImage("tile_highlighted.png", 0, 0);
	LoadImage("tree.png", -15, -5);
	LoadTilesetImage("player_war.png", -30, 12, 128, 128, 1, 8);
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

//-------------------------------------------------
// Returns an SDL_Texture*
//-------------------------------------------------
Image* NECROAssetsManager::GetImage(const std::string& filename)
{
	return &images.at(filename);
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
// Returns an TTF_Font*
//-------------------------------------------------
TTF_Font* NECROAssetsManager::GetFont(const std::string& filename)
{
	return fonts.at(filename);
}
