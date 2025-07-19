#include "acpch.h"
#include "SceneFinishChecks.h"

void SceneFinishChecks::CheckWinLevel(World& world)
{
	auto list = world.View<WinCondition, TilemapElement>().GetPacked();
	if(list.size() == 0)
	{
		return; // No win condition found
	}
	bool win = true;
	for(auto [Entity, component] : list)
	{
		WinCondition& wc = std::get<0>(component);
		TilemapElement& tilemap = std::get<1>(component);
		if(!world.Has<Number>(Entity))
		{
			win = false; // No number component found
			return;
		}
		Number& number = world.Get<Number>(Entity);
		if(!wc.checkWin(number))
		{
			win = false; // Win condition not met
			return;
		}
	}
	auto list2 = world.View<Number, TilemapElement>().GetPacked();
	if(list2.size() == 0)
	{
		return;
	}
	for (auto [Entity, component] : list2)
	{
		Number& n = std::get<0>(component);
		TilemapElement& tilemap = std::get<1>(component);
		if (!world.Has<WinCondition>(Entity))
		{
			win = false; // No number component found
			return;
		}
		WinCondition& wc = world.Get<WinCondition>(Entity);
		if (!wc.checkWin(n))
		{
			win = false; // Win condition not met
			return;
		}
	}

	if (win)
	{
		PlaySoundA("Assets/Audio/bell.wav", NULL, SND_FILENAME | SND_SYNC);
		LevelManagementSystems::ResetLevel(world);
		LevelManagementSystems::LoadLevel(world, Levels::MAIN_MENU);
	}
}

void SceneFinishChecks::CheckExitLevel(World& world)
{
	InputManager& input = world.GetResourse<InputManager>();
	if(input.IsKeyDown(AC_KEY_ESCAPE))
	{
		LevelManagementSystems::ResetLevel(world);
		LevelManagementSystems::LoadLevel(world, Levels::MAIN_MENU);
	}
}

void SceneFinishChecks::CheckRestartLevel(World& world)
{
	InputManager& input = world.GetResourse<InputManager>();
	if(input.IsKeyDown(AC_KEY_R))
	{
		LevelManagementSystems::ResetLevel(world);
		LevelManagementSystems::LoadLevel(world, world.GetResourse<SceneData>().currentLevel);
	}
}

void SceneFinishChecks::CheckTransitionLevel(World& world)
{
	world.View<LevelEntrance, Player>().ForEach([&world](Entity entity, LevelEntrance& entrance, Player& player)
	{
		Levels l = entrance.targetLevel;
		LevelManagementSystems::ResetLevel(world);
		LevelManagementSystems::LoadLevel(world, l);
	});
}
