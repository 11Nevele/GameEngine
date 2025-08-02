#include "acpch.h"
#include "LevelManager.h"
#include "StorySystem.h"

bool StorySystem::storyPlayed = false;
int StorySystem::stepIndex = 1;
Entity StorySystem::spriteEntity = NULL_ENTITY;
Entity StorySystem::textEntity = NULL_ENTITY;


void StorySystem::Init(Entity spriteID, Entity textID)
{
	storyPlayed = false;
	stepIndex = 1;
	spriteEntity = spriteID;
	textEntity = textID;
}
void StorySystem::StoryUpdateSystem(World& world)
{
	if (storyPlayed)
		return;
	if (world.GetResourse<InputManager>().IsKeyDown(AC_KEY_SPACE))
		{
		stepIndex++;
	}
	Sprite& sprite = world.Get<Sprite>(spriteEntity);

	switch (stepIndex)
	{
	case 1: case 2: case 3:case 4: case 5: case 6: case 7: case 8:
		sprite.textureID = world.GetResourse<TextureManager>().GetTextureID("Story" + to_string(stepIndex));
		break;
	case 9:
		storyPlayed = true;
		world.DeleteEntity(spriteEntity); // Remove sprite entity
		world.DeleteEntity(textEntity); // Remove text entity
		LevelManager::LoadLevel(world, BEGINNING_LEVEL); // Load the beginning level

	}
}
