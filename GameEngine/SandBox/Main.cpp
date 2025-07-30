#include "acpch.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glfw3.h>
#include "Achoium.h"
#include <fstream>
#include "GameComponents/Components.h"
#include "GameComponents/LevelManager.h"

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
	world.RegisterType<PlayerControl>();
	world.RegisterType<CountDown>();
	world.RegisterType<Ghost>();
	world.RegisterType<PlayerReplay>();
	world.RegisterType<Button>();
	world.RegisterType<Door>();
	world.RegisterType< Coorpse>();

	//register all resources
	world.AddResource<SceneData>(new SceneData);

	//register all all systems

	//register pictures
	string path = CURPATH + "/Assets/Image/";
	world.GetResourse<TextureManager>()
		.AddTexture("Spike", CURPATH + "/Assets/Image/Spike.png")
		.AddTexture("Ground", path + "Ground/ground_01.png")
		.AddTexture("Wall", path + "Blocks/block_06.png");



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