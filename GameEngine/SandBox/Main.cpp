#include "acpch.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glfw3.h>
#include "Achoium.h"
#include "Components/Components.h"


#include "UnitTests/TestUnit.h"
using namespace ac;


using namespace std;
using namespace ac;

using mTexture2D = OpenGLTexture2D;
using mInput = WindowsInput;
using mWindow = WinWindow;
using mRenderer = OpenGLRenderer;

struct Camera {
	glm::vec3 movementSpeed;
};
struct movement
{
	glm::vec3 velocity;
};

struct PlayerMovement
{

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
		auto& camera = get < 0> (i.components);
		if (input.IsKeyPressed(AC_KEY_A, win))
			transform.position.x -= camera.movementSpeed.x * time.Delta();

		if (input.IsKeyPressed(AC_KEY_D, win))
			transform.position.x += camera.movementSpeed.x * time.Delta();

		if (input.IsKeyPressed(AC_KEY_W, win))
			transform.position.z -= camera.movementSpeed.z * time.Delta();

		if (input.IsKeyPressed(AC_KEY_S, win))
			transform.position.z += camera.movementSpeed.z * time.Delta();

		if (input.IsKeyPressed(AC_KEY_LEFT_SHIFT, win))
			transform.position.y += camera.movementSpeed.y * time.Delta();

		if (input.IsKeyPressed(AC_KEY_LEFT_CONTROL, win))
			transform.position.y -= camera.movementSpeed.y * time.Delta();
		
		rend.UpdateCamera(transform.asMat4(true));
	}
}

void RandomCircleGenerator(World& world)
{
	WindowsInput& input = world.GetResourse<WindowsInput>();
	mWindow& win = world.GetResourse<mWindow>();
	static bool pressed = false;
	if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2, win) && pressed == false)
	{
		pressed = true;
		//randomly generate width height and angular  velocity
		float radius = rand() % 100 + 50; // Width between 50 and 150
		float angularVelocity = 0.0f; // Angular velocity between -5 and 5
		float mass = radius * radius * 0.01f;
		float v = rand() % 200 + 100;


		glm::vec2 mousePos = input.GetMousePosition(win);
		mousePos.y = 720 - mousePos.y; // Invert Y coordinate for OpenGL
		Entity block = world.CreateEntity();
		world.Add<Transform>(block, Transform());
		world.Get<Transform>(block).position = { mousePos.x, mousePos.y, 0 };
		world.Add<RigidBody2D>(block, RigidBody2D(mass, 0.5, 0.1, true, false, false));
		world.Add<CircleCollider2D>(block, CircleCollider2D(radius));
		world.Get<CircleCollider2D>(block).offset = glm::vec3(0, 0, 0);
		world.Get<RigidBody2D>(block).inertiaTensor = mass * std::pow(radius, 2);
		world.Get<RigidBody2D>(block).angularVelocity = angularVelocity;
		world.Get<RigidBody2D>(block).velocity.y = v;

		
	}
	if (!input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2, win))
		pressed = false;
}

void GenerateOrganisms(World& world)
{
	for (int i = 0; i < 100; ++i)
	{
		Entity e = world.CreateEntity();
		world.Add<NeuralNetwork>(e, std::move(NeuralNetwork::Create(3, { 2,4,4 })));
	}
}

int main()
{
	srand(time(0));
	ac::World world;
	InitEngine(world);

	Entity ground = world.CreateEntity();
	Entity ground1 = world.CreateEntity();
	Entity obj1 = world.CreateEntity();
	Entity obj2 = world.CreateEntity();
	Entity camera = world.CreateEntity();

	world.RegisterType<NeuralNetwork>();

	world.Add<Camera>(camera, Camera{ { 100.0f, 100.0f, 0 } });
	world.Add<Transform>(camera, Transform());
	world.Get<Transform>(camera).position = { 0, 0, 0 };

	world.Add<Transform>(ground, Transform());
	world.Get<Transform>(ground).scale.x = 10;
	world.Add<RigidBody2D>(ground, RigidBody2D(1,1,0.0, false, true, true));
	world.Add<RectCollider2D>(ground, RectCollider2D(10000, 100));
	world.Get<RectCollider2D>(ground).offset = glm::vec3(0,0,0);
	world.Get<Transform>(ground).RotateZ(0);
	world.Get<RigidBody2D>(ground).inertiaTensor = 100010000 / 12.0;

	world.Add<Transform>(ground1, Transform());
	world.Get<Transform>(ground1).scale.x = 10;
	world.Get<Transform>(ground1).position = {0,720,0};
	world.Add<RigidBody2D>(ground1, RigidBody2D(1, 1, 0, false, true, true));
	world.Add<RectCollider2D>(ground1, RectCollider2D(10000, 100));
	world.Get<RectCollider2D>(ground1).offset = glm::vec3(0, 0, 0);
	world.Get<Transform>(ground1).RotateZ(0);
	world.Get<RigidBody2D>(ground1).inertiaTensor = 100010000 / 12.0;


	world.AddUpdateSystem(CameraMovement, 0);
	world.AddPostUpdateSystem(RandomCircleGenerator, 0);
	world.AddUpdateSystem(NeuralNetworkSystems::UpdateNeuralNetworkSystem, 0);

	GenerateOrganisms(world);

	world.AddPostUpdateSystem(RenderCollider, 9);

	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();
		
		world.Update();


		win.OnUpdate();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}