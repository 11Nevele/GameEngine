#pragma once
#include "Achoium.h"
using namespace ac;
class StorySystem
{
public:
	static bool storyPlayed;
	static int stepIndex;
	static Entity spriteEntity;
	static Entity textEntity;
	static void Init(Entity spriteID, Entity textID);
	static void StoryUpdateSystem(World& world);
};

