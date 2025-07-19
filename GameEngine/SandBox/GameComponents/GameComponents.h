#pragma once
#include "Achoium.h"
using namespace ac;
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
	uint32_t upTextureID;
	uint32_t downTextureID;
	uint32_t leftTextureID;
	uint32_t rightTextureID;
	static Player Create(World& world)
	{
		TextureManager& tm = world.GetResourse<TextureManager>();
		Player player;
		player.upTextureID = tm.GetTextureID("PlayerUp");
		player.downTextureID = tm.GetTextureID("PlayerDown");
		player.leftTextureID = tm.GetTextureID("PlayerLeft");
		player.rightTextureID = tm.GetTextureID("PlayerRight");
		return player;
	}
};
struct WinCondition
{
	std::function<bool(Number)> checkWin = nullptr;
	WinCondition(std::function<bool(Number)> checkFunc) : checkWin(checkFunc) {}
};

struct SceneData
{
	long long mod;
	uint32_t gridWidth;
	uint32_t gridHeight;
	SceneData(long long modValue, uint32_t width, uint32_t height)
		: mod(modValue), gridWidth(width), gridHeight(height) {
	}
};