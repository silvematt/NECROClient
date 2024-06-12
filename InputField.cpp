#include "InputField.h"
#include "NECROEngine.h"

void InputField::Init(SDL_Rect sRct, SDL_Rect dRct, const std::string& s, Image* im, Image* actIm, int tLimit)
{
	rect = sRct;
	dstRect = dRct;
	str = s;
	img = im;
	activeImg = actIm;
	textLimit = tLimit;
}

void InputField::Draw()
{
	NECRORenderer& r = engine.GetRenderer();

	r.DrawImageDirectly(isFocused ? activeImg->GetSrc() : img->GetSrc(), &rect, &dstRect);
}
