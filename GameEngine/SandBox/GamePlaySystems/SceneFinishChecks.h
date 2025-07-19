#pragma once
#include "LevelManagementSystems.h"
class SceneFinishChecks
{

public:
	static void CheckWinLevel(World& world);
	static void CheckExitLevel(World& world);
	static void CheckRestartLevel(World& world);
	static void CheckTransitionLevel(World& world);
};

