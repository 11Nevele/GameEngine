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
#include "GameComponents/StorySystem.h"

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

	// 通知渲染器调整视口大小
	mRenderer& renderer = world.GetResourse<mRenderer>();
	renderer.OnWindowResize(event.width, event.height);

	// 如果您的游戏逻辑需要知道窗口大小，可以在这里更新

	return true; // 返回 true 表示事件已被处理
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
	world.RegisterType<FinishPoint>();
	world.RegisterType<LevelEntry>();

	//register all resources
	world.AddResource<SceneData>(new SceneData);
	world.AddResource<MapInfo>(new MapInfo);

	//register all all systems
	world.AddUpdateSystem(PlayerControlSystem::PlayerControl, 3);
	world.AddUpdateSystem(UpdateSprite::Update, 3);
	world.AddUpdateSystem(PlayerControlSystem::AnimationSystem, 4);
	world.AddUpdateSystem(PlayerControlSystem::NewTurnSystem, 3);
	world.AddUpdateSystem(InteractionSystems::CheckSpike, 0);
	world.AddUpdateSystem(InteractionSystems::CheckButton, 0);
	world.AddUpdateSystem(InteractionSystems::CountDownSystem, 1);
	world.AddUpdateSystem(InteractionSystems::CheckHealthKit, 2);
	world.AddUpdateSystem(PlayerControlSystem::ResetSystem, 5);
	world.AddUpdateSystem(PlayerControlSystem::UndoSystem, 5);
	world.AddUpdateSystem(InteractionSystems::CheckFinishPoint, 0);
	world.AddUpdateSystem(InteractionSystems::CheckLevelEntry, 0);
	world.AddPostUpdateSystem(InteractionSystems::RenderText, 9);
	world.AddUpdateSystem(StorySystem::StoryUpdateSystem, 0);


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
		.AddTexture("ButtonPressed", path + "ButtonPressed.png")
		.AddTexture("HealthKit", path + "Healthkit.png")
		.AddTexture("FinishPoint", path + "Ground/ground_02.png")

		.AddTexture("ButtonUnpressedR", path + "ButtonUnpressedRed.png")
		.AddTexture("ButtonUnpressedB", path + "ButtonUnpressedBlue.png")
		.AddTexture("ButtonUnpressedY", path + "ButtonUnpressedYellow.png")
		.AddTexture("DoorOpennedR", path + "DoorOpennedRed.png")
		.AddTexture("DoorOpennedB", path + "DoorOpennedBlue.png")
		.AddTexture("DoorOpennedY", path + "DoorOpennedYellow.png")
		.AddTexture("DoorClosedR", path + "DoorClosedRed.png")
		.AddTexture("DoorClosedB", path + "DoorClosedBlue.png")
		.AddTexture("DoorClosedY", path + "DoorClosedYellow.png")
		
		.AddTexture("Story1", path + "Story1.png")
		.AddTexture("Story2", path + "Story2.png")
		.AddTexture("Story3", path + "Story3.png")
		.AddTexture("Story4", path + "Story4.png")
		.AddTexture("Story5", path + "Story5.png")
		.AddTexture("Story6", path + "Story6.png")
		.AddTexture("Story7", path + "Story7.png")
		.AddTexture("Story8", path + "Story8.png")
		;



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

	/*Entity sprite = world.CreateEntity();
	world.Add<Sprite>(sprite, Sprite::Create("Story1", world.GetResourse<TextureManager>()));
	world.Add<Transform>(sprite, Transform({0,0,-0.6}));
	Entity text = world.CreateEntity();
	world.Add<Text>(text, Text("Press Space to continue", 36, glm::vec2(0.5f, 0), glm::vec3(1, 1, 1)));
	world.Add<Transform>(text, Transform(glm::vec3(480, 0, -0.7f)));
	StorySystem::Init(sprite, text);
	*/
	LevelManager::LoadLevel(world, MAIN_MENU);

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