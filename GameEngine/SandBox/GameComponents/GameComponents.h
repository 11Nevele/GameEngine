#pragma once
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
	LEVEL_9,
	LEVEL_10
};
struct Wall
{

};

struct Box
{
	
};
struct Number
{
	long long data = 0;
};
struct Player
{
	int dx, dy;
	uint32_t upTextureID;
	uint32_t downTextureID;
	uint32_t leftTextureID;
	uint32_t rightTextureID;
	static Player Create(World& world)
	{
		
		TextureManager& tm = world.GetResourse<TextureManager>();
		Player player;
		player.dx = 0;
		player.dy = -1;
		player.upTextureID = tm.GetTextureID("PlayerUp");
		player.downTextureID = tm.GetTextureID("PlayerDown");
		player.leftTextureID = tm.GetTextureID("PlayerLeft");
		player.rightTextureID = tm.GetTextureID("PlayerRight");
		return player;
	}
};
struct WinCondition
{
	std::string description;
	std::function<bool(Number)> checkWin = nullptr;
	WinCondition(const std::string& str, std::function<bool(Number)> checkFunc) : description(str), checkWin(checkFunc) {}
};

struct SceneData
{
	long long mod;
	Levels currentLevel = Levels::MAIN_MENU;
	uint32_t gridWidth;
	uint32_t gridHeight;
	SceneData(long long modValue, uint32_t width, uint32_t height)
		: mod(modValue), gridWidth(width), gridHeight(height) {
	}
};

struct LevelEntrance
{
	Levels targetLevel;
	std::string description;
};