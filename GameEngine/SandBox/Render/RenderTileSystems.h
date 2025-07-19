#pragma once
#include "Achoium.h"
using namespace ac;

class RenderTileSystems
{
private:
	static uint32_t GroundSpriteID;
	static uint32_t WallSpriteID;
public:
	static void Init(World& world);
	static void UpdateTileSprites(World& world);
	static void UpdateNumber(World& world);
};