#include "acpch.h"
#include "RenderTileSystems.h"
#include "../GameComponents/GameComponents.h"

uint32_t RenderTileSystems::GroundSpriteID = 0;
uint32_t RenderTileSystems::WallSpriteID = 0;
uint32_t RenderTileSystems::BoxSpriteID = 0;
uint32_t RenderTileSystems::NumberSpriteID = 0;
uint32_t RenderTileSystems::PlayerSpriteID = 0;

void RenderTileSystems::Init(World& world)
{
	TextureManager& textureManager = world.GetResourse<TextureManager>();
	GroundSpriteID = textureManager.GetTextureID("Ground1");
	WallSpriteID = textureManager.GetTextureID("Wall1");
	BoxSpriteID = textureManager.GetTextureID("Box1");
	NumberSpriteID = textureManager.GetTextureID("Box2");
}

void RenderTileSystems::UpdateTileSprites(World& world)
{
	world.View<Box, Sprite>().ForEach([&world](Entity entity, Box& b, Sprite& sprite)
	{
			sprite.textureID = BoxSpriteID;
			
	});
	world.View<Number, Sprite>().ForEach([&world](Entity entity, Number& b, Sprite& sprite)
		{
			sprite.textureID = NumberSpriteID;

		});
}

void RenderTileSystems::UpdateNumber(World& world)
{
	OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
	world.View<Number, TilemapElement>().ForEach([&world, &renderer](Entity entity, Number& number, TilemapElement& tilemapElement)
	{
		std::string str = std::to_string(number.data);
		renderer.SubmitText(str, Transform(glm::vec3(tilemapElement.x * 64 +32, tilemapElement.y * 64 +32, 0), glm::vec3(1,1,1)));
	});
}

void RenderTileSystems::UpdateWinCondition(World& world)
{
	OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
	world.View<WinCondition, TilemapElement>().ForEach([&world, &renderer](Entity entity, WinCondition& condition, TilemapElement& tilemapElement)
		{
			renderer.SubmitText(condition.description, Transform(glm::vec3(tilemapElement.x * 64+32, tilemapElement.y * 64 + 32, 0), glm::vec3(1, 0, 1)));
		});
	world.View<LevelEntrance, TilemapElement>().ForEach([&world, &renderer](Entity entity, LevelEntrance& entrance, TilemapElement& tilemapElement)
		{
			renderer.SubmitText(entrance.description, Transform(glm::vec3(tilemapElement.x * 64 + 32, tilemapElement.y * 64 + 32, 0), glm::vec3(0.5, 1, 1)));
		});
}
