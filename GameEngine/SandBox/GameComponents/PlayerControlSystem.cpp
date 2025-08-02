#include "acpch.h"
#include "PlayerControlSystem.h"
#include "Components.h"
#include "LevelManager.h"
std::vector<PlayerControlSystem::step> PlayerControlSystem::history;
bool PlayerControlSystem::isInAnimation = false;
float PlayerControlSystem::animationTime = 0.0f;

void PlayerControlSystem::UndoSystem(World& world)
{
	if (isInAnimation || world.GetResourse<SceneData>().currentLevel == BEGINNING_LEVEL)
		return;
	if(history.size() <= 1)
	{
		return;
	}
	if (!world.GetResourse<InputManager>().IsKeyDown(AC_KEY_Z))
		return;
	history.pop_back(); // 删除当前步骤
	step currentStep = history.back(); // 获取上一个步骤
	world.GetResourse<SceneData>().currentRound = currentStep.currentRound;
	world.GetResourse<SceneData>().currentStep = currentStep.currentStep;
	world.View<Coorpse>().ForEach([&world](Entity entity, Coorpse& coorpse)
	{
		RemoveEntity(world, entity);
		});
	for(auto& pos : currentStep.coorpses)
	{
		Entity corpseEntity = mCreate<Coorpse>(world, pos.x, pos.y, "Coorpse1");
	}
	world.View<Player>().ForEach([&world](Entity entity, Player& player)
	{
			RemoveEntity(world, entity);
		});
	world.View<PlayerReplay>().ForEach([&world](Entity entity, PlayerReplay& playerReplay)
		{
		RemoveEntity(world, entity);
		});
	for(auto& state: currentStep.entityStates)
	{
		if(state.hasPlayer)
		{
			Entity playerEntity = mCreate<Player>(world, state.position.x, state.position.y, "PlayerDown");
			world.Get<Player>(playerEntity) = state.player;
			world.Get<Sprite>(playerEntity).textureID = state.spriteTextureID;
			if(state.hasGhost)
			{
				world.Add<Ghost>(playerEntity, Ghost());
			}
			if(state.hasCountDown)
			{
				world.Add<CountDown>(playerEntity, CountDown{ state.countdown });
			}
		}
		else if(state.hasPlayerReplay)
		{
			Entity playerReplayEntity = mCreate<PlayerReplay>(world, state.position.x, state.position.y, "PlayerDown");
			world.Get<PlayerReplay>(playerReplayEntity) = state.playerReplay;
			world.Get<Sprite>(playerReplayEntity).textureID = state.spriteTextureID;
			if(state.hasGhost)
			{
				world.Add<Ghost>(playerReplayEntity, Ghost());
			}
			if (state.hasCountDown)
			{
				world.Add<CountDown>(playerReplayEntity, CountDown{ state.countdown });
			}
		}
	}
	world.View<HealthKit>().ForEach([&world](Entity entity, HealthKit& healthKit)
	{
		RemoveEntity(world, entity);
		});
	for(auto& pos : currentStep.healthKits)
	{
		mCreate<HealthKit>(world, pos.x, pos.y, "HealthKit");
	}

}

// 保存当前世界状态到历史记录
void PlayerControlSystem::SaveState(World& world)
{
	step currentStep;

	// 保存场景数据
	SceneData& sceneData = world.GetResourse<SceneData>();
	currentStep.currentRound = sceneData.currentRound;
	currentStep.currentStep = sceneData.currentStep;

	// 保存所有实体状态
	world.View<Position, Sprite>().ForEach([&world, &currentStep](Entity entity, Position& position, Sprite& sprite)
		{
			step::EntityState entityState;
			entityState.position = position;
			entityState.spriteTextureID = sprite.textureID;

			// 保存Player组件（如果存在）
			entityState.hasPlayer = world.Has<Player>(entity);
			if (entityState.hasPlayer)
			{
				entityState.player = world.Get<Player>(entity);
			}

			// 保存PlayerReplay组件（如果存在）
			entityState.hasPlayerReplay = world.Has<PlayerReplay>(entity);
			if (entityState.hasPlayerReplay)
			{
				entityState.playerReplay = world.Get<PlayerReplay>(entity);
			}

			// 保存Ghost组件状态
			entityState.hasGhost = world.Has<Ghost>(entity);

			// 保存CountDown组件状态
			entityState.hasCountDown = world.Has<CountDown>(entity);
			if (entityState.hasCountDown)
			{
				entityState.countdown = world.Get<CountDown>(entity);
			}

			currentStep.entityStates.push_back(entityState);
		});


	// 保存尸体实体
	world.View<Coorpse, Position>().ForEach([&currentStep](Entity entity, Coorpse& coorpse, Position& pos)
		{
			currentStep.coorpses.push_back(pos);
		});

	// 保存健康包实体
	world.View<HealthKit, Position>().ForEach([&currentStep](Entity entity, HealthKit& healthKit, Position& pos)
		{
			currentStep.healthKits.push_back(pos);
		});

	// 将当前步骤添加到历史记录中
	history.push_back(currentStep);
}

void PlayerControlSystem::PlayerControl(World& world)
{
    if (isInAnimation)
        return;
    
    // 在进行任何操作前保存当前状态
    
    
    bool actionPerformed = MovementSystem(world);
    if (!actionPerformed)
    {
        return;
    }
	world.GetResourse<SceneData>().currentStep++;
    world.View<PlayerReplay, Position>().ForEach([&world](Entity entity, PlayerReplay& playerReplay, Position& position)
    {
        if (playerReplay.curInd < playerReplay.directions.size())
        {
            position.dx = playerReplay.directions[playerReplay.curInd].dx;
            position.dy = playerReplay.directions[playerReplay.curInd].dy;
            
        }
        playerReplay.curInd++;
        });
    //update sprite
    TextureManager& textureManager = world.GetResourse<TextureManager>();
    world.View<Sprite, Position, Player>().ForEach([&world, &textureManager](Entity entity, Sprite& sprite, Position& pos, Player& player)
    {
                string textureType = "Player";
                if (world.Has<Ghost>(entity))
                {
                    textureType = "Ghost";
                }
                if (pos.dx == 1 && pos.dy == 0)
                {
                    sprite.textureID = textureManager.GetTextureID(textureType + "Right");
                }
                else if (pos.dx == -1 && pos.dy == 0)
                {
                    sprite.textureID = textureManager.GetTextureID(textureType + "Left");
                }
                else if (pos.dx == 0 && pos.dy == 1)
                {
                    sprite.textureID = textureManager.GetTextureID(textureType + "Up");
                }
                else if (pos.dx == 0 && pos.dy == -1)
                {
                    sprite.textureID = textureManager.GetTextureID(textureType + "Down");
                }
    });
    world.View<Sprite, Position, PlayerReplay>().ForEach([&world, &textureManager](Entity entity, Sprite& sprite, Position& pos, PlayerReplay& player)
        {
            string textureType = "Player";
            if (world.Has<Ghost>(entity))
            {
                textureType = "Ghost";
            }
            if (pos.dx == 1 && pos.dy == 0)
            {
                sprite.textureID = textureManager.GetTextureID(textureType + "Right");
            }
            else if (pos.dx == -1 && pos.dy == 0)
            {
                sprite.textureID = textureManager.GetTextureID(textureType + "Left");
            }
            else if (pos.dx == 0 && pos.dy == 1)
            {
                sprite.textureID = textureManager.GetTextureID(textureType + "Up");
            }
            else if (pos.dx == 0 && pos.dy == -1)
            {
                sprite.textureID = textureManager.GetTextureID(textureType + "Down");
            }
        });
    NextStep(world);
    isInAnimation = true; // 开始动画
}

void PlayerControlSystem::ResetSystem(World& world)
{
    if (isInAnimation || world.GetResourse<SceneData>().currentLevel == BEGINNING_LEVEL)
        return;
    if(!world.GetResourse<InputManager>().IsKeyPressed(AC_KEY_R))
		return;
    // 重置玩家位置和状态
    world.View< Position>().ForEach([&world](Entity entity, Position& pos)
    {
			RemoveEntity(world, entity); // 删除实体
    });
    
    world.View<TilemapElement>().ForEach([&world](Entity entity, TilemapElement& tile)
	{
		// 删除所有地图元素
			world.DeleteEntity(entity);
		});
	world.View<Sprite>().ForEach([&world](Entity entity, Sprite& tile)
		{
			// 删除所有地图元素
			world.DeleteEntity(entity);
		});
    
    // 重新加载当前关卡（但不重新创建地图）
    LevelManager::LoadLevel(world, (Levels)world.GetResourse<SceneData>().currentLevel, true);
}

void PlayerControlSystem::NewTurnSystem(World& world)
{
	if(isInAnimation || world.GetResourse<SceneData>().currentRound >=7 || world.GetResourse<SceneData>().currentLevel == MAIN_MENU
		|| world.GetResourse<SceneData>().currentLevel == BEGINNING_LEVEL)
		return;
	if (!world.GetResourse<InputManager>().IsKeyDown(AC_KEY_E))
		return;
	world.View<Player>().ForEach([&world](Entity entity, Player& player)
		{
			Entity e = mCreate<PlayerReplay>(world, world.GetResourse<SceneData>().startX, world.GetResourse<SceneData>().startY, "PlayerDown"); // 创建回放实体
			world.Add<CountDown>(e, CountDown(8)); // 添加倒计时组件
			world.Get<PlayerReplay>(e).directions = player.directions; // 复制玩家的方向向量
			world.Get<PlayerReplay>(e).round = player.round; // 复制玩家的回合数
			RemoveEntity(world, entity); // 删除玩家实体
		});
	world.View<Coorpse>().ForEach([&world](Entity entity, Coorpse& position)
		{
			RemoveEntity(world, entity); // 删除尸体实体
		});
	world.View<Ghost>().ForEach([&world](Entity entity, Ghost& g)
		{
			world.Delete<Ghost>(entity); // 删除鬼魂实体
		});
	world.View<FinishPoint>().ForEach([&world](Entity entity, FinishPoint& fp)
		{
			RemoveEntity(world, entity); // 删除终点实体
		});
	world.View<HealthKit>().ForEach([&world](Entity entity, HealthKit& hk)
		{
			RemoveEntity(world, entity); // 删除健康包实体
		});
	world.View<PlayerReplay, Position>().ForEach([&world](Entity entity, PlayerReplay& replay, Position& pos)
		{
			
			replay.curInd = 0; // 重置回放位置
			if (replay.round == -1)
			{
				MoveEntity(world, entity, pos.x, pos.y, 7, 2);
				world.Get<Sprite>(entity).textureID = world.GetResourse<TextureManager>().GetTextureID("PlayerUp");
			}
			else
			{
				MoveEntity(world, entity, pos.x, pos.y, world.GetResourse<SceneData>().startX, world.GetResourse<SceneData>().startY);
				world.Get<Sprite>(entity).textureID = world.GetResourse<TextureManager>().GetTextureID("PlayerDown");
			}
			
			if(world.Has<CountDown>(entity))
				{
				world.Get<CountDown>(entity).remain = 8; // 重置倒计时
			}
			else
			{
				world.Add<CountDown>(entity, CountDown(8)); // 添加倒计时组件
			}
		});
	world.GetResourse<SceneData>().currentRound++;
	world.GetResourse<SceneData>().currentStep = 0; // 重置当前步骤
	LevelManager::LoadLevel(world, (Levels)world.GetResourse<SceneData>().currentLevel, false);
}

bool PlayerControlSystem::MovementSystem(World& world)
{
	InputManager& inputManager = world.GetResourse<InputManager>();
	bool hasMovement = false;
	Levels currentLevel = (Levels)world.GetResourse<SceneData>().currentLevel;
	world.View<Player, Position>().ForEach([&inputManager, &hasMovement, currentLevel](Entity entity, Player& playerControl, Position& position)
	{
		if (inputManager.IsKeyPressed(AC_KEY_A) && currentLevel != BEGINNING_LEVEL)
		{
			position.dx = -1;
			position.dy = 0;
			playerControl.directions.push_back({ -1, 0 });
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_D) && currentLevel != BEGINNING_LEVEL)
		{
			position.dx = 1;
			position.dy = 0;
			playerControl.directions.push_back({ 1, 0 });
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_S) && currentLevel != BEGINNING_LEVEL)
		{
			position.dx = 0;
			position.dy = -1;
			playerControl.directions.push_back({ 0, -1 });
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_W) && currentLevel != BEGINNING_LEVEL)
		{
			position.dx = 0;
			position.dy = 1;
			playerControl.directions.push_back({ 0, 1 });
			hasMovement = true;
		}
		else if (inputManager.IsKeyDown(AC_KEY_SPACE))
		{
			// 如果按下空格键，清除移动意图
			position.dx = 0;
			position.dy = 0;
			playerControl.directions.push_back({ 0, 0 });
			hasMovement = true;
		}
	});
	return hasMovement;
}


//1 has space, 0 no space
int RecursivePush(World& world, Entity& e, Position& p)
{
	int tx = p.x + p.dx;
	int ty = p.y + p.dy;

	Tilemap& environment = world.Get<Tilemap>(world.GetResourse<MapInfo>().tilemap);
	auto& map = world.GetResourse<MapInfo>().map;
	if (tx < 0 || tx >= environment.map.size() || ty < 0 || ty >= environment.map[tx].size())
	{
		return 0; // 返回0表示没有空间可移动
	}
	Entity environmentTile = environment.map[tx][ty];

		// 如果是墙壁，则无法推动
	if (world.Has<Wall>(environmentTile))
	{
		return 0; // 返回0表示没有空间可移动
	}
	//check if there is a closed gate
	for(Entity entity : map[tx][ty])
	{
		if (world.Has<Door>(entity) && !world.Get<Door>(entity).isOpen)
		{
			return 0; // 返回0表示没有空间可移动
		}
	}

	bool hasSpace = true;
	if (!world.Has<Ghost>(e))
	{
		// 检查地图上该位置是否有实体
		for (const auto& entityVec : map[tx][ty])
		{
			Entity entity = entityVec;
			// 检查是否是Player或PlayerReplay实体
			if ((world.Has<Player>(entity) || world.Has<PlayerReplay>(entity)) &&
				world.Has<Position>(entity) && !world.Has<Ghost>(entity))
			{
				Position& blockingEntityPos = world.Get<Position>(entity);
				if (blockingEntityPos.dx != 0 || blockingEntityPos.dy != 0)
				{

				}
				else
				{
					blockingEntityPos.dx = p.dx;
					blockingEntityPos.dy = p.dy;
					hasSpace &= RecursivePush(world, entity, blockingEntityPos) == 1; // 递归推动实体
					blockingEntityPos.dx = 0;
					blockingEntityPos.dy = 0;
				}

			}
		}
	}
	
	if (hasSpace)
	{
		p.targetX = tx;
		p.targetY = ty;
		return 1;
	}
	return 0;
}

void PlayerControlSystem::NextStep(World& world)
{
    // 处理所有具有Position组件的Player和PlayerReplay实体
    world.View<Position>().ForEach([&world](Entity entity, Position& position) 
    {
        // 检查实体是否是Player或PlayerReplay
        if ((world.Has<Player>(entity) || world.Has<PlayerReplay>(entity)) && 
            (position.dx != 0 || position.dy != 0)) // 只处理有移动意向的实体
        {
            // 尝试移动实体，包括可能的递归推动
            int canMove = RecursivePush(world, entity, position);
            
            if (canMove == 1)
            {

            }
            else
            {
                // 如果不能移动，清除移动意向
                position.dx = 0;
                position.dy = 0;
            }
        }
    });
	auto& mapInfo = world.GetResourse<MapInfo>().map;
    // 完成移动后实际更新所有实体的位置
    world.View<Position>().ForEach([&mapInfo](Entity entity, Position& position) 
    {
        
        
        // 清除移动意向，为下一步做准备
        position.dx = 0;
        position.dy = 0;
    });
}

void PlayerControlSystem::AnimationSystem(World& world)
{
	if(!isInAnimation)
		return;
	if (animationTime - 0.2f >= 0)
	{
		SaveState(world); // 0表示移动操作
		animationTime = 0.0f; // 重置动画时间
		isInAnimation = false; // 动画结束
	}
	Time& time = world.GetResourse<Time>();
	world.View<Position,Transform>().ForEach([&world, &time](Entity entity, Position& position, Transform& transform)
	{
			//move in 0.2 seconds
			transform.position.x += (world.GetResourse<SceneData>().gridWidth / 0.2f * time.Delta()) * (position.targetX - position.x);
			transform.position.y += world.GetResourse<SceneData>().gridHeight / 0.2f * time.Delta() * (position.targetY - position.y);

		});
	
	animationTime += time.Delta();
	if (animationTime - 0.2f >= 0)
	{
		auto& mapInfo = world.GetResourse<MapInfo>().map;
		world.View<Position, Transform>().ForEach([&world, &mapInfo](Entity entity, Position& position, Transform& transform)
			{

				if (position.targetX != position.x || position.targetY != position.y)
				{
					mapInfo[position.x][position.y].erase(
						std::find(mapInfo[position.x][position.y].begin(),
							mapInfo[position.x][position.y].end(),
							entity));
					mapInfo[position.targetX][position.targetY].push_back(entity);
				}
				position.x = position.targetX;
				position.y = position.targetY;
				transform.position.x = position.x * world.GetResourse<SceneData>().gridWidth;
				transform.position.y = position.y * world.GetResourse<SceneData>().gridHeight;
			});
		world.View<CountDown>().ForEach([&world](Entity entity, CountDown& countDown)
		{
				if (world.Has<Player>(entity) && world.Get<Player>(entity).directions.back() != direction{0,0})
				{
					countDown.remain--;
				}
				else if (world.Has<PlayerReplay>(entity) )
				{
					PlayerReplay& playerReplay = world.Get<PlayerReplay>(entity);
					if(playerReplay.curInd - 1 < playerReplay.directions.size() && playerReplay.directions[playerReplay.curInd - 1] != direction{ 0,0 })
					{
						countDown.remain--;
					}
				}
		});
	}


	
}
