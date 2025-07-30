#include "acpch.h"
#include "LevelManager.h"
#include "Achoium.h"
#include "Components.h"


void LevelManager::ResetLevel(World& world)
{
	world.View<Sprite>().ForEach([&world](Entity entity, Sprite& sprite)
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


void LevelManager::LoadLevel(World& world, Levels level)
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
			
			switch (map[x][y])
			{
			case 1:
				world.Add<Wall>(tile, Wall());
				world.Add<Sprite>(tile, Sprite::Create("Wall", world.GetResourse<TextureManager>()));
				break;
			case 2:
				world.Add<Spike>(tile, Spike());
				world.Add<Sprite>(tile, Sprite::Create("Spike", world.GetResourse<TextureManager>()));
			}
			Entity backgroundTile = world.CreateEntity();
			world.Add<TilemapElement>(backgroundTile, TilemapElement(background, x, y));
			world.Add<Sprite>(backgroundTile, Sprite::Create("Ground", world.GetResourse<TextureManager>()));
		}
	}
}

void LevelManager::TestLevel(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(tilemap, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);

}

void LevelManager::MainMenu(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}

void LevelManager::Level2(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}

void LevelManager::Level3(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);

}

void LevelManager::Level4(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}

void LevelManager::Level1(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}

void LevelManager::Level5(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}

void LevelManager::Level6(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}

void LevelManager::Level9(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}

void LevelManager::Level8(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}

void LevelManager::Level7(World& world)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{0,0,0,0,2,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	Entity background = world.CreateEntity();
	world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
	Entity tilemap = world.CreateEntity();
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
	world.Add<Transform>(background, Transform(glm::vec3(0, 0, -0.1)));
	LoadMap(world, tilemap, background, map);
}