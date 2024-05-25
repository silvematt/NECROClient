#ifndef NECROASSETS_MANAGER_H
#define NECROASSETS_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Image.h"
#include "Prefab.h"

const int FONT_DEFAULT_PTSIZE = 24;

class NECROAssetsManager
{
private:
	std::unordered_map<std::string, Image> images;
	std::unordered_map<std::string, TTF_Font*> fonts;
	std::unordered_map<std::string, Prefab> prefabs;

private:
	void				LoadAllImages();
	void				LoadAllFonts();
	void				LoadAllPrefabs();

	SDL_Texture*		LoadSDLTexture(const char* file);

	void				LoadImage(const std::string& file, int xOffset, int yOffset, const std::string& shortname = std::string());	// Shortname will be the key of the ump if NOT empty
	void				LoadTilesetImage(const std::string& filename, int xOffset, int yOffset, int tWidth, int tHeight, int tNumX, int tNumY, const std::string& shortname = std::string());
	void				LoadFont(const std::string& file, int ptsize, const std::string& shortname = std::string());
	void				LoadPrefab(const std::string& file);

public:
	int					Init();			// Load everything

	Image*				GetImage(const std::string& filename);
	TTF_Font*			GetFont(const std::string& filename);
	Prefab*				GetPrefab(const std::string& prefabName);
};

extern const char* IMGS_FOLDER;
extern const char* FONTS_FOLDER;
extern const char* PREFABS_FOLDER;
extern const char* ANIMATORS_FOLDER;

#endif
