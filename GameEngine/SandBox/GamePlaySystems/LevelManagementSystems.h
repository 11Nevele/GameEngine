#pragma once
#include "Achoium.h"
#include "../GameComponents/GameComponents.h"
#include "../GamePlaySystems/LevelManagementSystems.h"
using namespace ac;

enum Levels
{
	MAIN_MENU,
	TEST_LEVEL,
	LEVEL_1
};

class LevelManagementSystems
{
public:
	static void ResetLevel(World& world);
	static void LoadLevel(World& world, Levels level);
	static void TestLevel(World& world);
	static void MainMenu(World& world);
	static void Level1(World& world);

};

