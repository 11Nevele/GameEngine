#include "acpch.h"
#include "LevelManagementSystems.h"

void LevelManagementSystems::ResetLevel(World& world)
{
	world.View<Player>().ForEach([&world](Entity entity, Player& player)
		{
			world.DeleteEntity(entity);
		});
	world.View<Wall>().ForEach([&world](Entity entity, Wall& wall)
		{
			world.DeleteEntity(entity);
		});
	world.View<Box>().ForEach([&world](Entity entity, Box& box)
		{
			world.DeleteEntity(entity);
		});
	world.View<Number>().ForEach([&world](Entity entity, Number& number)
		{
			world.DeleteEntity(entity);
		});
	world.View<WinCondition>().ForEach([&world](Entity entity, WinCondition& winCondition)
		{
			world.DeleteEntity(entity);
		});
	world.View<TilemapElement>().ForEach([&world](Entity entity, TilemapElement& tilemapElement)
		{
			world.DeleteEntity(entity);
		});
	world.View<Tilemap>().ForEach([&world](Entity entity, Tilemap& tilemap)
		{
			world.DeleteEntity(entity);
		});
	// Reset other components as needed

}

void AddNumber(World& world, Tilemap& tilemap, uint32_t x, uint32_t y, long long value)
{
	Entity e = tilemap.map[x][y];
	if (!world.Has<Number>(e))
	{
		world.Add<Number>(e, Number(value));
	}
	world.Add<Sprite>(e, Sprite::Create("Box2", world.GetResourse<TextureManager>()));
}

void LevelManagementSystems::LoadLevel(World& world, Levels level)
{
	switch (level)
	{
		case MAIN_MENU:
			MainMenu(world);
			break;
		case TEST_LEVEL:
			TestLevel(world);
			break;
		case LEVEL_1:
			Level1(world);
			break;
	}
}

void LevelManagementSystems::TestLevel(World& world)
{
	world.GetResourse<SceneData>() = {5, 64, 64}; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	int map[mapHeight][mapWidth] = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,3,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	//flip the map vertically to match the tilemap coordinate system
	for (int y = 0; y < mapHeight / 2; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			std::swap(map[y][x], map[mapHeight - 1 - y][x]);
		}
	}
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));

	
	for (int x = 0; x < mapWidth; ++x)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			
			Entity tile = world.CreateEntity();
			world.Add<TilemapElement>(tile, TilemapElement(tilemap, x, y));
			if (map[y][x] == 1)
			{
				world.Add<Sprite>(tile, Sprite::Create("Wall1", world.GetResourse<TextureManager>()));
				world.Add<Wall>(tile, Wall());
			}
			else if (map[y][x] == 2)
			{
				world.Add<Sprite>(tile, Sprite::Create("PlayerDown", world.GetResourse<TextureManager>()));
				world.Add<Player>(tile, Player::Create(world));
			}
			else if (map[y][x] == 3)
			{
				world.Add<Sprite>(tile, Sprite::Create("Box1", world.GetResourse<TextureManager>()));
				world.Add<Box>(tile, Box());
			}

			Entity backgroundTile = world.CreateEntity();
			world.Add<TilemapElement>(backgroundTile, TilemapElement(background, x, y));
			world.Add<Sprite>(backgroundTile, Sprite::Create("Ground1", world.GetResourse<TextureManager>()));
		}
	}

	AddNumber(world, world.Get<Tilemap>(tilemap), 2, 3, 3); // Add a number at (2, 3) with value 1
	AddNumber(world, world.Get<Tilemap>(tilemap), 3, 3, 2); // Add a number at (3, 3) with value 2
}

void LevelManagementSystems::MainMenu(World& world)
{
}

void LevelManagementSystems::Level1(World& world)
{
}
