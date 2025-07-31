#include "acpch.h"
#include "PlayerControlSystem.h"
#include "Components.h"
#include "LevelManager.h"
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
	bool actionPerformed = MovementSystem(world) || SuicideSystem(world)|| HealSystem(world);
	if (!actionPerformed)
		return;
	world.View<PlayerReplay, Position>().ForEach([&world](Entity entity, PlayerReplay& playerReplay, Position& position)
	{
		if (playerReplay.curInd < playerReplay.directions.size())
		{
			position.dx = playerReplay.directions[playerReplay.curInd].dx;
			position.dy = playerReplay.directions[playerReplay.curInd].dy;
			playerReplay.curInd++;
		}

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
	isInAnimation = true; // ��ʼ����
}
void ClearAll(World& world)
{
	world.View<Position>().ForEach([&world](Entity entity, Position& position)
	{
			world.DeleteEntity(entity);
		});

}
void PlayerControlSystem::ResetSystem(World& world)
{

}

void PlayerControlSystem::NewTurnSystem(World& world)
{
	if(isInAnimation)
		return;
	if (!world.GetResourse<InputManager>().IsKeyDown(AC_KEY_E))
		return;
	world.View<Player>().ForEach([&world](Entity entity, Player& player)
		{
			Entity e = mCreate<PlayerReplay>(world, world.GetResourse<SceneData>().startX, world.GetResourse<SceneData>().startY, "PlayerDown"); // �����ط�ʵ��
			world.Add<CountDown>(e, CountDown(8)); // ��ӵ���ʱ���
			world.Get<PlayerReplay>(e).directions = player.directions; // ������ҵķ�������
			RemoveEntity(world, entity); // ɾ�����ʵ��
		});
	world.View<Coorpse>().ForEach([&world](Entity entity, Coorpse& position)
		{
			RemoveEntity(world, entity); // ɾ��ʬ��ʵ��
		});
	world.View<Ghost>().ForEach([&world](Entity entity, Ghost& g)
		{
			world.Delete<Ghost>(entity); // ɾ�����ʵ��
		});
	world.View<PlayerReplay, CountDown, Position>().ForEach([&world](Entity entity, PlayerReplay& replay, CountDown& cd, Position& pos)
		{
			replay.curInd = 0; // ���ûط�λ��
			MoveEntity(world, entity, pos.x, pos.y, world.GetResourse<SceneData>().startX, world.GetResourse<SceneData>().startY);
			world.Get<Sprite>(entity).textureID = world.GetResourse<TextureManager>().GetTextureID("PlayerDown");
			cd.remain = 8; // ���õ���ʱ
		});
	LevelManager::LoadLevel(world, (Levels)world.GetResourse<SceneData>().currentLevel, false);
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
			playerControl.directions.push_back({ -1, 0 });
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_D))
		{
			position.dx = 1;
			position.dy = 0;
			playerControl.directions.push_back({ 1, 0 });
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_S))
		{
			position.dx = 0;
			position.dy = -1;
			playerControl.directions.push_back({ 0, -1 });
			hasMovement = true;
		}
		else if (inputManager.IsKeyPressed(AC_KEY_W))
		{
			position.dx = 0;
			position.dy = 1;
			playerControl.directions.push_back({ 0, 1 });
			hasMovement = true;
		}
		else if (inputManager.IsKeyDown(AC_KEY_SPACE))
		{
			// ������¿ո��������ƶ���ͼ
			position.dx = 0;
			position.dy = 0;
			playerControl.directions.push_back({ 0, 0 });
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
		return 0; // ����0��ʾû�пռ���ƶ�
	}
	Entity environmentTile = environment.map[tx][ty];

		// �����ǽ�ڣ����޷��ƶ�
	if (world.Has<Wall>(environmentTile))
	{
		return 0; // ����0��ʾû�пռ���ƶ�
	}
	//check if there is a closed gate
	for(Entity entity : map[tx][ty])
	{
		if (world.Has<Door>(entity) && !world.Get<Door>(entity).isOpen)
		{
			return 0; // ����0��ʾû�пռ���ƶ�
		}
	}

	bool hasSpace = true;
	if (!world.Has<Ghost>(e))
	{
		// ����ͼ�ϸ�λ���Ƿ���ʵ��
		for (const auto& entityVec : map[tx][ty])
		{
			Entity entity = entityVec;
			// ����Ƿ���Player��PlayerReplayʵ��
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
					hasSpace &= RecursivePush(world, entity, blockingEntityPos) == 1; // �ݹ��ƶ�ʵ��
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
    // �������о���Position�����Player��PlayerReplayʵ��
    world.View<Position>().ForEach([&world](Entity entity, Position& position) 
    {
        // ���ʵ���Ƿ���Player��PlayerReplay
        if ((world.Has<Player>(entity) || world.Has<PlayerReplay>(entity)) && 
            (position.dx != 0 || position.dy != 0)) // ֻ�������ƶ������ʵ��
        {
            // �����ƶ�ʵ�壬�������ܵĵݹ��ƶ�
            int canMove = RecursivePush(world, entity, position);
            
            if (canMove == 1)
            {

            }
            else
            {
                // ��������ƶ�������ƶ�����
                position.dx = 0;
                position.dy = 0;
            }
        }
    });
	auto& mapInfo = world.GetResourse<MapInfo>().map;
    // ����ƶ���ʵ�ʸ�������ʵ���λ��
    world.View<Position>().ForEach([&mapInfo](Entity entity, Position& position) 
    {
        
        
        // ����ƶ�����Ϊ��һ����׼��
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
		animationTime = 0.0f; // ���ö���ʱ��
		isInAnimation = false; // ��������
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
					if(playerReplay.directions[playerReplay.curInd -1] != direction{ 0,0 })
					{
						countDown.remain--;
					}
				}
		});
	}


	
}
