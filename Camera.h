#ifndef NECROCAMERA_H
#define NECROCAMERA_H

#include "Vector2.h"

const float CAMERA_DEFAULT_ZOOM = 1.0f;

class Camera
{
private:
	const float MIN_ZOOM = 0.5f;
	const float MAX_ZOOM = 2.0f;

	// ------------------------------------------------------------------------------------------
	// zoomSizeX and zoomSizeY are used to keep the camera centered while zooming in/out.
	// 
	// They represent the size of the viewport on the X and Y axes after applying the zoom level
	// and are calculated as SCREEN_WIDTH / zoomLevel, SCREEN_HEIGHT / zoomLevel 
	// 
	// For example, if the zoom level is 2.0, zoomSizeX and zoomSizeY will represent half of the 
	// original screen width and height, showing half of the world.
	// 
	// When the zoom level changes, oldZoomSizeX and oldZoomSizeY (defined in the Update()) 
	// hold the previous viewport size, which is used to adjust the camera position to 
	// keep it centered correctly after the zoom.
	// ------------------------------------------------------------------------------------------
	float zoomSizeX;
	float zoomSizeY;

	float zoomLevel = 1.0f;
	float zoomSpeed = 0.05f;

	float panSpeed = 50.0f;

	bool freeCamera = false;

private:
	void		FreeMove();
	void		LockPlayerMove();

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
