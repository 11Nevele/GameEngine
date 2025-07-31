#include "acpch.h"
#include "Components.h"



void MoveEntity(World& world, Entity e, int x, int y, int nx, int ny)
{
	Position& pos = world.Get<Position>(e);
	pos.x = nx;
	pos.y = ny;
	pos.targetX = nx;
	pos.targetY = ny;
	Transform& transform = world.Get<Transform>(e);
	transform.position = glm::vec3(nx * world.GetResourse<SceneData>().gridWidth, ny * world.GetResourse<SceneData>().gridHeight, -0.2f);
	world.GetResourse<MapInfo>().map[x][y].erase(std::remove(world.GetResourse<MapInfo>().map[x][y].begin(), world.GetResourse<MapInfo>().map[x][y].end(), e), world.GetResourse<MapInfo>().map[x][y].end());
	world.GetResourse<MapInfo>().map[nx][ny].push_back(e);
}

void RemoveEntity(World& world, Entity e)
{
	Position& pos = world.Get<Position>(e);
	world.GetResourse<MapInfo>().map[pos.x][pos.y].erase(std::remove(world.GetResourse<MapInfo>().map[pos.x][pos.y].begin(), world.GetResourse<MapInfo>().map[pos.x][pos.y].end(), e), world.GetResourse<MapInfo>().map[pos.x][pos.y].end());
	world.DeleteEntity(e);
}