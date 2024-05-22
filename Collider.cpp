#include "Collider.h"

#include "NECROEngine.h"

void Collider::Init(bool pEnabled, Entity* own, int x, int y, int w, int h)
{
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	enabled = pEnabled;
	owner = own;

	debugImg = engine.GetAssetsManager().GetImage("tile_debug_coll.png");
}

void Collider::SetOffset(float x, float y)
{
	collOffset.x = x;
	collOffset.y = y;
}

void Collider::Update()
{
	if (!enabled || !owner)
		return;

	float posX = owner->pos.x + collOffset.x;
	float posY = owner->pos.y + collOffset.y;

	// Center-align the collision rectangle on its entity's position:
	r.x = posX + (r.w / 2.0f);
	r.y = posY - (r.h / 2.0f);

	// TODO: IsoPos is only used for debugging, don't calculate it if it's not to be debugged
	// if (debugEnabled)
	// {
	isoPosX = isoPosY = 0;

	// Update ISO coordinates
	NMath::CartToIso(posX / CELL_WIDTH, posY / CELL_HEIGHT, isoPosX, isoPosY);

	isoPosX -= HALF_CELL_WIDTH;

	// Adjust isoX and isoY to the world offset
	isoPosX += engine.GetGame().GetMainCamera()->pos.x;
	isoPosY += engine.GetGame().GetMainCamera()->pos.y;

	if (!debugImg->IsTileset())
		isoPosY -= debugImg->GetHeight();
	else
		isoPosY -= debugImg->GetTilesetHeight();

	// Account for the Y offset of the image
	isoPosX += debugImg->GetXOffset();
	isoPosY += debugImg->GetYOffset();

	// }
}

void Collider::DebugDraw()
{
	auto previousTarget = engine.GetRenderer().GetCurrentERenderTargetVal();

	// Set debug target, update scale in base of the main camera zoom
	engine.GetRenderer().SetRenderTarget(NECRORenderer::ERenderTargets::DEBUG_TARGET);
	float zoomLevel = engine.GetGame().GetMainCamera()->GetZoom();
	engine.GetRenderer().SetScale(zoomLevel, zoomLevel); // TODO: this should not be here (probably in SetZoom with the main RenderTarget scale), we need to set the scale of the renderer one time and not for each debug draw

	SDL_Rect dstRect;
	dstRect = { (int)isoPosX, (int)isoPosY, r.w, r.h };
	engine.GetRenderer().DrawImageDirectly(debugImg->GetSrc(), NULL, &dstRect);

	// Restore previous target
	engine.GetRenderer().SetRenderTarget(previousTarget);
}

// TODO: Implement intersection check instead of using SDL's.
bool Collider::TestIntersection(const Collider* other)
{
	return SDL_HasIntersection(&r, &other->r);
}
