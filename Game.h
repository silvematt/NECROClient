#ifndef NECROGAME_H
#define NECROGAME_H

#include "World.h"

//-------------------------------------------------------
// Defines the Game related things that run in the engine
//-------------------------------------------------------
class NECROGame
{
private:
	World currentWorld;

public:
	void		Init();
	void		Update();
	void		Shutdown();
};

#endif
