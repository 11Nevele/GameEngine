#pragma once
#include "Achoium.h"
using namespace ac;
class PlayerControlSystem
{
private:
	struct step
	{

	};
	//previous steps use for undo
	static std::vector<step> history;
	static bool isInAnimation;
	static float animationTime;
	static bool MovementSystem(World& world);//return if any action was performed
	static bool SuicideSystem(World& world);
	static bool HealSystem(World& world);
	static void NextStep(World& world);
	
public:

	static void UndoSystem(World& world);
	static void PlayerControl(World& world);
	static void ResetSystem(World& world);
	static void NewTurnSystem(World& world);
	static void AnimationSystem(World& world);

};

