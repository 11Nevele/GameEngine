#pragma once
#include "Achoium.h"
using namespace ac;

class RenderTileSystems
{
private:
	static uint32_t GroundSpriteID;
	static uint32_t WallSpriteID;
	static uint32_t BoxSpriteID;
	static uint32_t NumberSpriteID;
	static uint32_t PlayerSpriteID;
public:
	static void Init(World& world);
	static void UpdateTileSprites(World& world);
	static void UpdateNumber(World& world);
	static void UpdateWinCondition(World& world);
};