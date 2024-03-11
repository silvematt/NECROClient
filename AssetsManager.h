#ifndef NECROASSETS_MANAGER_H
#define NECROASSETS_MANAGER_H

#include <iostream>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"

class NECROAssetsManager
{
private:
	std::vector<SDL_Texture*> images; // TODO: do it in a proper way

private:
	void		LoadImage(const char* file);
	void		LoadAllImages();

public:
	int					Init();			// Load everything

	SDL_Texture*		GetImageAt(size_t indx);
};

#endif
