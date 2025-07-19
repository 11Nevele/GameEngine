#include "acpch.h"
#include "RenderTileSystems.h"
#include "../GameComponents/GameComponents.h"

uint32_t RenderTileSystems::GroundSpriteID = 0;
uint32_t RenderTileSystems::WallSpriteID = 0;

void RenderTileSystems::Init(World& world)
{
	TextureManager& textureManager = world.GetResourse<TextureManager>();
	GroundSpriteID = textureManager.GetTextureID("Ground1");
	WallSpriteID = textureManager.GetTextureID("Wall1");
}

void RenderTileSystems::UpdateTileSprites(World& world)
{
	//world.View<TilemapElement, Sprite>().ForEach([&world](Entity entity, TilemapElement& tilemapElement, Sprite& sprite)
	//{
	//	if(world.Has<Wall>(entity))
	//		sprite.textureID = RenderTileSystems::WallSpriteID;
	//	else 
	//		sprite.textureID = RenderTileSystems::GroundSpriteID;
	//});
}

void RenderTileSystems::UpdateNumber(World& world)
{
	OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
	world.View<Number, TilemapElement>().ForEach([&world, &renderer](Entity entity, Number& number, TilemapElement& tilemapElement)
	{
		std::string str = std::to_string(number.data);
		renderer.SubmitText(str, Transform(glm::vec3(tilemapElement.x * 64, tilemapElement.y * 64, 0), glm::vec3(1,1,1)));
	});
}
