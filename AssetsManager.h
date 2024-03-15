#ifndef NECROASSETS_MANAGER_H
#define NECROASSETS_MANAGER_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Image.h"

const int FONT_DEFAULT_PTSIZE = 24;

class NECROAssetsManager
{
private:
	std::unordered_map<std::string, Image> images;
	std::unordered_map<std::string, TTF_Font*> fonts;

private:
	void				LoadAllImages();
	void				LoadAllFonts();

	void				LoadImage(const std::string& file, int yOffset, const std::string& shortname = std::string());	// Shortname will be the key of the ump if NOT empty
	SDL_Texture*		LoadSDLTexture(const char* file);

	void				LoadFont(const std::string& file, int ptsize, const std::string& shortname = std::string());

public:
	int					Init();			// Load everything

	Image*				GetImage(const std::string& filename);
	TTF_Font*			GetFont(const std::string& filename);
};

#endif
