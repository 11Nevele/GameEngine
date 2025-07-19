#pragma once
#include "Achoium.h"
#include "../GameComponents/GameComponents.h"
#include "../GamePlaySystems/LevelManagementSystems.h"
using namespace ac;



class LevelManagementSystems
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


};

