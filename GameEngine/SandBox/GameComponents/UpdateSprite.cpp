#include "acpch.h"
#include "UpdateSprite.h"

void UpdateSprite::Update(World& world)
{
	TextureManager& textureManager = world.GetResourse<TextureManager>();
	world.View<Sprite, Transform, Position>().ForEach([&textureManager, &world](Entity entity, Sprite& sprite, Transform& transform, Position& pos)
		{
			if(world.Has<Button>(entity))
			{
				if(world.Get<Button>(entity).isPressed)
				{
					//sprite.textureID = textureManager.GetTextureID("ButtonPressed");
				}
				else
				{
					//sprite.textureID = textureManager.GetTextureID("ButtonNormal");
				}
			}
			else if(world.Has<Door>(entity))
			{
				Door& door = world.Get<Door>(entity);
				if (door.isOpen)
				{

				}
				else
				{

				}
			}

			else if(world.Has<Coorpse>(entity))
			{
				Coorpse& coorpse = world.Get<Coorpse>(entity);
				if(coorpse.type == 0)
					sprite.textureID = textureManager.GetTextureID("Coorpse1");
				else if(coorpse.type == 1)
					sprite.textureID = textureManager.GetTextureID("Coorpse2");
				else if(coorpse.type == 2)
					sprite.textureID = textureManager.GetTextureID("Coorpse3");
			}


		}
	);
}
