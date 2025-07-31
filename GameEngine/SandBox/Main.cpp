#include "acpch.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glfw3.h>
#include "Achoium.h"
#include <fstream>
#include "GameComponents/Components.h"
#include "GameComponents/LevelManager.h"
#include "GameComponents/PlayerControlSystem.h"
#include "GameComponents/UpdateSprite.h"
#include "GameComponents/InteractionSystems.h"

#include "UnitTests/TestUnit.h"
using namespace ac;


using namespace std;
using namespace ac;

using mTexture2D = OpenGLTexture2D;
using mInput = WindowsInput;
using mWindow = WinWindow;
using mRenderer = OpenGLRenderer;



bool exitgame = false;

bool HandelGameExit(const WindowCloseEvent& event)
{
	exitgame = true;
	ACMSG("Game exit requested.");
	return true;
}
ac::World world;
bool HandleWindowResize(const WindowResizeEvent& event)
{
	ACMSG("Window resized: " << event.width << " x " << event.height);

	// ֪ͨ��Ⱦ�������ӿڴ�С
	mRenderer& renderer = world.GetResourse<mRenderer>();
	renderer.OnWindowResize(event.width, event.height);

	// ���������Ϸ�߼���Ҫ֪�����ڴ�С���������������

	return true; // ���� true ��ʾ�¼��ѱ�����
}

void InitGame()
{
	//register all components
	world.RegisterType<Position>();
	world.RegisterType<Player>();
	world.RegisterType<CountDown>();
	world.RegisterType<Ghost>();
	world.RegisterType<PlayerReplay>();
	world.RegisterType<Button>();
	world.RegisterType<Door>();
	world.RegisterType< Coorpse>();
	world.RegisterType<Wall>();
	world.RegisterType<Spike>();
	world.RegisterType<HealthKit>();

	//register all resources
	world.AddResource<SceneData>(new SceneData);
	world.AddResource<MapInfo>(new MapInfo);

	//register all all systems
	world.AddUpdateSystem(PlayerControlSystem::PlayerControl, 0);
	world.AddUpdateSystem(UpdateSprite::Update, 2);
	world.AddUpdateSystem(PlayerControlSystem::AnimationSystem, 1);
	world.AddUpdateSystem(PlayerControlSystem::NewTurnSystem, 3);
	world.AddUpdateSystem(InteractionSystems::CheckSpike, 0);
	world.AddUpdateSystem(InteractionSystems::CheckButton, 0);
	world.AddUpdateSystem(InteractionSystems::CountDownSystem, 0);
	world.AddUpdateSystem(InteractionSystems::CheckHealthKit, 0);

	world.AddPostUpdateSystem(InteractionSystems::RenderText, 9);

	//register pictures
	string path = CURPATH + "/Assets/Image/";
	world.GetResourse<TextureManager>()
		.AddTexture("Spike", CURPATH + "/Assets/Image/Spike.png")
		.AddTexture("Ground", path + "Ground/ground_01.png")
		.AddTexture("Wall", path + "Blocks/block_06.png")
		.AddTexture("PlayerUp", path + "Player/player_02.png")
		.AddTexture("PlayerDown", path + "Player/player_23.png")
		.AddTexture("PlayerLeft", path + "Player/player_14.png")
		.AddTexture("PlayerRight", path + "Player/player_11.png")
		.AddTexture("GhostUp", path + "Player/ghostup.png")
		.AddTexture("GhostDown", path + "Player/ghostdown.png")
		.AddTexture("GhostLeft", path + "Player/ghostleft.png")
		.AddTexture("GhostRight", path + "Player/ghostright.png")
		.AddTexture("Coorpse1", path + "Player/player_dead.png")
		.AddTexture("DoorOpenned", path + "DoorOpenned.png")
		.AddTexture("DoorClosed", path + "DoorClosed.png")
		.AddTexture("ButtonPressed", path + "ButtonPressed.png")
		.AddTexture("ButtonUnpressed", path + "ButtonUnpressed.png")
		.AddTexture("HealthKit", path + "Healthkit.png")
		.AddTexture("FinishPoint", path + "Ground/ground_02.png");



	//register all events
	world.GetResourse<EventManager>()
		.AddListener<WindowCloseEvent>(HandelGameExit)
		.AddListener<WindowResizeEvent>(HandleWindowResize);



}


int main()
{
	

	srand(time(0));
	InitEngine(world);
	
	Entity camera = world.CreateEntity();


	world.Add<Camera>(camera, Camera{ });
	world.Add<AudioListener>(camera, AudioListener());
	world.Add<Transform>(camera, Transform());
	world.Get<Transform>(camera).position = { 0, 0, 0 };
	
	InitGame();

	LevelManager::LoadLevel(world, TEST_LEVEL);

	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();
		
		mRenderer& renderer = world.GetResourse<mRenderer>();

		world.Update();
		if (exitgame)
			break;

		win.OnUpdate();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}