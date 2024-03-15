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

public:
	int				GetMouseX() const;
	int				GetMouseY() const;
	int				GetMouseDown(SDL_Scancode button) const;

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


#endif
