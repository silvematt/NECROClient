#ifndef NECROGAME_H
#define NECROGAME_H

#include "World.h"
#include "Camera.h"
#include <string>

class Player;

enum IsoDirection
{
	WEST = 0,
	NORTH_WEST,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST
};

//-------------------------------------------------------
// Defines the Game related things that run in the engine
//-------------------------------------------------------
class NECROGame
{
private:
	World currentWorld;
	Camera mainCamera;
	Player* curPlayer;

public:
	void		Init();
	void		Update();
	void		Shutdown();

	void		SetCurPlayer(Player* p);

	Camera*		GetMainCamera();
	World*		GetCurrentWorld();
	Player*		GetCurPlayer();
};

inline void NECROGame::SetCurPlayer(Player* p)
{
	curPlayer = p;
}

inline Camera* NECROGame::GetMainCamera()
{
	return &mainCamera;
}

inline World* NECROGame::GetCurrentWorld()
{
	return &currentWorld;
}

inline Player* NECROGame::GetCurPlayer()
{
	return curPlayer;
}

#endif
