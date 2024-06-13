#include "InputField.h"
#include "NECROEngine.h"

void InputField::Init(SDL_Rect sRct, SDL_Rect dRct, const std::string& s, Image* im, Image* actIm, int tLimit)
{
	rect = sRct;
	dstRect = dRct;
	str = s;
	img = im;
	focusedImage = actIm;
	textLimit = tLimit;

	// Default font
	font = engine.GetAssetsManager().GetFont("defaultFont");
}

void InputField::Draw()
{
	NECRORenderer& r = engine.GetRenderer();

	// Draw correct background
	r.DrawImageDirectly(isFocused ? focusedImage->GetSrc() : img->GetSrc(), &rect, &dstRect);

	if (str.size() > 0)
	{
		// Draw text inside the field img
		r.DrawTextDirectly(font, str.c_str(), dstRect.x + xOffset, dstRect.y, color);
	}
}

void InputField::SetFocused(bool f)
{
	isFocused = f;

	if (isFocused)
	{
		// Reset the string
		str.clear();
		engine.GetInput().SetCurInputField(this);
	}
	else
		engine.GetInput().SetCurInputField(nullptr);
}
