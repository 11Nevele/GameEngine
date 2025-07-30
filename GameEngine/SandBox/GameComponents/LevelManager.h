#pragma once
#include "acpch.h"
#include "Achoium.h"
using namespace ac;
enum Levels
{
	MAIN_MENU,
	TEST_LEVEL,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4,
	LEVEL_5,
	LEVEL_6,
	LEVEL_7,
	LEVEL_8,
	LEVEL_9
};

class LevelManager
{
public:
	static void ResetLevel(World& world);
	static void LoadLevel(World& world, Levels level);
	static void TestLevel(World& world);
	static void MainMenu(World& world);
	static void Level1(World& world);
	static void Level2(World& world);
	static void Level3(World& world);
	static void Level4(World& world);
	static void Level5(World& world);
	static void Level6(World& world);
	static void Level7(World& world);
	static void Level8(World& world);
	static void Level9(World& world);
};

