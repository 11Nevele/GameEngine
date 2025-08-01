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
};

