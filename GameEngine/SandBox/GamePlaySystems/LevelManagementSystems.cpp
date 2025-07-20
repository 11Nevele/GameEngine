#include "acpch.h"
#include "LevelManagementSystems.h"
#include "GameplayerSystems.h"

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
void AddWinCondition(World& world, Entity tilemap, Entity background, int x, int y, const std::string& description, std::function<bool(Number)> func)
{
	Entity e = world.Get<Tilemap>(tilemap).map[x][y];
	world.Add<WinCondition>(e, WinCondition(description, func));
	e = world.Get<Tilemap>(background).map[x][y];
	world.Get<Sprite>(e).textureID = world.GetResourse<TextureManager>().GetTextureID("Ground2");
}
void AddLevelEntrance(World& world, Entity tilemap, Entity background, int x, int y, Levels targetLevel, const std::string& str)
{
	Entity e = world.Get<Tilemap>(tilemap).map[x][y];
	world.Add<LevelEntrance>(e, {targetLevel, str});
	e = world.Get<Tilemap>(background).map[x][y];
	world.Get<Sprite>(e).textureID = world.GetResourse<TextureManager>().GetTextureID("Ground2");
}

void LevelManagementSystems::LoadLevel(World& world, Levels level)
{
	world.GetResourse<SceneData>().currentLevel = level;
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
		case LEVEL_2:
			Level2(world);
			break;
		case LEVEL_3:
			Level3(world);
			break;
		case LEVEL_4:
			Level4(world);
			break;
		case LEVEL_5:
			Level5(world);
			break;
		case LEVEL_6:
			Level6(world);
			break;
		case LEVEL_7:
			Level7(world);
			break;
		case LEVEL_8:
			Level8(world);
			break;
		case LEVEL_9:
			Level9(world);
			break;
	}
	auto t = world.View<Player, TilemapElement>().GetPacked();
	Tilemap& tile = world.Get<Tilemap>(std::get<1>(t[0].components).tilemap);
	GameplayerSystems::AddStep(world, tile);
}
void LoadMap(World& world, Entity tilemap, Entity background, vector<vector<int>> map)
{
	//flip the map vertically to match the tilemap coordinate system
	for (int y = 0; y < map.size() / 2; ++y)
	{
		for (int x = 0; x < map[0].size(); ++x)
		{
			std::swap(map[y][x], map[map[0].size() - 1 - y][x]);
		}
	}
	for (int x = 0; x < map.size(); ++x)
	{
		for (int y = 0; y < map[0].size(); ++y)
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
}

void LevelManagementSystems::TestLevel(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 100; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));

	
	LoadMap(world, tilemap, background, map);

	AddNumber(world, world.Get<Tilemap>(tilemap), 2, 3, 3); // Add a number at (2, 3) with value 1
	AddNumber(world, world.Get<Tilemap>(tilemap), 3, 3, 2); // Add a number at (3, 3) with value 2
	AddWinCondition(world, tilemap, background, 5, 6, "5", [](Number n) {return n.data == 5; });
}

void LevelManagementSystems::MainMenu(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 100; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,2,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddLevelEntrance(world, tilemap, background, 4, 4, Levels::LEVEL_1, "1");
	AddLevelEntrance(world, tilemap, background, 7, 4, Levels::LEVEL_2, "2");
	AddLevelEntrance(world, tilemap, background, 10, 4, Levels::LEVEL_3, "3");
	AddLevelEntrance(world, tilemap, background, 4, 7, Levels::LEVEL_4, "4");
	AddLevelEntrance(world, tilemap, background, 7, 7, Levels::LEVEL_5, "5");
	AddLevelEntrance(world, tilemap, background, 10, 7, Levels::LEVEL_6, "6");
	AddLevelEntrance(world, tilemap, background, 4, 10, Levels::LEVEL_7, "7");
	AddLevelEntrance(world, tilemap, background, 7, 10, Levels::LEVEL_8, "8");
	AddLevelEntrance(world, tilemap, background, 10, 10, Levels::LEVEL_9, "9");
}

void LevelManagementSystems::Level2(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 100; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,2,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddNumber(world, world.Get<Tilemap>(tilemap), 5, 7, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 7, 3);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 7, 5);
	AddNumber(world, world.Get<Tilemap>(tilemap), 9, 7, 4); 
	AddWinCondition(world, tilemap, background, 5, 9, "7", [](Number n) {return n.data == 7; });
	AddWinCondition(world, tilemap, background, 9, 5, "7", [](Number n) {return n.data == 7; });
}

void LevelManagementSystems::Level3(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 100; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,2,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddNumber(world, world.Get<Tilemap>(tilemap), 5, 7, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 7, 3);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 7, 5);
	AddNumber(world, world.Get<Tilemap>(tilemap), 9, 7, 4);
	AddWinCondition(world, tilemap, background, 5, 9, "25", [](Number n) {return n.data == 25; });
	AddWinCondition(world, tilemap, background, 9, 5, "81", [](Number n) {return n.data == 81; });
}

void LevelManagementSystems::Level4(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 100; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,1,0,0,0,0,1,1},
		{1,0,1,1,1,1,1,3,1,0,0,0,0,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{1,0,1,1,1,1,1,1,1,1,1,1,0,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{1,0,0,0,0,0,0,2,0,0,0,0,0,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 7, 3);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 7, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 4, 7, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 2, 7, 1);
	AddWinCondition(world, tilemap, background, 12, 2, ">10", [](Number n) {return n.data > 10; });
}

void LevelManagementSystems::Level1(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 100; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,2,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddNumber(world, world.Get<Tilemap>(tilemap), 7, 7, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 8, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 6, 3);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 6, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 8, 2);
	AddWinCondition(world, tilemap, background, 9, 9, "", [](Number n) {return true; });
	AddWinCondition(world, tilemap, background, 5, 9, "", [](Number n) {return true; });
}

void LevelManagementSystems::Level5(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 1000; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,2,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,1,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddNumber(world, world.Get<Tilemap>(tilemap), 7, 6, 3);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 6, 2);
	AddWinCondition(world, tilemap, background, 8, 8, "8", [](Number n) {return n.data == 8; });
}

void LevelManagementSystems::Level6(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 1000;// Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,2,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,1,0,0,0},
		{0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddNumber(world, world.Get<Tilemap>(tilemap), 5, 4, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 4, 3);
	AddNumber(world, world.Get<Tilemap>(tilemap), 7, 4, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 4, 4);
	AddNumber(world, world.Get<Tilemap>(tilemap), 9, 4, 5);
	AddWinCondition(world, tilemap, background, 06, 3, "min", [](Number n) {return n.data == 3; });
}

void LevelManagementSystems::Level9(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 100; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	//prime even small 
	vector<vector<int>> map = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,2,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);

	AddNumber(world, world.Get<Tilemap>(tilemap), 3, 12, 8);
	AddNumber(world, world.Get<Tilemap>(tilemap), 11, 2, 7);
	AddNumber(world, world.Get<Tilemap>(tilemap), 11, 12, 9);
	AddNumber(world, world.Get<Tilemap>(tilemap), 3, 2, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 8, 3);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 6, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 8, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 6, 3);
	AddWinCondition(world, tilemap, background, 2, 12, "<10", [](Number n) {return n.data < 10; });
	AddWinCondition(world, tilemap, background, 13, 2, "prime", [](Number n) 
		{
			if (n.data < 2) return false; // 0 and 1 are not prime
			for (int i = 2; i * i <= n.data; ++i) 
			{
				if (n.data % i == 0) return false; // Found a divisor, not prime
			}
			return true; // No divisors found, it's prime
			; 
		});
	AddWinCondition(world, tilemap, background, 12, 12, "<10", [](Number n) {return n.data < 10; });
	AddWinCondition(world, tilemap, background, 2, 2, "fibonacci ", [](Number n) 
		{
			int a = 0, b = 1;
			while (a < n.data) 
			{
				int temp = a;
				a = b;
				b = temp + b;
			}
			return a == n.data; // Check if we reached the number
		});
}

void LevelManagementSystems::Level8(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 1000; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,1,1,1,1,0,0,0,0,0,1},
		{1,1,1,1,1,1,0,0,1,0,0,0,0,0,1},
		{1,1,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{1,1,0,1,1,0,0,0,1,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,0,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1,1,1,0,0,1},
		{1,1,1,1,1,1,1,0,0,1,0,1,1,1,1},
		{1,0,0,0,0,0,1,1,1,1,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddNumber(world, world.Get<Tilemap>(tilemap), 2, 6, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 4, 6, 3);
	AddNumber(world, world.Get<Tilemap>(tilemap), 5, 6, 4);
	AddNumber(world, world.Get<Tilemap>(tilemap), 7, 6, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 7, 8, 2);
	AddWinCondition(world, tilemap, background, 7, 7, "8", [](Number n) {return n.data == 8; });
	AddWinCondition(world, tilemap, background, 13, 7, "25", [](Number n) {return n.data == 25; });
}

void LevelManagementSystems::Level7(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().mod = 1000; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,3,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,2,0,0,0,0,0,0,1},
		{1,0,0,0,3,0,0,0,0,0,0,3,0,0,1},
		{1,0,1,1,1,1,1,0,1,1,1,1,1,0,1},
		{1,1,1,0,0,0,1,0,1,0,0,0,1,1,1},
		{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64, 1));
	LoadMap(world, tilemap, background, map);
	AddNumber(world, world.Get<Tilemap>(tilemap), 2, 10, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 3, 10, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 5, 10, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 6, 10, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 8, 10, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 9, 10, 2);
	AddNumber(world, world.Get<Tilemap>(tilemap), 10, 10, 1);
	AddNumber(world, world.Get<Tilemap>(tilemap), 12, 10, 3);
	AddWinCondition(world, tilemap, background, 7, 1, "512", [](Number n) {return n.data == 512; });
	AddWinCondition(world, tilemap, background, 7, 3, "64", [](Number n) {return n.data == 64; });
}
