#pragma once
#include <acpch.h>
#include "Core/World.hpp"
#include "Core/ECSEvents.h";
namespace ac
{
	struct Tilemap
	{
		std::vector<std::vector<Entity>> map;
		uint32_t gridWidth, gridHeight;
		uint32_t layer = 0;
		Tilemap() = default;
		Tilemap(uint32_t width, uint32_t height, uint32_t gridWidth, uint32_t gridHeight, uint32_t layer = 0);
	};
	struct TilemapElement
	{
		Entity tilemap;
		uint32_t x, y;
		TilemapElement() = default;
		TilemapElement(Entity tilemap, uint32_t xPos, uint32_t yPos);
	};

	bool OnTilemapElementAdded(const OnAdded< TilemapElement>& event);
	bool OnTilemapElementDeleted(const OnDeleted<TilemapElement>& event);
	bool OnTilemapDeleted(const OnAdded< Tilemap>& event);
}