#ifndef NECROGAME_H
#define NECROGAME_H

#include "World.h"
#include "Camera.h"

//-------------------------------------------------------
// Defines the Game related things that run in the engine
//-------------------------------------------------------
class NECROGame
{
private:
	World currentWorld;
	Camera mainCamera;

public:
	void		Init();
	void		Update();
	void		Shutdown();

	Camera*		GetMainCamera();
};

inline Camera* NECROGame::GetMainCamera()
{
	return &mainCamera;
}


#endif
