#include "acpch.h"
#include "PlayerControlSystem.h"
#include "Components.h"
std::vector<PlayerControlSystem::step> PlayerControlSystem::history;
bool PlayerControlSystem::isInAnimation = false;
float PlayerControlSystem::animationTime = 0.0f;

void PlayerControlSystem::UndoSystem(World& world)
{
	if (isInAnimation)
		return;
}


void PlayerControlSystem::PlayerControl(World& world)
{
	if (isInAnimation)
		return;
	bool actionPerformed = false;
	actionPerformed |= MovementSystem(world);
	actionPerformed |= SuicideSystem(world);
	actionPerformed |= HealSystem(world);
	if (!actionPerformed)
		return;
	NextStep(world);
	isInAnimation = true; // 开始动画
}

bool PlayerControlSystem::MovementSystem(World& world)
{
	InputManager& inputManager = world.GetResourse<InputManager>();
	bool hasMovement = false;
	world.View<Player, Position>().ForEach([&inputManager, &hasMovement](Entity entity, Player& playerControl, Position& position)
	{
		if (inputManager.IsKeyPressed(AC_KEY_A))
		{
			position.dx = -1;
			position.dy = 0;
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_D))
		{
			position.dx = 1;
			position.dy = 0;
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_S))
		{
			position.dx = 0;
			position.dy = -1;
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_W))
		{
			position.dx = 0;
			position.dy = 1;
			hasMovement = true;
		}
		else if (inputManager.IsKeyDown(AC_KEY_SPACE))
		{
			// 如果按下空格键，清除移动意图
			position.dx = 0;
			position.dy = 0;
			hasMovement = true;
		}
	});
	return hasMovement;
}

bool PlayerControlSystem::SuicideSystem(World& world)
{
	//implement later
	return false;
}

bool PlayerControlSystem::HealSystem(World& world)
{
	//implement later
	return false;
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

	bool hasSpace = true;
	// 检查地图上该位置是否有实体
	for (const auto& entityVec : map[tx][ty])
	{
		Entity entity = entityVec;
		// 检查是否是Player或PlayerReplay实体
		if ((world.Has<Player>(entity) || world.Has<PlayerReplay>(entity)) &&
			world.Has<Position>(entity))
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
        if (position.targetX != position.x || position.targetY != position.y)
        {
			mapInfo[position.x][position.y].erase(
				std::find(mapInfo[position.x][position.y].begin(),
					mapInfo[position.x][position.y].end(),
					entity));
			mapInfo[position.targetX][position.targetY].push_back(entity);
        }
        
        // 清除移动意向，为下一步做准备
        position.dx = 0;
        position.dy = 0;
    });
}

void PlayerControlSystem::AnimationSystem(World& world)
{
	if(!isInAnimation)
		return;
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
		animationTime = 0.0f; // 重置动画时间
		isInAnimation = false; // 动画结束
		world.View<Position, Transform>().ForEach([&world, &time](Entity entity, Position& position, Transform& transform)
			{
				position.x = position.targetX;
				position.y = position.targetY;
				transform.position.x = position.x * world.GetResourse<SceneData>().gridWidth;
				transform.position.y = position.y * world.GetResourse<SceneData>().gridHeight;
			});
	}


	
}
