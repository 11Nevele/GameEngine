#include "acpch.h"
#include "InteractionSystems.h"
#include "Achoium.h"
#include "Components.h"
void InteractionSystems::CheckSpike(World& world)
{
	world.View<Position, Player>().ForEach([&world](Entity entity, Position& pos, Player& player)
		{
			if (world.Has<Ghost>(entity))
			{
				// Player is out of bounds, handle accordingly
				return;
			}
			// Check if the player is on a spike tile
			auto& map = world.GetResourse<MapInfo>().map;
			Tilemap& tilemap = world.Get<Tilemap>(world.GetResourse<MapInfo>().tilemap);
			if (world.Has<Spike>(tilemap.map[pos.x][pos.y]))
			{
				bool hasCoorpse = false;
				for(const auto& entityVec : map[pos.x][pos.y])
				{
					if (world.Has<Coorpse>(entityVec))
					{
						hasCoorpse = true;
						break;
					}
				}
				if (!hasCoorpse)
				{
					world.Add<Ghost>(entity, Ghost());
					world.Get<Sprite>(entity).textureID = world.GetResourse<TextureManager>().GetTextureID("GhostDown");
					mCreate<Coorpse>(world, pos.x, pos.y, "Coorpse1");
				}
			}

		});
	world.View<Position, PlayerReplay>().ForEach([&world](Entity entity, Position& pos, PlayerReplay& player)
		{
			if (world.Has<Ghost>(entity))
			{
				// Player is out of bounds, handle accordingly
				return;
			}
			// Check if the player is on a spike tile
			auto& map = world.GetResourse<MapInfo>().map;
			Tilemap& tilemap = world.Get<Tilemap>(world.GetResourse<MapInfo>().tilemap);
			if (world.Has<Spike>(tilemap.map[pos.x][pos.y]))
			{
				bool hasCoorpse = false;
				for (const auto& entityVec : map[pos.x][pos.y])
				{
					if (world.Has<Coorpse>(entityVec))
					{
						hasCoorpse = true;
						break;
					}
				}
				if (!hasCoorpse)
				{
					world.Add<Ghost>(entity, Ghost());
					world.Get<Sprite>(entity).textureID = world.GetResourse<TextureManager>().GetTextureID("GhostDown");
					mCreate<Coorpse>(world, pos.x, pos.y, "Coorpse1");
				}
			}

		});
}

void InteractionSystems::CheckButton(World& world)
{
}

void InteractionSystems::CheckDoor(World& world)
{
}

void InteractionSystems::CountDownSystem(World& world)
{
}

