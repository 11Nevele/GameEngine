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

struct movementComponent
{
	float speed = 2.0f; // units per second
	float mouseSensitivity = 0.1f;
	float rotationSpeed = 90.0f; // degrees per second
};

void Movement(ac::World& world)
{
	float deltaTime = world.GetResourse<Time>().Delta();
	InputManager& input = world.GetResourse<InputManager>();
	world.View<movementComponent, Transform>().ForEach([&](Entity entity, movementComponent& moveComp, Transform& transform)
		{
			// Handle keyboard input for movement
			glm::vec3 direction(0.0f);
			if (input.IsKeyPressed(AC_KEY_W))
				direction += glm::vec3(0, 0, -1); // Forward
			if (input.IsKeyPressed(AC_KEY_S))
				direction += glm::vec3(0, 0, 1);  // Backward
			if (input.IsKeyPressed(AC_KEY_A))
				direction += glm::vec3(-1, 0, 0); // Left
			if (input.IsKeyPressed(AC_KEY_D))
				direction += glm::vec3(1, 0, 0);  // Right
			if(input.IsKeyPressed(AC_KEY_Q))
				transform.RotateRoll(moveComp.rotationSpeed * deltaTime); // Rotate Left
			if (input.IsKeyPressed(AC_KEY_E))
				transform.RotateRoll(-moveComp.rotationSpeed * deltaTime);  // Rotate Right
			if(input.IsKeyPressed(AC_KEY_LEFT_SHIFT))
				direction += glm::vec3(0, 1, 0);  // Up
			if (input.IsKeyPressed(AC_KEY_LEFT_CONTROL))
				direction += glm::vec3(0, -1, 0); // Down
			if (glm::length(direction) > 0.0f)
			{
				//move in direction of current rotation
				direction = transform.rotation.operator glm::mat<4, 4, float, glm::packed_highp>() * glm::vec4(direction, 0.0f);
				direction = glm::normalize(direction);
				transform.position += direction * moveComp.speed * deltaTime;
			}
			// Handle mouse input for rotation
			glm::vec2 mouseDelta = input.GetMouseDelta();
			//rotate relative to current axis
			transform.RotateYaw(-mouseDelta.x * moveComp.mouseSensitivity);
			transform.RotatePitch(-mouseDelta.y * moveComp.mouseSensitivity);


		});

	
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
	world.RegisterType<movementComponent>();
	world.AddUpdateSystem(Movement, 0);


	Entity camera = world.CreateEntity();
	world.Add<Camera>(camera, Camera{ });
	world.Add<Transform>(camera, Transform());
	world.Get<Transform>(camera).position = { 0, 0, 0};
	world.Add<movementComponent>(camera, movementComponent{ 5.0f, 0.1f, 90.0f });

	Entity object = world.CreateEntity();
	world.Add<Transform>(object, Transform(glm::vec3(0,-0.3,-1),0,1));

	// After creating the OpenGLModel, add validation
	OpenGLModel model(CURPATH + "/Assets/teapot/Chaynik.obj");
	world.Add<OpenGLModel>(object, std::move(model));

	Entity e2 = world.CreateEntity("");
	world.Add<Sprite>(e2, Sprite::Create("Default", world.GetResourse<TextureManager>()));
	world.Add<Transform>(e2, Transform({ 0,0,-1000 }));

	Entity light = world.CreateEntity();
	world.Add<AmbientLight>(light, { glm::vec3(0.2,0.2,0.2) });

	Entity pointLight = world.CreateEntity();
	world.Add<PointLight>(pointLight, { glm::vec3(0.9,0.9,0.9)});
	world.Add<Transform>(pointLight, Transform({ 1,1,0 }));


	

	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();

		mRenderer& renderer = world.GetResourse<mRenderer>();

		
		//world.Get<Transform>(object).RotateY(1);
		//world.Get<Transform>(object).RotateX(0.7);

		//rotate light around object
		float time = (float)glfwGetTime();
		float radius = 5.0f;
		float lightX = sin(time) * radius;
		float lightZ = cos(time) * radius;
		world.Get<Transform>(pointLight).position = glm::vec3(lightX, 1.0f, lightZ - 1);

		world.Update();
		if (exitgame)
			break;

		win.OnUpdate();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Add this in your render loop for debugging
		
	}
}
