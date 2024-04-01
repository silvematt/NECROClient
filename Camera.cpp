#include "Camera.h"

#include "NECROEngine.h"

// ----------------------------------------------------------------------------------------------------
// Sets the camera zoom level
// ----------------------------------------------------------------------------------------------------
void Camera::SetZoom(float newZoom)
{
	zoomLevel = newZoom;
	zoomSizeX = SCREEN_WIDTH / zoomLevel;
	zoomSizeY = SCREEN_HEIGHT / zoomLevel;
	engine.GetRenderer().SetScale(zoomLevel, zoomLevel);
}

// ----------------------------------------------------------------------------------------------------
// Resets the camera zoom level to the default vlaue
// ----------------------------------------------------------------------------------------------------
void Camera::ResetZoom()
{
	zoomLevel = CAMERA_DEFAULT_ZOOM;
	zoomSizeX = SCREEN_WIDTH / zoomLevel;
	zoomSizeY = SCREEN_HEIGHT / zoomLevel;
	engine.GetRenderer().SetScale(zoomLevel, zoomLevel);
}

// ----------------------------------------------------------------------------------------------------
// From a point on the screen returns the point in the world
// ----------------------------------------------------------------------------------------------------
Vector2 Camera::ScreenToWorld(const Vector2& point)
{
	Vector2 worldPoint;

	// Adjust for offset and zoom level
	worldPoint.x = (point.x - pos.x * zoomLevel) / zoomLevel;
	worldPoint.y = (point.y - pos.y * zoomLevel) / zoomLevel;
	worldPoint.y += CELL_HEIGHT; // account for bottom-left origin

	// Convert to world coordinates
	NMath::IsoToCart(worldPoint.x, worldPoint.y, worldPoint.x, worldPoint.y);

	return worldPoint;
}

// ----------------------------------------------------------------------------------------------------
// Update camera
// ----------------------------------------------------------------------------------------------------
void Camera::Update()
{
	float oldX = pos.x;
	float oldY = pos.y;

	// Hold the zoom size before zoom change
	float oldZoomSizeX = zoomSizeX;
	float oldZoomSizeY = zoomSizeY;

	if (engine.GetInput().GetMouseScroll() > 0)			// Zoom in		
		SetZoom(zoomLevel + zoomSpeed);
	else if (engine.GetInput().GetMouseScroll() < 0)	// Zoom out
		SetZoom(zoomLevel - zoomSpeed);

	// Camera movement
	float deltaX = 0.0f;
	float deltaY = 0.0f;

	if (engine.GetInput().GetMouseHeld(static_cast<SDL_Scancode>(SDL_BUTTON_MIDDLE)))
	{
		// TODO: fix mouseMotion at different framerates
		Vector2 mouseMotion = engine.GetInput().GetMouseMotionThisFrame();
		deltaX += mouseMotion.x * panSpeed * engine.GetDeltaTime();
		deltaY += mouseMotion.y * panSpeed * engine.GetDeltaTime();
	}

	// Update position, subtracting ((oldZoomSizeX / 2) - (zoomSizeX / 2)) allows us to keep the camera centered after zooming
	// It adjusts the camera position by half of the difference in each dimension to keep the view centered.
	pos.x = oldX - ((oldZoomSizeX / 2) - (zoomSizeX / 2)) + deltaX;
	pos.y = oldY - ((oldZoomSizeY / 2) - (zoomSizeY / 2)) + deltaY;
}
