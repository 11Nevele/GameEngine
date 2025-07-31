#pragma once
#include "Achoium.h"
#include "Components.h"
using namespace ac;
class UpdateSprite
{
public:
	static void Update(World& world)
	{
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		world.View<Sprite, Transform>().ForEach([&textureManager, &world](Entity entity, Sprite& sprite, Transform& transform)
		{
			if(world.Has<Player>(entity) || world.Has<PlayerReplay>(entity))
			{
				sprite.textureID = textureManager.GetTextureID("PlayerDown");
			}
		}
		);
	}
};

