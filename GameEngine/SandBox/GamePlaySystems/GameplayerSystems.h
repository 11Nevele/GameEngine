#pragma once
#include "acpch.h"
#include "Achoium.h"
#include "../GameComponents/GameComponents.h"
using namespace ac;
class GameplayerSystems
{
public:
	static void PlayerMovement(World& world);
	static void ResetStepHistory()
	{
		stepHistory.clear();
	}
	static void AddStep(World& world, Tilemap& tilemap);
	static void UndoLastStep(World& world);
private:
	struct StepInfo
	{
		std::vector<std::pair<uint32_t, uint32_t>> playerPos;
		std::vector<std::pair<uint32_t, uint32_t>> boxPos;
		std::vector<std::pair<uint32_t, uint32_t>> numberPos;
		std::vector<Number> numbers; // Store numbers for undo
	};
	static std::vector<StepInfo> stepHistory;
	//0=can not move, 1=can move, 2=can combine
	static int HasSpaceToMove(World& world, Tilemap& tilemap, uint32_t x, uint32_t y, int dx, int dy);
};

