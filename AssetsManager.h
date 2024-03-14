#ifndef NECROASSETS_MANAGER_H
#define NECROASSETS_MANAGER_H

#include <iostream>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Image.h"

const int FONT_DEFAULT_PTSIZE = 24;

class NECROAssetsManager
{
private:
	std::vector<Image> images;
	std::vector<TTF_Font*> fonts;

private:
	void				LoadImage(const char* file, int yOffset);
	SDL_Texture*		LoadSDLTexture(const char* file);
	void				LoadAllImages();

	void				LoadFont(const char* file, size_t ptsize);
	void				LoadAllFonts();

public:
	int					Init();			// Load everything

	Image*				GetImageAt(size_t indx);
	TTF_Font*			GetFontAt(size_t indx);
};

#endif
