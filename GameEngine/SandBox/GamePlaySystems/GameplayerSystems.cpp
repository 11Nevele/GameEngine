#include "acpch.h"
#include "GameplayerSystems.h"

void GameplayerSystems::PlayerMovement(World& world)
{
	InputManager& input = world.GetResourse<InputManager>();
	if (input.IsKeyDown(AC_KEY_W))
	{
		world.View<Player, TilemapElement, Sprite>().ForEach([&world, &input](Entity entity, Player& player, TilemapElement& tilemapElement, Sprite& sprite)
		{
			Tilemap& tilemap = world.Get<Tilemap>(tilemapElement.tilemap);
			uint32_t x = tilemapElement.x;
			uint32_t y = tilemapElement.y;
			sprite.textureID = player.upTextureID; // Default sprite for moving down
			HasSpaceToMove(world, tilemap, x, y, 0, 1);
		});
	}
	else if (input.IsKeyDown(AC_KEY_S))
	{
		world.View<Player, TilemapElement, Sprite>().ForEach([&world, &input](Entity entity, Player& player, TilemapElement& tilemapElement, Sprite& sprite)
		{
			Tilemap& tilemap = world.Get<Tilemap>(tilemapElement.tilemap);
			uint32_t x = tilemapElement.x;
			uint32_t y = tilemapElement.y;
			sprite.textureID = player.downTextureID; // Default sprite for moving down
			HasSpaceToMove(world, tilemap, x, y, 0, -1);
			});
	}
	else if (input.IsKeyDown(AC_KEY_A))
	{
		world.View<Player, TilemapElement, Sprite>().ForEach([&world, &input](Entity entity, Player& player, TilemapElement& tilemapElement, Sprite& sprite)
		{
			Tilemap& tilemap = world.Get<Tilemap>(tilemapElement.tilemap);
			uint32_t x = tilemapElement.x;
			uint32_t y = tilemapElement.y;
			sprite.textureID = player.leftTextureID; // Default sprite for moving left
			HasSpaceToMove(world, tilemap, x, y, -1, 0);
			});
	}
	else if (input.IsKeyDown(AC_KEY_D))
	{
		world.View<Player, TilemapElement, Sprite>().ForEach([&world, &input](Entity entity, Player& player, TilemapElement& tilemapElement, Sprite& sprite)
		{
			Tilemap& tilemap = world.Get<Tilemap>(tilemapElement.tilemap);
			uint32_t x = tilemapElement.x;
			uint32_t y = tilemapElement.y;
			sprite.textureID = player.rightTextureID; // Default sprite for moving right
			HasSpaceToMove(world, tilemap, x, y, 1, 0);
			});
	}
}

long long powm(long long base, long long exp, long long mod)
{
	long long result = 1;
	base = base % mod; // Ensure base is within mod
	while (exp > 0)
	{
		if (exp % 2 == 1) // If exp is odd
			result = (result * base) % mod;
		exp = exp >> 1; // Divide exp by 2
		base = (base * base) % mod; // Square the base
	}
	return result;
}

int GameplayerSystems::HasSpaceToMove(World& world, Tilemap& tilemap, uint32_t x, uint32_t y, int dx, int dy)
{
	if(x < 0 || x >= tilemap.map.size() || 
	   y < 0 || y >= tilemap.map[0].size())
	{
		return 0; // Out of bounds
	}
	Entity e = tilemap.map[x][y];
	if(world.Has<Wall>(e))
		{
		return 0; // There is a wall in the way
	}
	if(world.Has<Box>(e) || world.Has<Player>(e))
	{
		if (HasSpaceToMove(world, tilemap, x + dx, y + dy, dx, dy) == 1)
		{
			swap(world.Get<TilemapElement>(e), world.Get<TilemapElement>(tilemap.map[x + dx][y + dy]));
			swap(tilemap.map[x][y], tilemap.map[x + dx][y + dy]);
			return 1;
		}
		return 0;
	}
	if(world.Has<Number>(e))
	{
		Number& number = world.Get<Number>(e);
		int res = HasSpaceToMove(world, tilemap, x + dx, y + dy, dx, dy);
		if (res == 1)
		{
			swap(world.Get<TilemapElement>(e), world.Get<TilemapElement>(tilemap.map[x + dx][y + dy]));
			swap(tilemap.map[x][y], tilemap.map[x + dx][y + dy]);
			return 1;
		}
		if (res == 2)
		{
			Number& other = world.Get<Number>(tilemap.map[x + dx][y + dy]);
			if (dy == 0)
			{
				other.data = (other.data + number.data) % world.GetResourse<SceneData>().mod;
			}
			else if (dy == 1)
			{
				other.data = powm(number.data, other.data, world.GetResourse<SceneData>().mod); // Use modular exponentiation
			}
			else if (dy == -1)
			{
				other.data = powm(other.data, number.data, world.GetResourse<SceneData>().mod); // Use modular exponentiation
			}
			world.Delete<Number>(e); // Remove the original number entity
			world.Delete<Sprite>(e); // Remove the sprite associated with the number
			return 1;
		}
		
		return 2;
	}

	return 1;
}
