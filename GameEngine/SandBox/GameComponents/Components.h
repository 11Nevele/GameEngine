#pragma once
#include "acpch.h"
#include "Achoium.h"
using namespace ac;
struct Position
{
	int x = 0, y = 0;
	int targetX = 0, targetY = 0; // 目标位置
	int dx = 0, dy = 0;
};
struct Player
{
	std::vector<std::pair<int, int>> positions;
};
struct CountDown
{
	int remain = 0;
};
struct Ghost
{

};
struct PlayerReplay 
{
	int curInd = 0;
	std::vector<Position> positions;
	std::vector<bool> isGhost;
};
struct Button
{
	bool isPressed = false;
};
struct Door
{
	std::vector<uint32_t> keyIDs; // 按钮ID
	bool isOpen = false; // 门是否打开
};
struct Coorpse
{
	int type = 0;
};
struct SceneData
{
	int currentLevel = 0;
	int gridWidth = 64; // Set the global number to 100
	int gridHeight = 64; // Set the global number to 100
	int startX = 0; // Start position X
	int startY = 0; // Start position Y
};
struct MapInfo
{
	Entity tilemap;
	Entity background;
	std::vector<std::vector<std::vector<Entity>>> map; // 3D vector to store entities at each tile position
};

//scene components
struct Wall
{ };
struct Spike
{
};