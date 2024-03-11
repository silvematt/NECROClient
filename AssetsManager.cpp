#include "AssetsManager.h"
#include "NECROEngine.h"


//-------------------------------------------------
// Initializing the AssetsManager loads everything
//-------------------------------------------------
int NECROAssetsManager::Init()
{
	LoadAllImages();

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
	LoadImage("tile.png");
}

//-------------------------------------------------
// Loads a single image into the images vector
//-------------------------------------------------
void NECROAssetsManager::LoadImage(const char* filename)
{
	SDL_Texture* texture = NULL;

	texture = IMG_LoadTexture(engine.GetRenderer().GetInnerRenderer(), filename);

	if (texture != NULL)
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
