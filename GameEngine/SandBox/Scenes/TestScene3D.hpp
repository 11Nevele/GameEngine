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
	float speed = 0.05f; // units per second
	float mouseSensitivity = 0.1f;
	float rotationSpeed = 10.0f; // degrees per second
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
			/*if (input.IsKeyPressed(AC_KEY_Q))
				transform.RotateRoll(moveComp.rotationSpeed * deltaTime); // Rotate Left
			if (input.IsKeyPressed(AC_KEY_E))
				transform.RotateRoll(-moveComp.rotationSpeed * deltaTime);  // Rotate Right*/
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
			glm::vec3 rotation = glm::eulerAngles(transform.rotation);
			rotation.y += glm::radians(-mouseDelta.x * moveComp.mouseSensitivity);
			rotation.x += glm::radians(-mouseDelta.y * moveComp.mouseSensitivity);
			rotation.z = 0.0f;
			transform.rotation = glm::quat(rotation);


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
	world.Add<movementComponent>(camera, movementComponent{ 1.0f, 0.1f, 5.0f });

	vector<string> modelPaths =
	{
		CURPATH + "/Assets/teapot/Chaynik.obj",
		//CURPATH + "/Assets/grass/Grass_Block.obj",
		//CURPATH + "/Assets/plane/Seahawk.obj",
		//CURPATH + "/Assets/trex/trex.obj"
	};

	
	for (int i = 0; i < 10; i++)
	{
		Entity object = world.CreateEntity();

		string modelPath = modelPaths[rand() % modelPaths.size()];
		float angle = glm::two_pi<float>() * static_cast<float>(i) / 10.0f;
		float radius = 2.0f;
		float x = cos(angle) * radius;
		float z = sin(angle) * radius - 10.0f;
		float rotation = -glm::degrees(angle) + 90.0f;
		float scale = 1.0f;

		if (modelPath.find("Grass_Block.obj") != string::npos)
			scale *= 0.2f;
		OpenGLModel model(modelPaths[0]);
		world.Add<Transform>(object, Transform(glm::vec3(x, -0.3f, z), rotation, scale));

		
		world.Add<OpenGLModel>(object, std::move(model));
	}

	Entity e2 = world.CreateEntity("");
	world.Add<Sprite>(e2, Sprite::Create("Default", world.GetResourse<TextureManager>()));
	world.Add<Transform>(e2, Transform({ 0,0,-1000 }));

	Entity light = world.CreateEntity();
	world.Add<AmbientLight>(light, { glm::vec3(0.2,0.2,0.2) });

	Entity pointLightWarm = world.CreateEntity();
	world.Add<PointLight>(pointLightWarm, { glm::vec3(1.0f,0.85f,0.65f) });
	world.Add<Transform>(pointLightWarm, Transform({ 5.0f,1.5f,-10.0f }));

	Entity pointLightBlue = world.CreateEntity();
	world.Add<PointLight>(pointLightBlue, { glm::vec3(0.35f,0.55f,1.0f) });
	world.Add<Transform>(pointLightBlue, Transform({ -5.0f,2.0f,-10.0f }));

	Entity pointLightMagenta = world.CreateEntity();
	world.Add<PointLight>(pointLightMagenta, { glm::vec3(1.0f,0.3f,0.8f) });
	world.Add<Transform>(pointLightMagenta, Transform({ 0.0f,3.0f,-4.0f }));


	

	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();

		mRenderer& renderer = world.GetResourse<mRenderer>();

		
		//world.Get<Transform>(object).RotateY(1);
		//world.Get<Transform>(object).RotateX(0.7);

		float time = (float)glfwGetTime();
		world.Get<Transform>(pointLightWarm).position = glm::vec3(sin(time) * 5.0f, 1.5f, cos(time) * 5.0f - 10.0f);
		world.Get<Transform>(pointLightBlue).position = glm::vec3(cos(time * 0.7f) * 7.0f, 2.5f, sin(time * 0.7f) * 4.0f - 10.0f);
		world.Get<Transform>(pointLightMagenta).position = glm::vec3(sin(time * 1.3f) * 3.0f, 3.5f + sin(time * 2.0f) * 0.5f, cos(time * 1.3f) * 3.0f - 10.0f);

		world.Update();
		if (exitgame)
			break;

		win.OnUpdate();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Add this in your render loop for debugging
		
	}
}
