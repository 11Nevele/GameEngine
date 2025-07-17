#include "acpch.h"
#include "Tilemap.h"
#include "Debug.h"
namespace ac
{
	ac::Tilemap::Tilemap(uint32_t width, uint32_t height, uint32_t gridWidth, uint32_t gridHeight):
		map(width, std::vector<Entity>(height, 0)), gridWidth(gridWidth), gridHeight(gridHeight)
	{

	}
	TilemapElement::TilemapElement(Entity tilemap, uint32_t xPos, uint32_t yPos):
		tilemap(tilemap), x(xPos), y(yPos)
	{
	}
	bool OnTilemapElementAdded(const OnAdded<TilemapElement>& event)
	{
		
		if (!event.world.Has<Tilemap>(event.component.tilemap))
		{
			ACMSG("WARNING: TilemapElement " << event.ID << " is trying to register a non exist Tilemap: " << event.component.tilemap);
			return true;
		}
		Tilemap& map = event.world.Get<Tilemap>(event.component.tilemap);
		if (event.component.x >= map.map.size() || event.component.y >= map.map[0].size())
		{
			map.map.resize(event.component.x, std::vector<Entity>(event.component.y));
		}
		if (map.map[event.component.x][event.component.y] != 0)
		{
			ACMSG("WARNING: In Tilemap " << event.ID << "grid " <<
				event.component.x << " " << event.component.y <<
				"is used twice by " << map.map[event.component.x][event.component.y] <<
				" and " << event.ID);
		}
		map.map[event.component.x][event.component.y] = event.ID;;
		return true;
	}
	bool OnTilemapElementDeleted(const OnDeleted<TilemapElement>& event)
	{
		if (!event.world.Has<Tilemap>(event.component.tilemap))
		{
			ACMSG("WARNING: TilemapElement " << event.ID << " is trying to unregister from non exist Tilemap: " << event.component.tilemap);
			return true;
		}
		Tilemap& map = event.world.Get<Tilemap>(event.component.tilemap);
		if (event.component.x >= map.map.size() || event.component.y >= map.map[0].size())
		{
			ACMSG("WARNING: TilemapElement " << event.ID << " is out of bound for: " << event.component.tilemap << " during unregister");
			return true;
		}
		map.map[event.component.x][event.component.y] = 0;
		return true;
	}
	bool OnTilemapDeleted(const OnAdded<Tilemap>& event)
	{
		return true;
	}
}

