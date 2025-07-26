#include "acpch.h"
//#define ECS_INFO_ENABLED
#define SHOW_WARNING
//#define SHOW_MESSAGE
#define SHOW_ERROR


#include <gl/GL.h>
#include <gl/GLU.h>
#include <glfw3.h>
#include "Achoium.h"
#include <fstream>

#include "GameComponents/GameComponents.h"
#include "Render/RenderTileSystems.h"
#include "GamePlaySystems/LevelManagementSystems.h"
#include "GamePlaySystems/GameplayerSystems.h"
#include "UnitTests/TestUnit.h"
#include "Render/UISystems.h"
#include"GamePlaySystems//SceneFinishChecks.h"
using namespace ac;


using namespace std;
using namespace ac;

using mTexture2D = OpenGLTexture2D;
using mInput = WindowsInput;
using mWindow = WinWindow;
using mRenderer = OpenGLRenderer;


struct Camera {
};
void CameraMovement(ac::World& world)
{

	mInput& input = world.GetResourse<mInput>();
	mWindow& win = world.GetResourse<mWindow>();
	mRenderer& rend = world.GetResourse<mRenderer>();
	Time& time = world.GetResourse<Time>();
	auto t = world.View<Camera, Transform>().GetPacked();
	for (auto i : t)
	{
		auto& transform = get<1>(i.components);
		rend.UpdateCamera(transform.asMat4(true));
	}
}
void LoadAssets(World& world)
{
	std::string curPath = filesystem::current_path().string() + "/Assets/Image/";
	world.GetResourse<TextureManager>().AddTexture("White", curPath + "White.png")
		.AddTexture("PlayerDown", curPath + "Player/player_23.png")
		.AddTexture("PlayerUp", curPath + "Player/player_02.png")
		.AddTexture("PlayerLeft", curPath + "Player/player_14.png")
		.AddTexture("PlayerRight", curPath + "Player/player_11.png")
		.AddTexture("Ground1", curPath + "Ground/ground_01.png")
		.AddTexture("Ground2", curPath + "Ground/ground_02.png")
		.AddTexture("Wall1", curPath + "Blocks/block_06.png")
		.AddTexture("Box1", curPath + "Crates/crate_07.png")
		.AddTexture("Box2", curPath + "Crates/crate_09.png");

	world.GetResourse<AudioManager>().RegisterAudio("Test", CURPATH + "/Assets/Audio/test.wav");
}

void RegisterComponentsAndSystems(World& world)
{
	world.RegisterType<Wall>();
	world.RegisterType<Player>();
	world.RegisterType<Number>();
	world.RegisterType<WinCondition>();
	world.RegisterType<Box>();
	world.RegisterType<LevelEntrance>();

	world.AddUpdateSystem(GameplayerSystems::UndoLastStep, 0);
	world.AddUpdateSystem(SceneFinishChecks::CheckWinLevel, 0);
	world.AddUpdateSystem(SceneFinishChecks::CheckExitLevel, 0);
	world.AddUpdateSystem(SceneFinishChecks::CheckRestartLevel, 0);
	world.AddUpdateSystem(SceneFinishChecks::CheckTransitionLevel, 0);

	world.AddUpdateSystem(GameplayerSystems::PlayerMovement, 1);
	world.AddPostUpdateSystem(RenderTileSystems::UpdateNumber, 9);
	world.AddPostUpdateSystem(RenderTileSystems::UpdateWinCondition, 7);
	world.AddPostUpdateSystem(UISystems::UpdateUI, 9);
}

void TestAudioSystem(World& world)
{
	InputManager& input = world.GetResourse<InputManager>();
	if (input.IsKeyDown(AC_KEY_SPACE))
	{
		world.View<Sprite, AudioSource>().ForEach([&world](Entity entity, Sprite& sprite, AudioSource& audioSource)
		{
			if (audioSource.audioID != INVALID_AUDIO_ID)
			{
				AudioManager& audioManager = world.GetResourse<AudioManager>();
				AudioClip* clip = audioManager.GetAudioClip(audioSource.audioID);
				if (clip && clip->loaded)
				{
					audioManager.Play(audioSource.audioID, audioSource.loop, audioSource.volume);
					ACMSG("Playing audio: " << clip->name);
				}
				else
				{
					ACMSG("Audio clip not loaded or invalid ID: " << audioSource.audioID);
				}
			}
			else
			{
				ACMSG("Invalid audio ID for entity: " << entity);
			}
			});
	}
}

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

int main()
{
	srand(time(0));
	
	
	InitEngine(world);
	
	Entity camera = world.CreateEntity();
	world.Add<Transform>(camera, Transform());
	world.Add<Camera>(camera, Camera());
	world.AddUpdateSystem(CameraMovement, 0);
	world.GetResourse<EventManager>().AddListener<WindowCloseEvent>(HandelGameExit);
	world.GetResourse<EventManager>().AddListener<WindowResizeEvent>(HandleWindowResize);


	LoadAssets(world);
	RegisterComponentsAndSystems(world);

	world.AddResource<SceneData>(new SceneData(100, 64, 64));

	world.AddUpdateSystem(TestAudioSystem, 0);
	
	
	RenderTileSystems::Init(world);
	world.AddUpdateSystem(RenderTileSystems::UpdateTileSprites, 9);

	LevelManagementSystems::LoadLevel(world, Levels::MAIN_MENU);
	
	InputManager& input = world.GetResourse<InputManager>();
	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();
		
		mRenderer& renderer = world.GetResourse<mRenderer>();
		Transform t;
		t.scale = { 2, 1, 1 };
		//renderer.SubmitText("Hello World", t);
		world.Update();
		win.OnUpdate();
		if (exitgame)
		{
			ACMSG("Exiting game loop.");
			break;
		}
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}