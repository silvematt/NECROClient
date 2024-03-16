#ifndef NECROCAMERA_H
#define NECROCAMERA_H

#include "Vector2.h"

const float CAMERA_DEFAULT_ZOOM = 1.0f;

class Camera
{
private:
	float zoomLevel = 1.0f;
	float zoomSpeed = 0.1f;

public:
	Vector2 pos;

	void		Update();
	void		SetZoom(float newZoom);
	void		ResetZoom();
	float		GetZoom();

	Vector2		ScreenToWorld(const Vector2& point);	// ScreenToWorld while accounting for zoom
};

inline float Camera::GetZoom()
{
	return zoomLevel;
}

#endif
