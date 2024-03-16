#include "Camera.h"

#include "NECROEngine.h"

void Camera::SetZoom(float newZoom)
{
	zoomLevel = newZoom;
	engine.GetRenderer().SetScale(zoomLevel, zoomLevel);
}

void Camera::ResetZoom()
{
	zoomLevel = DEFAULT_ZOOM;
	engine.GetRenderer().SetScale(zoomLevel, zoomLevel);
}

Vector2 Camera::ScreenToWorld(const Vector2& point)
{
	Vector2 worldPoint;

	// Adjust for offset and zoom level
	worldPoint.x = (point.x - pos.x * zoomLevel) / zoomLevel;
	worldPoint.y = (point.y - pos.y * zoomLevel) / zoomLevel;

	// Convert to world coordinates
	NMath::IsoToCart(worldPoint.x, worldPoint.y, worldPoint.x, worldPoint.y);

	return worldPoint;
}

void Camera::Update()
{
	if (engine.GetInput().GetMouseScroll() > 0)
		SetZoom(zoomLevel + zoomSpeed);
	else if (engine.GetInput().GetMouseScroll() < 0)
		SetZoom(zoomLevel - zoomSpeed);

	if (engine.GetInput().KeyHeld(SDL_SCANCODE_A))
		pos.x += 5;
	if (engine.GetInput().KeyHeld(SDL_SCANCODE_D))
		pos.x -= 5;
	if (engine.GetInput().KeyHeld(SDL_SCANCODE_W))
		pos.y += 5;
	if (engine.GetInput().KeyHeld(SDL_SCANCODE_S))
		pos.y -= 5;
}
