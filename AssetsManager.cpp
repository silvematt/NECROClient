#include "AssetsManager.h"
#include "NECROEngine.h"


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
// will be done through a proper file and by managing
// the images vector properly
//-------------------------------------------------
void NECROAssetsManager::LoadAllImages()
{
	LoadImage("Data/imgs/tile.png");
	LoadImage("Data/imgs/tile_highlighted.png");
}

//-------------------------------------------------
// Load all the images 
// 
// TODO: This is for quick testing, actual loading
// will be done through a proper file and by managing
// the fonts vector properly
//-------------------------------------------------
void NECROAssetsManager::LoadAllFonts()
{
	LoadFont("Data/fonts/montserrat.regular.ttf", FONT_DEFAULT_PTSIZE);
}

//-------------------------------------------------
// Loads a single image into the images vector
//-------------------------------------------------
void NECROAssetsManager::LoadImage(const char* filename)
{
	SDL_Texture* texture = nullptr;

	texture = IMG_LoadTexture(engine.GetRenderer().GetInnerRenderer(), filename);

	if (texture != nullptr)
	{
		images.push_back(texture);
	}
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,"NECROAssetsManager: Failed to LoadImage(%s)\n", filename);
	}
}

//-------------------------------------------------
// Returns an SDL_Texture*
//-------------------------------------------------
SDL_Texture* NECROAssetsManager::GetImageAt(size_t indx)
{
	if (images.size() > indx)
		return images[indx];
	else
		return nullptr;
}

//-------------------------------------------------
// Loads a single font into the fonts vector
//-------------------------------------------------
void NECROAssetsManager::LoadFont(const char* filename, size_t ptsize)
{
	TTF_Font* font = nullptr;

	font = TTF_OpenFont(filename, ptsize);

	if (font != nullptr)
	{
		fonts.push_back(font);
	}
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "NECROAssetsManager: Failed to LoadFont(%s)\n", filename);
	}
}

//-------------------------------------------------
// Returns an TTF_Font*
//-------------------------------------------------
TTF_Font* NECROAssetsManager::GetFontAt(size_t indx)
{
	if (fonts.size() > indx)
		return fonts[indx];
	else
		return nullptr;
}
