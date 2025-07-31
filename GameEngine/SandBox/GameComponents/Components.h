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
struct direction
{
	int dx = 0, dy = 0; // 方向向量
};
struct Player
{
	std::vector<direction> directions; // 方向向量列表
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
	std::vector<direction> directions;
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

// 函数声明（实现移到 Components.cpp）
template<typename T>
Entity mCreate(World& world, int x, int y, const std::string& spritename)
{
	Entity entity = world.CreateEntity();
	world.Add<T>(entity, T());
	world.Add<Transform>(entity, Transform(glm::vec3(x * world.GetResourse<SceneData>().gridWidth, y * world.GetResourse<SceneData>().gridHeight, -0.2)));
	world.Add<Sprite>(entity, Sprite::Create(spritename, world.GetResourse<TextureManager>()));
	world.Add<Position>(entity, Position(x, y, x, y, 0, 0));
	world.GetResourse<MapInfo>().map[x][y].push_back(entity);
	return entity;
}

void MoveEntity(World& world, Entity e, int x, int y, int nx, int ny);

void RemoveEntity(World& world, Entity e);