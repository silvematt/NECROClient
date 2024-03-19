#ifndef NECROPLAYER_H
#define NECROPLAYER_H

#include "Entity.h"

class Player : public Entity
{
private:
	float curMoveSpeed = 2.5f;

public:
	float			GetCurMoveSpeed();

	virtual void	Update() override;
};

inline float Player::GetCurMoveSpeed()
{
	return curMoveSpeed;
}

#endif
