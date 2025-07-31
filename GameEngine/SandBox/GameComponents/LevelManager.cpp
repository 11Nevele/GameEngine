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


void LevelManager::LoadLevel(World& world, Levels level, bool loadMap)
{
	world.GetResourse<SceneData>().currentLevel = level;
	switch (level)
	{
	case MAIN_MENU:
		MainMenu(world, loadMap);
		break;
	case TEST_LEVEL:
		TestLevel(world, loadMap);
		break;
	case LEVEL_1:
		Level1(world, loadMap);
		break;
	case LEVEL_2:
		break;
	case LEVEL_3:
		break;
	case LEVEL_4:
		break;
	case LEVEL_5:
		break;
	case LEVEL_6:
		break;
	case LEVEL_7:
		break;
	case LEVEL_8:
		break;
	case LEVEL_9:
		break;
	}
	SceneData& data = world.GetResourse<SceneData>();
	Entity player = mCreate<Player>(world, data.startX, data.startY, "PlayerDown");
	world.Add<CountDown>(player, CountDown(8));
}

void AddDoorAndButton(World& world, int x, int y, const std::vector<std::pair<int, int>>& buttonPositions)
{
	std::vector<Entity> buttons;
	for(auto p : buttonPositions)
	{
		Entity button = mCreate<Button>(world, p.first, p.second, "ButtonUnpressed");
		world.Get<Button>(button).isPressed = false; // Initialize button state
		buttons.push_back(button);
	}
	Entity door = mCreate<Door>(world, x, y, "DoorClosed");
	world.Get<Door>(door).keyIDs = buttons;
}
void LoadMap(World& world, Entity tilemap, Entity background, vector<vector<int>> map, bool loadTile = true)
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
			if (loadTile)
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
			}
			
			
			
			if (loadTile)
			{
				Entity backgroundTile = world.CreateEntity();
				world.Add<TilemapElement>(backgroundTile, TilemapElement(background, x, y));
				world.Add<Sprite>(backgroundTile, Sprite::Create("Ground", world.GetResourse<TextureManager>()));
			}
			
		}
	}
}

void LevelManager::TestLevel(World& world, bool loadMap)
{
	world.GetResourse<SceneData>().gridHeight = 64; // Set the global number to 100
	world.GetResourse<SceneData>().gridWidth = 64; // Set the global number to 100
	world.GetResourse<SceneData>().startX = 1; // Set the global number to 100
	world.GetResourse<SceneData>().startY = 2; // Set the global number to 100
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
	if (loadMap)
	{
		Entity background = world.CreateEntity();
		world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
		Entity tilemap = world.CreateEntity();
		world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(tilemap, Transform(glm::vec3(0, 0, -0.1)));
		LoadMap(world, tilemap, background, map);
		MapInfo& info = world.GetResourse<MapInfo>();
		info.tilemap = tilemap;
		info.background = background;
		info.map.resize(mapWidth, vector<vector<Entity>>(mapHeight));
		AddDoorAndButton(world, 5, 5, { {0,0},{5,8},{6,1} });
	}
	else
	{
		Entity background = world.GetResourse<MapInfo>().background;
		Entity tilemap = world.GetResourse<MapInfo>().tilemap;
		if (background == NULL_ENTITY || tilemap == NULL_ENTITY)
		{
			cout << "Map not loaded, cannot load test level." << endl;
			return;
		}
		LoadMap(world, tilemap, background, map, false);
	}


}

void LevelManager::MainMenu(World& world, bool loadMap)
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


void LevelManager::Level1(World& world, bool loadMap)
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
