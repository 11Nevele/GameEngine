#include "acpch.h"
#include "UpdateSprite.h"

void UpdateSprite::Update(World& world)
{
	TextureManager& textureManager = world.GetResourse<TextureManager>();
	world.View<Sprite, Transform, Position>().ForEach([&textureManager, &world](Entity entity, Sprite& sprite, Transform& transform, Position& pos)
		{
			if(world.Has<Button>(entity))
			{
				transform.position.z = -0.15;
				char c = world.Get<Button>(entity).color;
				if(world.Get<Button>(entity).isPressed)
				{
					sprite.textureID = textureManager.GetTextureID("ButtonPressed");
				}
				else
				{
					sprite.textureID = textureManager.GetTextureID("ButtonUnpressed" + string(1,c));
				}
			}
			else if(world.Has<Door>(entity))
			{
				Door& door = world.Get<Door>(entity);
				transform.position.z = -0.15;
				char c = door.color;
				if (door.isOpen)
				{
					sprite.textureID = textureManager.GetTextureID("DoorOpenned" + string(1, c));
				}
				else
				{
					sprite.textureID = textureManager.GetTextureID("DoorClosed" + string(1, c));
				}
			}

			else if(world.Has<Coorpse>(entity))
			{
				transform.position.z = -0.17;
				Coorpse& coorpse = world.Get<Coorpse>(entity);
				if(coorpse.type == 0)
					sprite.textureID = textureManager.GetTextureID("Coorpse1");
				else if(coorpse.type == 1)
					sprite.textureID = textureManager.GetTextureID("Coorpse2");
				else if(coorpse.type == 2)
					sprite.textureID = textureManager.GetTextureID("Coorpse3");
			}
			else if (world.Has<HealthKit>(entity))
			{
				transform.position.z = -0.16;
				sprite.textureID = textureManager.GetTextureID("HealthKit");
			}
			else if (world.Has<FinishPoint>(entity))
			{
				transform.position.z = -0.16;
				sprite.textureID = textureManager.GetTextureID("FinishPoint");
			}
			else if (world.Has<PlayerReplay>(entity))
			{
				transform.position.z = -0.2;
			}
			else if (world.Has<Player>(entity))
			{
				transform.position.z = -0.2;
			}
			
		}
	);
}
