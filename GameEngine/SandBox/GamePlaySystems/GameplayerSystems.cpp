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
			player.dx = 0;
			player.dy = 1;
			sprite.textureID = player.upTextureID;
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
			player.dx = 0;
			player.dy = -1;
			sprite.textureID = player.downTextureID;
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
			player.dx = -1;
			player.dy = 0;
			sprite.textureID = player.leftTextureID;
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
			player.dx = 1;
			player.dy = 0;
			sprite.textureID = player.rightTextureID;
			HasSpaceToMove(world, tilemap, x, y, 1, 0);
			});
	}
}

long long powm(long long base, long long exp)
{
	long long result = 1;
	base = base; // Ensure base is within mod
	while (exp > 0)
	{
		if (exp % 2 == 1) // If exp is odd
			result = (result * base);
		exp = exp >> 1; // Divide exp by 2
		base = (base * base); // Square the base
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
	if(world.Has<Box>(e))
	{
		if (HasSpaceToMove(world, tilemap, x + dx, y + dy, dx, dy) == 1)
		{
			Sprite s = world.Get<Sprite>(e);
			world.Add<Sprite>(tilemap.map[x + dx][y + dy], std::move(s));
			world.Delete<Sprite>(e);


			world.Add<Box>(tilemap.map[x + dx][y + dy], Box());
			world.Delete<Box>(e);
			return 1;
		}
		return 0;
	}
	if (world.Has<Player>(e))
	{
		if (HasSpaceToMove(world, tilemap, x + dx, y + dy, dx, dy) == 1)
		{
			Sprite s = world.Get<Sprite>(e);
			world.Add<Sprite>(tilemap.map[x + dx][y + dy], std::move(s));
			world.Delete<Sprite>(e);

			Player p = world.Get<Player>(e);
			world.Add<Player>(tilemap.map[x + dx][y + dy], std::move(p));
			world.Delete<Player>(e);
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
			Sprite s = world.Get<Sprite>(e);
			world.Add<Sprite>(tilemap.map[x + dx][y + dy], std::move(s));
			world.Delete<Sprite>(e);

			Number p = world.Get<Number>(e);
			world.Add<Number>(tilemap.map[x + dx][y + dy], std::move(p));
			world.Delete<Number>(e);
			return 1;
		}
		if (res == 2)
		{
			Number& other = world.Get<Number>(tilemap.map[x + dx][y + dy]);
			if (dy == 0)
			{
				other.data = (other.data + number.data);
			}
			else if (dy == 1)
			{
				other.data = powm(number.data, other.data); // Use modular exponentiation
			}
			else if (dy == -1)
			{
				other.data = powm(other.data, number.data); // Use modular exponentiation
			}
			world.Delete<Number>(e); // Remove the original number entity
			world.Delete<Sprite>(e); // Remove the sprite associated with the number
			return 1;
		}
		
		return 2;
	}

	return 1;
}
