#ifndef NECROINPUT_H
#define NECROINPUT_H

#include "SDL.h"

class NECROInput
{
private:
	int				mouseX;
	int				mouseY;
	int				oldMouseX;
	int				oldMouseY;
	int				mouseButtons[3];
	int				prevMouseButtons[3];
	int				mouseScroll;

	Uint8*			keys;
	Uint8*			prevKeys;
	int				numKeys;

public:
	~NECROInput();

	int				GetMouseX() const;
	int				GetMouseY() const;
	int				GetMouseScroll() const;
	int				GetMouseDown(SDL_Scancode button) const;

	int				KeyHeld(SDL_Scancode key) const;

	int				Init();
	void			Handle();

};

inline int NECROInput::GetMouseX() const
{
	return mouseX;
}

inline int NECROInput::GetMouseY() const
{
	return mouseY;
}

inline int NECROInput::GetMouseScroll() const
{
	return mouseScroll;
}

#endif
