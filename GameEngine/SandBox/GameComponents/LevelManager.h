#pragma once
#include "acpch.h"
#include "Achoium.h"
#include "Components.h"
using namespace ac;


class LevelManager
{
public:
	static void ResetLevel(World& world);
	static void LoadLevel(World& world, Levels level, bool loadMap = true);
	static void TestLevel(World& world, bool loadMap = true);
	static void MainMenu(World& world, bool loadMap = true);
	static void Level1(World& world, bool loadMap = true);
	static void Level2(World& world, bool loadMap = true);
	static void Level3(World& world, bool loadMap = true);
	static void Level4(World& world, bool loadMap = true);
	static void Level5(World& world, bool loadMap = true);
	static void Level6(World& world, bool loadMap = true);
	static void Level7(World& world, bool loadMap = true);
	static void Level8(World& world, bool loadMap = true);
	static void Level9(World& world, bool loadMap = true);
};

