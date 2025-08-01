#include "acpch.h"
#include "LevelManager.h"
#include "Achoium.h"
#include "Components.h"
#include "PlayerControlSystem.h"
//0 none
//1 wall
//2 spike
//3 start
//4 end
//5 healthKit
//6 player with no countdown

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
		Level2(world, loadMap);
		break;
	case LEVEL_3:
		//Level3(world, loadMap);
		break;
	case LEVEL_4:
		//Level4(world, loadMap);
		break;
	case LEVEL_5:
		//Level5(world, loadMap);
		break;
	case LEVEL_6:
		Level6(world, loadMap);
		break;
	case LEVEL_7:
		//Level7(world, loadMap);
		break;
	case LEVEL_8:
		//Level8(world, loadMap);
		break;
	case LEVEL_9:
		//Level9(world, loadMap);
		break;
	}
	PlayerControlSystem::SaveState(world);
	
}

void AddDoorAndButton(World& world, int x, int y, const std::vector<std::pair<int, int>>& buttonPositions, char doorColor, char buttonColor)
{
	std::vector<Entity> buttons;
	for(auto p : buttonPositions)
	{
		string buttonColorStr(1, buttonColor);  // ½«char×ª»»Îªstring
		Entity button = mCreate<Button>(world, p.first, p.second, "ButtonUnpressed" + buttonColorStr);
		world.Get<Button>(button).isPressed = false; // Initialize button state
		world.Get<Button>(button).color = buttonColor; // Set button color
		buttons.push_back(button);
	}
	Entity door = mCreate<Door>(world, x, y, "DoorClosed" + string(1,doorColor));
	world.Get<Door>(door).color = doorColor; // Set door color
	world.Get<Door>(door).keyIDs = buttons;
}
void AddLevelEntry(World& world, Levels level, int x, int y)
{
	Entity e = mCreate<LevelEntry>(world, x, y, "FinishPoint");
	world.Get<LevelEntry>(e).level = level; // Set the level for this entry
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
				switch (map[y][x])
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
			switch (map[y][x])
			{
				case 3: // Start point
				{
					SceneData& data = world.GetResourse<SceneData>();
					data.startX = x;
					data.startY = y;
					Entity player = mCreate<Player>(world, x, y, "PlayerDown");
					world.Get<Player>(player).round = data.currentRound; // Start moving down
					world.Add<CountDown>(player, CountDown(8));
				}
				break;
				case 4: // End point
				{
					mCreate<FinishPoint>(world, x, y, "FinishPoint");
				}
				break;
				case 5: // Health Kit
				{
					Entity healthKit = mCreate<HealthKit>(world, x, y, "HealthKit"); 
				}
				break;
				case 6: // Start point
				{
					SceneData& data = world.GetResourse<SceneData>();
					data.startX = x;
					data.startY = y;
					Entity player = mCreate<Player>(world, x, y, "PlayerDown");
					world.Get<Player>(player).round = data.currentRound; // Start moving down
				}
			default:
				break;
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
		{0,0,0,0,0,0,0,0,5,0,0,0,4,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,3,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	if (loadMap)
	{
		world.GetResourse<SceneData>().currentRound = 0; // Reset the current round
		Entity background = world.CreateEntity();
		world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
		Entity tilemap = world.CreateEntity();
		world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(tilemap, Transform(glm::vec3(0, 0, -0.1)));
		MapInfo& info = world.GetResourse<MapInfo>();
		info.tilemap = tilemap;
		info.background = background;
		info.map.resize(mapWidth, vector<vector<Entity>>(mapHeight));
		LoadMap(world, tilemap, background, map);
		AddDoorAndButton(world, 5, 5, { {0,0},{5,8},{6,1} }, 'R', 'R');
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
	world.GetResourse<SceneData>().startX = 1; // Set the global number to 100
	world.GetResourse<SceneData>().startY = 2; // Set the global number to 100
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
		{1,0,0,0,0,0,0,6,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	if (loadMap)
	{
		world.GetResourse<SceneData>().currentRound = 0; // Reset the current round
		Entity background = world.CreateEntity();
		world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
		Entity tilemap = world.CreateEntity();
		world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(tilemap, Transform(glm::vec3(0, 0, -0.1)));
		MapInfo& info = world.GetResourse<MapInfo>();
		info.tilemap = tilemap;
		info.background = background;
		info.map.resize(mapWidth, vector<vector<Entity>>(mapHeight));
		LoadMap(world, tilemap, background, map);
		
		//AddLevelEntry(world, TEST_LEVEL, 6, 6); // Add entry to test level
		AddLevelEntry(world, LEVEL_1, 1, 1); // Add entry to test level
		AddLevelEntry(world, LEVEL_2, 1, 2); // Add entry to test level
		AddLevelEntry(world, LEVEL_6, 1, 6); // Add entry to test level
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


void LevelManager::Level1(World& world, bool loadMap)
{
	world.GetResourse<SceneData>().gridHeight = 64; 
	world.GetResourse<SceneData>().gridWidth = 64; 

	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,4,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,2,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,2,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,3,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	};
	if (loadMap)
	{
		world.GetResourse<SceneData>().currentRound = 0; // Reset the current round
		Entity background = world.CreateEntity();
		world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
		Entity tilemap = world.CreateEntity();
		world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(tilemap, Transform(glm::vec3(0, 0, -0.1)));
		MapInfo& info = world.GetResourse<MapInfo>();
		info.tilemap = tilemap;
		info.background = background;
		info.map.resize(mapWidth, vector<vector<Entity>>(mapHeight));
		LoadMap(world, tilemap, background, map);

		AddDoorAndButton(world, 7,11, { {1,1}, }, 'R', 'R');
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


void LevelManager::Level2(World& world, bool loadMap)
{
	world.GetResourse<SceneData>().gridHeight = 64;
	world.GetResourse<SceneData>().gridWidth = 64;

	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,1,0,1,0,0,0,0,0,0,0,0,1,1 },
		{ 1,0,0,0,1,0,1,1,1,1,1,1,0,1,1 },
		{ 1,0,0,0,1,0,1,0,0,0,0,1,0,1,1 },
		{ 1,0,0,0,1,0,1,0,0,0,0,1,0,1,1 },
		{ 1,0,1,0,1,0,1,0,0,0,0,1,0,1,1 },
		{ 1,0,1,0,1,0,1,0,4,0,0,1,0,1,1 },
		{ 1,0,1,0,1,0,0,0,0,0,0,1,0,1,1 },
		{ 1,0,1,1,1,1,1,1,1,1,1,1,0,1,1 },
		{ 1,0,1,3,0,0,0,0,0,0,0,0,0,1,1 },
		{ 1,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	};
	if (loadMap)
	{
		world.GetResourse<SceneData>().currentRound = 0; // Reset the current round
		Entity background = world.CreateEntity();
		world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
		Entity tilemap = world.CreateEntity();
		world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(tilemap, Transform(glm::vec3(0, 0, -0.1)));
		MapInfo& info = world.GetResourse<MapInfo>();
		info.tilemap = tilemap;
		info.background = background;
		info.map.resize(mapWidth, vector<vector<Entity>>(mapHeight));
		LoadMap(world, tilemap, background, map);
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

void LevelManager::Level6(World& world, bool loadMap)
{
	world.GetResourse<SceneData>().gridHeight = 64;
	world.GetResourse<SceneData>().gridWidth = 64;

	const int mapWidth = 15;
	const int mapHeight = 15;
	vector<vector<int>> map = {
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,1,0,0,0,1,0,0,0,1,0,0,0,1 },
		{ 1,0,0,0,1,0,0,0,1,0,0,0,1,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,1,1,1 },
		{ 1,0,0,0,0,0,0,3,0,0,0,5,0,4,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,1 },
		{ 1,5,5,0,1,0,0,0,1,0,0,0,1,0,1 },
		{ 1,5,1,0,0,0,1,0,0,0,1,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	};
	if (loadMap)
	{
		world.GetResourse<SceneData>().currentRound = 0; // Reset the current round
		Entity background = world.CreateEntity();
		world.Add<ac::Tilemap>(background, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(background, Transform(glm::vec3(0, 0, 0)));
		Entity tilemap = world.CreateEntity();
		world.Add<ac::Tilemap>(tilemap, ac::Tilemap(mapWidth, mapHeight, 64, 64));
		world.Add<Transform>(tilemap, Transform(glm::vec3(0, 0, -0.1)));
		MapInfo& info = world.GetResourse<MapInfo>();
		info.tilemap = tilemap;
		info.background = background;
		info.map.resize(mapWidth, vector<vector<Entity>>(mapHeight));
		LoadMap(world, tilemap, background, map);
		AddDoorAndButton(world,12, 7, { {1,13},{1,12},{02,12}, }, 'R', 'R');
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