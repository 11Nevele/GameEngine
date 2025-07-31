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
	static void LoadLevel(World& world, Levels level, bool loadMap = true);
	static void TestLevel(World& world, bool loadMap = true);
	static void MainMenu(World& world, bool loadMap = true);
	static void Level1(World& world, bool loadMap = true);
};

