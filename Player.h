#ifndef NECROPLAYER_H
#define NECROPLAYER_H

#include "Entity.h"
#include "Game.h"
#include "Animator.h"
#include "Collider.h"

const float PLAYER_MOVE_SPEED_FREE = 180.0f;
const float PLAYER_MOVE_SPEED_AIM = 100.0f;

// Dimensions of each player frame
const int PLAYER_WIDTH = 128;
const int PLAYER_HEIGHT = 128;

const int HALF_PLAYER_WIDTH = 64;
const int HALF_PLAYER_HEIGHT = 64;

//-------------------------------------------------
// Player class, derived by Entity
//-------------------------------------------------
class Player : public Entity
{
private:
	float curMoveSpeed = 2.5f;
	IsoDirection isoDirection = IsoDirection::SOUTH;		// The isometric direction the player is facing

	float deltaX = 0.0f, deltaY = 0.0f;

	bool wasAiming = false;
	bool isAiming = false;									// Is the player in aim mode?

	bool wasMoving = false;
	bool isMoving = false;

	// Relative mouse pos used when aiming
	float relativeMouseX, relativeMouseY;

	// List of close (8-neighbours close) entities, filled every frame
	std::vector<Entity*> closeEntities;

private:
	void			CalculateIsoDirection(float deltaX, float deltaY);
	void			CalculateIsoDirectionWhileAiming();
	void			HandleMovements();
	void			HandleAnim();

	void			UpdateCloseEntities();

public:
	~Player();

	static uint32_t	ENT_ID;
	bool			controlsEnabled = true; // TEST: 

public:
	void			Init();
	virtual void	Update() override;

	float			GetCurMoveSpeed() const;
	int				GetLayerFromZPos() const;
};

inline float Player::GetCurMoveSpeed() const
{
	return curMoveSpeed;
}

inline int Player::GetLayerFromZPos() const
{
	return std::floor(zPos / LAYER_Z_COEFFICIENT);
}

#endif
