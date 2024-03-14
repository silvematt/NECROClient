#ifndef NECROIMAGE_H
#define NECROIMAGE_H

#include "SDL.h"

//-------------------------------------------------------
// Represents an Image
// 
// TODO support sprite atlas
//-------------------------------------------------------
class Image
{
private:
	SDL_Texture* imgTexture;
	int width;
	int height;

	SDL_Rect rect;

	int offsetY;					// Offset Y is used to draw images that are, for example 64x64 on map of 64x32
									// If the 'tree.png' is 64x64, it should be drawn with a y offset of -32 (to draw the bottom of the tree correctly)

public:
	Image(SDL_Texture* tex, int yOff);

	SDL_Texture*				GetSrc() const;
	SDL_Rect&					GetRect();
	int							GetWidth() const;
	int							GetHeight() const;
	int							GetYOffset() const;
};




//-------------------------------------------------------
// Constructor
//-------------------------------------------------------
inline Image::Image(SDL_Texture* tex, int yOff) :
	imgTexture(tex),
	offsetY(yOff)
{
	SDL_QueryTexture(imgTexture, NULL, NULL, &width, &height);

	rect = { 0, 0, width, height };
}

inline SDL_Texture* Image::GetSrc() const
{
	return imgTexture;
}

inline SDL_Rect& Image::GetRect()
{
	return rect;
}

inline int Image::GetWidth() const
{
	return width;
}

inline int Image::GetHeight() const
{
	return height;
}

inline int Image::GetYOffset() const
{
	return offsetY;
}

#endif
