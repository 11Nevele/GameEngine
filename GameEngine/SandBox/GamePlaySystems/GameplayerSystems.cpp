#include "acpch.h"
#include "GameplayerSystems.h"


// 修复：为静态成员变量 stepHistory 提供类型和作用域
std::vector<GameplayerSystems::StepInfo> GameplayerSystems::stepHistory;
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
			if(HasSpaceToMove(world, tilemap, x, y, 0, 1))
				AddStep(world, tilemap);
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
			if(HasSpaceToMove(world, tilemap, x, y, 0, -1))
				AddStep(world, tilemap);;
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
			if(HasSpaceToMove(world, tilemap, x, y, -1, 0))
				AddStep(world, tilemap);
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
			if(HasSpaceToMove(world, tilemap, x, y, 1, 0))
				AddStep(world, tilemap);
		});
	}
}

void GameplayerSystems::AddStep(World& world, Tilemap& tilemap)
{
	StepInfo stepInfo;
	world.View<Player, TilemapElement>().ForEach([&stepInfo](Entity entity, Player& player, TilemapElement& tilemapElement)
	{
		stepInfo.playerPos.emplace_back(tilemapElement.x, tilemapElement.y);
	});
	world.View<Box, TilemapElement>().ForEach([&stepInfo](Entity entity, Box& box, TilemapElement& tilemapElement)
	{
		stepInfo.boxPos.emplace_back(tilemapElement.x, tilemapElement.y);
	});
	world.View<Number, TilemapElement>().ForEach([&stepInfo](Entity entity, Number& number, TilemapElement& tilemapElement)
	{
		stepInfo.numberPos.emplace_back(tilemapElement.x, tilemapElement.y);
		stepInfo.numbers.push_back(number); // Store the number for undo
	});
	stepHistory.push_back(stepInfo);
}

void GameplayerSystems::UndoLastStep(World& world)
{
	InputManager& input = world.GetResourse<InputManager>();
	if (!input.IsKeyDown(AC_KEY_Z))
		return;
	if (stepHistory.size() <= 1)
		return;
	stepHistory.pop_back(); // Remove the last step
	StepInfo& lastStep = stepHistory.back();
	Tilemap& tilemap = world.Get<Tilemap>(std::get<1>(world.View<Player,TilemapElement>().GetPacked()[0].components).tilemap);
	world.View<Player, TilemapElement>().ForEach([&world, &lastStep](Entity entity, Player& player, TilemapElement& tilemapElement)
	{
			world.Delete<Player>(entity); // Remove the current player sprite
			world.Delete<Sprite>(entity); // Remove the current player sprite
		});
	world.View<Box, TilemapElement>().ForEach([&world](Entity entity, Box& box, TilemapElement& tilemapElement)
		{
			world.Delete<Box>(entity); // Remove the current box sprite
			world.Delete<Sprite>(entity); // Remove the current box sprite
		});
	world.View<Number, TilemapElement>().ForEach([&world](Entity entity, Number& number, TilemapElement& tilemapElement)
		{
			world.Delete<Number>(entity); // Remove the current number sprite
			world.Delete<Sprite>(entity); // Remove the current number sprite
		});
	for (const auto& pos : lastStep.playerPos)
		{
		Entity tile = tilemap.map[pos.first][pos.second];
		world.Add<Player>(tile, Player::Create(world));
		world.Add<Sprite>(tile, Sprite::Create("PlayerDown", world.GetResourse<TextureManager>())); // Default sprite
	}
	for (const auto& pos : lastStep.boxPos)
		{
		Entity tile = tilemap.map[pos.first][pos.second];
		world.Add<Box>(tile, Box());
		world.Add<Sprite>(tile, Sprite::Create("Box1", world.GetResourse<TextureManager>())); // Default box sprite
	}
	for (int i = 0; i < lastStep.numberPos.size(); ++i)
		{
		auto& pos = lastStep.numberPos[i];
		Entity tile = tilemap.map[pos.first][pos.second];
		Number num = lastStep.numbers[i];
		world.Add<Number>(tile, std::move(num)); // Reset number to 0
		world.Add<Sprite>(tile, Sprite::Create("Box2", world.GetResourse<TextureManager>())); // Default number sprite
	}

}

long long powm(long long base, long long exp)
{
	const long long mod = 1000000007; // Use a large prime number as modulus
	long long result = 1;
	base = base % mod; // Ensure base is within mod
	base = base; // Ensure base is within mod
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
				other.data = (other.data + number.data) % 1000000007;
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
