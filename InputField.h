#ifndef NECROINPUT_FIELD_H
#define NECROINPUT_FIELD_H

#include <string>
#include "SDL.h"

#include "Image.h"


//----------------------------------------------------------------------------------------------
// Selectable and writable Input Field displayed as UI element
//----------------------------------------------------------------------------------------------
class InputField
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };

	std::string str = "";

	Image* img = nullptr;
	Image* activeImg = nullptr;

	bool isFocused = false;
	int textLimit = 0;

public:
	InputField() = default;

	void		Init(SDL_Rect sRct, SDL_Rect dRct, const std::string& s, Image* im, Image* actIm, int tLimit = 0);
	void		Draw();
};

#endif
