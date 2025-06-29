#pragma once
#include "Core\ECSEvents.h"
#include "Core\World.hpp"
#include "EngineComponents\Sprite.h"
namespace ac
{
	bool OnSpriteAdded(const OnAdded<Sprite>& event);
	bool OnSpriteDelete(const OnDeleted<Sprite>& event);
	void RenderSprite(World& world);
	void RenderCircle(World& world);
	void RenderCollider(World& world);

}