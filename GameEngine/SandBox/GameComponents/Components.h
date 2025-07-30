#pragma once
#include "acpch.h"
struct Position
{
	int x, y;
};
struct PlayerControl
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
};

//scene components
struct Wall
{ };
struct Spike
{
};