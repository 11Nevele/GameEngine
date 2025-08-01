#pragma once
#include "Achoium.h"
#include "Components.h"
using namespace ac;
class PlayerControlSystem
{
private:
	struct step
	{
		// ����ʵ����Ϣ
		struct EntityState {
			Position position;
			bool hasPlayer;
			Player player;
			bool hasPlayerReplay;
			PlayerReplay playerReplay;
			bool hasGhost;
			bool hasCountDown;
			CountDown countdown;
			uint32_t spriteTextureID;
		};
		

		// ��Ϸ����״̬
		std::vector<EntityState> entityStates;
		std::vector<Position> coorpses; // �洢ʬ��ʵ��ID
		std::vector<Position> healthKits; // �洢������ʵ��ID
		
		int currentRound;

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
	static void SaveState(World& world);
	static void UndoSystem(World& world);
	static void PlayerControl(World& world);
	static void ResetSystem(World& world);
	static void NewTurnSystem(World& world);
	static void AnimationSystem(World& world);
};

