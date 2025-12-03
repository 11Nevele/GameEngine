#pragma once
#include "acpch.h"
#include "Achoium.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

	// Notify renderer to adjust viewport size
	mRenderer& renderer = world.GetResourse<mRenderer>();
	renderer.OnWindowResize(event.width, event.height);

	// If your game logic needs to know the window size, you can update it here

	return true; // Return true to indicate the event has been handled
}

void Movement(ac::World& world)
{

	
}


void LoadAssets(World& world)
{
	std::string curPath = filesystem::current_path().string();
	world.GetResourse<TextureManager>().AddTexture("White", curPath + "/Assets/Image/White.png");
}

void TestScene3D()
{


	srand(time(0));
	InitEngine(world);

	Entity camera = world.CreateEntity();
	world.Add<Camera>(camera, Camera{ });
	world.Add<Transform>(camera, Transform());
	world.Get<Transform>(camera).position = { 0, 0, 0};

	Entity object = world.CreateEntity();
	world.Add<Transform>(object, Transform(glm::vec3(0,-0,-200),0,1));

	// After creating the OpenGLModel, add validation
	OpenGLModel model(CURPATH + "/Assets/plane/Seahawk.obj");
	world.Add<OpenGLModel>(object, std::move(model));

	Entity e2 = world.CreateEntity("");
	world.Add<Sprite>(e2, Sprite::Create("Default", world.GetResourse<TextureManager>()));
	world.Add<Transform>(e2, Transform({ 0,0,-1000 }));


	

	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();

		mRenderer& renderer = world.GetResourse<mRenderer>();

		world.Get<Transform>(e2).RotateY(1);
		world.Get<Transform>(object).RotateY(1);
		//world.Get<Transform>(object).RotateX(0.7);
		world.Update();
		if (exitgame)
			break;

		win.OnUpdate();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Add this in your render loop for debugging
		
	}
}
