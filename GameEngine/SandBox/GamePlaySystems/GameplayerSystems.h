#pragma once
#include "Achoium.h"
#include "../GameComponents/GameComponents.h"
using namespace ac;
class GameplayerSystems
{
public:
	static void PlayerMovement(World& world);
private:
	//0=can not move, 1=can move, 2=can combine
	static int HasSpaceToMove(World& world, Tilemap& tilemap, uint32_t x, uint32_t y, int dx, int dy);
};

