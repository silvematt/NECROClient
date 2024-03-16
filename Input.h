#ifndef NECROINPUT_H
#define NECROINPUT_H

#include "SDL.h"
#include "Vector2.h"

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

	int				mouseMotionX;
	int				mouseMotionY;

	Uint8*			keys;
	Uint8*			prevKeys;
	int				numKeys;

public:
	~NECROInput();

	int				GetMouseX() const;
	int				GetMouseY() const;
	int				GetMouseScroll() const;
	int				GetMouseHeld(SDL_Scancode button) const;
	int				GetMouseDown(SDL_Scancode button) const;
	Vector2			GetMouseMotionThisFrame();
	int				GetKeyHeld(SDL_Scancode key) const;

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
