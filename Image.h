#ifndef NECROIMAGE_H
#define NECROIMAGE_H

#include "SDL.h"

//-------------------------------------------------------
// Represents an Image
//-------------------------------------------------------
class Image
{
public:
	// Data for tilesets
	struct Tileset
	{
		int tileWidth;
		int tileHeight;

		int tileXNum;
		int tileYNum;
	};

private:
	SDL_Texture* imgTexture;
	int width;
	int height;

	SDL_Rect rect;

	int offsetX;
	int offsetY;					// Offset Y is used to draw images that are, for example 64x64 on map of 64x32
									// If the 'tree.png' is 64x64, it should be drawn with a y offset of -32 (to draw the bottom of the tree correctly)

	bool isTileset;
	Tileset tileset;
public:
	Image(SDL_Texture* tex, int xOff, int yOff);
	Image(SDL_Texture* tex, int xOff, int yOff, int tWidth, int tHeight, int tNumX, int tNumY);

	SDL_Texture*				GetSrc() const;
	SDL_Rect&					GetRect();
	int							GetWidth() const;
	int							GetHeight() const;
	int							GetXOffset() const;
	int							GetYOffset() const;

	bool						IsTileset() const;
	Tileset						GetTileset() const;

	int							GetTilesetWidth() const; // shortcut
	int							GetTilesetHeight() const; // shortcut
};




//-------------------------------------------------------
// Constructor
//-------------------------------------------------------
inline Image::Image(SDL_Texture* tex, int xOff, int yOff) :
	imgTexture(tex),
	offsetX(xOff),
	offsetY(yOff)
{
	SDL_QueryTexture(imgTexture, NULL, NULL, &width, &height);
	SDL_SetTextureBlendMode(imgTexture, SDL_BLENDMODE_BLEND);

	rect = { 0, 0, width, height };

	isTileset = false;

	tileset.tileWidth = 0;
	tileset.tileHeight = 0;
	tileset.tileXNum = 0;
	tileset.tileYNum = 0;
}

//-------------------------------------------------------
// Constructor for Tileset
//-------------------------------------------------------
inline Image::Image(SDL_Texture* tex, int xOff, int yOff, int tWidth, int tHeight, int tNumX, int tNumY) :
	imgTexture(tex),
	offsetX(xOff),
	offsetY(yOff)
{
	SDL_QueryTexture(imgTexture, NULL, NULL, &width, &height);
	SDL_SetTextureBlendMode(imgTexture, SDL_BLENDMODE_BLEND);

	rect = { 0, 0, width, height };

	isTileset = true;

	tileset.tileWidth = tWidth;
	tileset.tileHeight = tHeight;
	tileset.tileXNum = tNumX;
	tileset.tileYNum = tNumY;
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

inline int Image::GetXOffset() const
{
	return offsetX;
}

inline bool Image::IsTileset() const
{
	return isTileset;
}

inline Image::Tileset Image::GetTileset() const
{
	return tileset;
}

inline int Image::GetTilesetHeight() const
{
	return tileset.tileHeight;
}

inline int Image::GetTilesetWidth() const
{
	return tileset.tileWidth;
}

#endif
