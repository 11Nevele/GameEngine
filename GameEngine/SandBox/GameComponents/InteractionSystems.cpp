#include "acpch.h"
#include "InteractionSystems.h"
#include "Achoium.h"
#include "Components.h"
#include "LevelManager.h"
#include "PlayerControlSystem.h"
#include "StorySystem.h"
void InteractionSystems::CheckSpike(World& world)
{
	
	world.View<Position, PlayerReplay>().ForEach([&world](Entity entity, Position& pos, PlayerReplay& player)
		{
			if (world.Has<Ghost>(entity))
			{
				// Player is out of bounds, handle accordingly
				return;
			}
			// Check if the player is on a spike tile
			auto& map = world.GetResourse<MapInfo>().map;
			Tilemap& tilemap = world.Get<Tilemap>(world.GetResourse<MapInfo>().tilemap);
			if (world.Has<Spike>(tilemap.map[pos.x][pos.y]))
			{
				bool hasCoorpse = false;
				for (const auto& entityVec : map[pos.x][pos.y])
				{
					if (world.Has<Coorpse>(entityVec))
					{

						hasCoorpse = true;
						break;
					}
				}
				if (!hasCoorpse)
				{
					world.Add<Ghost>(entity, Ghost());
					world.Delete<CountDown>(entity); // 删除倒计时组件
					world.Get<Sprite>(entity).textureID = world.GetResourse<TextureManager>().GetTextureID("GhostDown");
					mCreate<Coorpse>(world, pos.x, pos.y, "Coorpse1");
				}
			}

		});
	world.View<Position, Player>().ForEach([&world](Entity entity, Position& pos, Player& player)
		{
			if (world.Has<Ghost>(entity))
			{
				// Player is out of bounds, handle accordingly
				return;
			}
			// Check if the player is on a spike tile
			auto& map = world.GetResourse<MapInfo>().map;
			Tilemap& tilemap = world.Get<Tilemap>(world.GetResourse<MapInfo>().tilemap);
			if (world.Has<Spike>(tilemap.map[pos.x][pos.y]))
			{
				bool hasCoorpse = false;
				for (const auto& entityVec : map[pos.x][pos.y])
				{
					if (world.Has<Coorpse>(entityVec))
					{
						hasCoorpse = true;
						break;
					}
				}
				if (!hasCoorpse)
				{
					world.Add<Ghost>(entity, Ghost());
					world.Delete<CountDown>(entity); // 删除倒计时组件
					world.Get<Sprite>(entity).textureID = world.GetResourse<TextureManager>().GetTextureID("GhostDown");
					mCreate<Coorpse>(world, pos.x, pos.y, "Coorpse1");
				}
			}

		});
}

void InteractionSystems::CheckButton(World& world)
{
	auto& map = world.GetResourse<MapInfo>().map;
	world.View<Button, Position>().ForEach([&world, &map](Entity entity, Button& button, Position& pos)
		{
			bool open = false;
			for(Entity e : map[pos.x][pos.y])
			{
				if((world.Has<Player>(e) && !world.Has<Ghost>(e)) || (world.Has<PlayerReplay>(e) && !world.Has<Ghost>(e)) || world.Has<Coorpse>(e))
					{
					button.isPressed = true;
					open = true;
					break;
				}
			}
			button.isPressed = open;
		});
	world.View<Door, Position>().ForEach([&world](Entity entity, Door& door, Position& pos)
		{
			bool open = true;
			for (uint32_t keyID : door.keyIDs)
			{
				open &= world.Has<Button>(keyID) && world.Get<Button>(keyID).isPressed;
			}
			door.isOpen = open;
		});
}

void InteractionSystems::CheckDoor(World& world)
{
	
}

void InteractionSystems::CountDownSystem(World& world)
{
	OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
	world.View<CountDown, Position, Transform>().ForEach([&world, &renderer](Entity entity, CountDown& countDown, Position& pos, Transform& transform)
		{
			if (countDown.remain > 0)
			{
			}
			else
			{
				world.Delete<CountDown>(entity); // 删除倒计时组件
				world.Add<Ghost>(entity, Ghost()); // 添加鬼魂组件
				world.Get<Sprite>(entity).textureID = world.GetResourse<TextureManager>().GetTextureID("GhostDown");
				mCreate<Coorpse>(world, pos.x, pos.y, "Coorpse1"); // 创建尸体实体
			}
		});
}

void InteractionSystems::RenderText(World& world)
{
	if (StorySystem::storyPlayed == false)
		return;
	OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
	world.View<CountDown, Position, Transform>().ForEach([&world, &renderer](Entity entity, CountDown& countDown, Position& pos, Transform& transform)
		{
				Transform t = transform;
				t.scale = glm::vec3(0.7f, 0.7f, 1.0f); // Adjust scale for text rendering
				t.position += glm::vec3(world.GetResourse<SceneData>().gridWidth / 2.0f, world.GetResourse<SceneData>().gridHeight, -0.5f);
				renderer.SubmitText(to_string(countDown.remain),
					t, { 1,1,1 }, { 0.5,0.5 });

		});
	renderer.SubmitText("Timeline: " + to_string(world.GetResourse<SceneData>().currentRound + 1),
		Transform(glm::vec3(0, 0, -0.5f)), { 1,1,1 }, { 0,0 });
}

void InteractionSystems::CheckHealthKit(World& world)
{
	auto& map = world.GetResourse<MapInfo>().map;
	world.View<Position, HealthKit>().ForEach([&world, &map](Entity entity, Position& pos, HealthKit& hk)
		{
			//get all entities at the position and check if any player or player replay is present
			for(Entity e : map[pos.x][pos.y])
			{
				if ((world.Has<Player>(e) && !world.Has<Ghost>(e)))
				{
					// Player or player replay is present, remove health kit
					
					world.View<PlayerReplay, Ghost>().ForEach([&world, e, entity](Entity ee, PlayerReplay& replay, Ghost& ghost)
					{
							if (replay.round == world.Get<Player>(e).round - 1)
							{
								world.Delete<Ghost>(ee);
								world.Get<Sprite>(ee).textureID = world.GetResourse<TextureManager>().GetTextureID("PlayerDown");
								world.Add<CountDown>(ee, CountDown(8)); // Add countdown component
								RemoveEntity(world, entity);
							}
							
					});
					break; // Exit the loop after finding a player
				}
				if ((world.Has<PlayerReplay>(e) && !world.Has<Ghost>(e)))
				{
					// Player or player replay is present, remove health kit
					
					world.View<PlayerReplay, Ghost>().ForEach([&world, e, entity](Entity ee, PlayerReplay& replay, Ghost& ghost)
						{
							if (replay.round == world.Get<PlayerReplay>(e).round - 1)
							{
								world.Delete<Ghost>(ee);
								world.Get<Sprite>(ee).textureID = world.GetResourse<TextureManager>().GetTextureID("PlayerDown");
								world.Add<CountDown>(ee, CountDown(8)); // Add countdown component
								RemoveEntity(world, entity);
							}
								
						});
					break; // Exit the loop after finding a player
				}
			}
		});
}

void InteractionSystems::CheckFinishPoint(World& world)
{
	auto& map = world.GetResourse<MapInfo>().map;
	world.View<Position, FinishPoint>().ForEach([&world, &map](Entity entity, Position& pos, FinishPoint& fp)
		{
			//get all entities at the position and check if any player or player replay is present
			for (Entity e : map[pos.x][pos.y])
			{
				if ((world.Has<Player>(e) && !world.Has<Ghost>(e)) || (world.Has<PlayerReplay>(e) && !world.Has<Ghost>(e)))
				{
					if (world.GetResourse<SceneData>().currentLevel == ENDING_LEVEL)
					{
						if (!world.Has<PlayerReplay>(e) || world.Has<Ghost>(e) || world.Get<PlayerReplay>(e).round != -1)
							continue;
					}
					world.View<Sprite>().ForEach([&world, e](Entity ee, Sprite& sprite)
						{
							world.DeleteEntity(ee);
						});
					world.View<Tilemap>().ForEach([&world](Entity entity, Tilemap& tilemap)
						{
							world.DeleteEntity(entity); // Remove the tilemap entity
						});
					world.View<TilemapElement>().ForEach([&world](Entity entity, TilemapElement& tile)
						{
							// Remove all tilemap elements
							world.DeleteEntity(entity);
						});
					PlayerControlSystem::ClearHistory(); // Clear player control history
					// Player or player replay is present, remove finish point
					LevelManager::LoadLevel(world, MAIN_MENU);
					break; // Exit the loop after finding a player
				}
			}
		});
}

void InteractionSystems::CheckLevelEntry(World& world)
{
	auto& map = world.GetResourse<MapInfo>().map;
	world.View<Position, LevelEntry>().ForEach([&world, &map](Entity entity, Position& pos, LevelEntry& fp)
		{
			//get all entities at the position and check if any player or player replay is present
			for (Entity e : map[pos.x][pos.y])
			{
				if ((world.Has<Player>(e) && !world.Has<Ghost>(e)))
				{
					Levels l = fp.level;
					world.View<Sprite>().ForEach([&world, e](Entity ee, Sprite& sprite)
						{
							world.DeleteEntity(ee);
						});
					world.View<Tilemap>().ForEach([&world](Entity entity, Tilemap& tilemap)
					{
							world.DeleteEntity(entity); // Remove the tilemap entity
						});
					world.View<TilemapElement>().ForEach([&world](Entity entity, TilemapElement& tile)
					{
						// Remove all tilemap elements
						world.DeleteEntity(entity);
						});
					// Player or player replay is present, remove finish point
					PlayerControlSystem::ClearHistory(); // Clear player control history
					LevelManager::LoadLevel(world, l);
					break; // Exit the loop after finding a player
				}
			}
		});
}

