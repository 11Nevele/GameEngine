#include "acpch.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glfw3.h>
#include "Achoium.h"

#include "UnitTests/TestUnit.h"
using namespace ac;

EventManager eventManager;
class Test
{
public:
	int data;
	bool OnEvent(const int& i)
	{
		std::cout << i + data << std::endl;
		return true;
	}
};
void Add()
{
	Test test;
	test.data = 42;
	eventManager.AddListener<int>(std::bind(&Test::OnEvent, &test, std::placeholders::_1));
}
using namespace std;
using namespace ac;

using mTexture2D = OpenGLTexture2D;
using mInput = WindowsInput;
using mWindow = WinWindow;
using mRenderer = OpenGLRenderer;

unsigned int vertexArray, vertexBuffer, indexBuffer;
float verteies[] = {
	// Front face
	 0.5,  0.5,  0.5, 1.0, 1.0,  // 0
	 0.5, -0.5,  0.5, 1.0, 0.0, // 1
	-0.5, -0.5,  0.5, 0.0, 0.0, // 2
	-0.5,  0.5,  0.5, 0.0, 1.0, // 3

	// Back face
	0.5,  0.5, -0.5, 1.0, 1.0, // 4
	0.5, -0.5, -0.5, 1.0, 0.0, // 5
	-0.5, -0.5, -0.5, 0.0, 0.0, // 6
	-0.5,  0.5, -0.5, 0.0, 1.0  // 7
};

float fullcube[] = {
	// Positions          // Texture Coords
	// Front face (Z = 0.5)
	-0.5f, -0.5f,  0.5f,  0.0f, 2.0f/3.0f,  // Bottom-left
	 0.5f, -0.5f,  0.5f,  0.25f, 2.0f / 3.0f,  // Bottom-right
	 0.5f,  0.5f,  0.5f,  0.25f, 1.0f,  // Top-right
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // Top-left

	// Back face (Z = -0.5)
	-0.5f, -0.5f, -0.5f,  0.25f, 2.0f / 3.0f,  // Bottom-right (mirrored)
	 0.5f, -0.5f, -0.5f,  0.0f, 2.0f / 3.0f,  // Bottom-left (mirrored)
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Top-left (mirrored)
	-0.5f,  0.5f, -0.5f,  0.25f, 1.0f,  // Top-right (mirrored)

	// Left face (X = -0.5)
	-0.5f, -0.5f, -0.5f,  0.0f, 2.0f / 3.0f,  // Bottom-back
	-0.5f, -0.5f,  0.5f, 0.25f, 2.0f / 3.0f,  // Bottom-front
	-0.5f,  0.5f,  0.5f,  0.25f, 1.0f,  // Top-front
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Top-back

	// Right face (X = 0.5)
	 0.5f, -0.5f,  0.5f,   0.0f, 2.0f / 3.0f,  // Bottom-front
	 0.5f, -0.5f, -0.5f,  0.25f, 2.0f / 3.0f,  // Bottom-back
	 0.5f,  0.5f, -0.5f,  0.25f, 1.0f,  // Top-back
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // Top-front

	 // Bottom face (Y = -0.5)
	 -0.5f, -0.5f, -0.5f,  0.0f, 2.0 / 3.0f,  // Back-left
	  0.5f, -0.5f, -0.5f,  0.25f, 2.0 / 3.0f,  // Back-right
	  0.5f, -0.5f,  0.5f,  0.25f, 1.0 / 3.0f,  // Front-right
	 -0.5f, -0.5f,  0.5f,  0.0f, 1.0/3.0f,  // Front-left

	 // Top face (Y = 0.5)
	 -0.5f,  0.5f,  0.5f,  0.5f, 1.0/3.0f,  // Front-left
	  0.5f,  0.5f,  0.5f,  0.75f, 1.0/3.0f,  // Front-right
	  0.5f,  0.5f, -0.5f,  0.75f, 2.0 / 3.0f,  // Back-right
	 -0.5f,  0.5f, -0.5f,  0.5f, 2.0 / 3.0f   // Back-left
};

uint32_t index[] = {
	// Front face
	0, 1, 2,
	2, 3, 0,
	
	// Back face
	4, 6, 5,
	6, 4, 7,

	// Left face
	3, 2, 6,
	6, 7, 3,

	// Right face
	0, 5, 1,
	5, 0, 4,

	// Top face
	0, 3, 7,
	7, 4, 0,

	// Bottom face
	1, 6, 2,
	6, 1, 5
	
};

unsigned int fullcubeindices[] = {
	// Front face
	0, 1, 2,   // Triangle 1
	2, 3, 0,   // Triangle 2
	// Back face
	4, 6, 5,   // Triangle 1
	6, 4, 7,   // Triangle 2
	// Left face
	8, 9, 10,  // Triangle 1
	10, 11, 8, // Triangle 2
	// Right face
	12, 13, 14, // Triangle 1
	14, 15, 12, // Triangle 2
	// Bottom face
	16, 17, 18, // Triangle 1
	18, 19, 16, // Triangle 2
	// Top face
	20, 21, 22, // Triangle 1
	22, 23, 20  // Triangle 2
};

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

void PlayerControl(ac::World& world)
{

	mInput& input = world.GetResourse<mInput>();
	mWindow& win = world.GetResourse<mWindow>();
	Time& time = world.GetResourse<Time>();
	auto t = world.View<PlayerMovement, RigidBody2D>().GetPacked();
	
	for (auto i : t)
	{
		const float f = 10.0;
		auto& rb = get<1>(i.components);
		if (input.IsKeyPressed(AC_KEY_LEFT, win))
			rb.ApplyForce({ -f,0 });
		if (input.IsKeyPressed(AC_KEY_RIGHT, win))
			rb.ApplyForce({ f,0 });

		if (input.IsKeyPressed(AC_KEY_UP, win))
			rb.ApplyForce({ 0,f });

		if (input.IsKeyPressed(AC_KEY_DOWN, win))
			rb.ApplyForce({ 0,-f });

	}
}

void RandomBlockGenerator(World& world)
{
	WindowsInput& input = world.GetResourse<WindowsInput>();
	mWindow& win = world.GetResourse<mWindow>();
	static bool pressed = false;
	if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1, win) && pressed == false)
	{
		pressed = true;
		//randomly generate width height and angular  velocity
		float width = rand() % 100 + 50; // Width between 50 and 150
		float height = rand() % 100 + 50; // Height between 50 and 150
		float angularVelocity = (rand() % 100 - 50) / 10.0f; // Angular velocity between -5 and 5
		float mass = width * height * 0.01f;
		float v = rand() % 200 + 100;


		glm::vec2 mousePos = input.GetMousePosition(win);
		mousePos.y = 720 - mousePos.y; // Invert Y coordinate for OpenGL
		Entity block = world.CreateEntity();
		world.Add<Transform>(block, Transform());
		world.Get<Transform>(block).position = { mousePos.x, mousePos.y, 0 };
		world.Add<RigidBody2D>(block, RigidBody2D(mass, 0, 0.1, true, false, false));
		world.Add<RectCollider2D>(block, RectCollider2D(width, height));
		world.Get<RectCollider2D>(block).offset = glm::vec3(0, 0, 0);
		world.Get<RigidBody2D>(block).inertiaTensor = mass * (float)(width * width + height * height) / 12.0;
		world.Get<RigidBody2D>(block).angularVelocity = angularVelocity;
		//world.Get<RigidBody2D>(block).velocity.y = v;
	}
	if (!input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1, win))
		pressed = false;
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
		//world.Get<RigidBody2D>(block).velocity.y = v;
	}
	if (!input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2, win))
		pressed = false;
}

int main()
{
	//StartTest();

	//StartTest();
	ac::World world;
	InitEngine(world);

	Entity ground = world.CreateEntity();
	Entity ground1 = world.CreateEntity();
	Entity obj1 = world.CreateEntity();
	Entity obj2 = world.CreateEntity();
	Entity camera = world.CreateEntity();

	world.Add<Camera>(camera, Camera{ { 100.0f, 100.0f, 0 } });
	world.Add<Transform>(camera, Transform());
	world.Get<Transform>(camera).position = { 0, 0, 0 };

	/*world.Add<Transform>(obj1, Transform());
	world.Get<Transform>(obj1).position = { 500, 600, 0 };
	world.Get<Transform>(obj1).RotateZ(0);
	world.Add<RigidBody2D>(obj1, RigidBody2D(1, 1, 0, true, false, false));
	world.Add<RectCollider2D>(obj1, RectCollider2D(45, 30));
	world.Get<RectCollider2D>(obj1).offset = glm::vec3(0, 0, 0);
	world.Get<RigidBody2D>(obj1).inertiaTensor = (45.0 * 45.0 + 900.0) / 12.0;
	world.Get<RigidBody2D>(obj1).velocity = { 0, -50};

	world.Add<Transform>(obj2, Transform());
	world.Get<Transform>(obj2).position = { 500,400,0 };
	world.Get<Transform>(obj2).RotateZ(0);
	world.Add<RigidBody2D>(obj2, RigidBody2D(1, 0, 0.5, true, false, false));
	world.Add<RectCollider2D>(obj2, RectCollider2D(100, 50));
	world.Get<RectCollider2D>(obj2).offset = glm::vec3(0, 0, 0);
	world.Get<RigidBody2D>(obj2).inertiaTensor = 12500 / 12.0;
	world.Add<PlayerMovement>(obj2, PlayerMovement());
	world.Get<RigidBody2D>(obj2).angularVelocity = 1.1;
	world.Get<RigidBody2D>(obj2).velocity = { 0, -75 };*/

	world.Add<Transform>(ground, Transform());
	world.Get<Transform>(ground).scale.x = 10;
	world.Add<RigidBody2D>(ground, RigidBody2D(1,1,0.0, false, true, true));
	world.Add<RectCollider2D>(ground, RectCollider2D(10000, 100));
	world.Get<RectCollider2D>(ground).offset = glm::vec3(0,0,0);
	world.Get<Transform>(ground).RotateZ(10);
	world.Get<RigidBody2D>(ground).inertiaTensor = 100010000 / 12.0;

	world.Add<Transform>(ground1, Transform());
	world.Get<Transform>(ground1).scale.x = 10;
	world.Get<Transform>(ground1).position = {0,720,0};
	world.Add<RigidBody2D>(ground1, RigidBody2D(1, 1, 0, false, true, true));
	world.Add<RectCollider2D>(ground1, RectCollider2D(10000, 100));
	world.Get<RectCollider2D>(ground1).offset = glm::vec3(0, 0, 0);
	world.Get<Transform>(ground1).RotateZ(0);
	world.Get<RigidBody2D>(ground1).inertiaTensor = 100010000 / 12.0;

	world.AddPostUpdateSystem(PhysicsSystem::DebugPhysics, 9);
	//world.Add<Sprite>(ground, Sprite::Create("Default", world.GetResourse<TextureManager>()));
	//world.Add<Sprite>(obj1, Sprite::Create("Default", world.GetResourse<TextureManager>()));
	//world.Add<Sprite>(obj2, Sprite::Create("Default", world.GetResourse<TextureManager>()));

	world.AddUpdateSystem(CameraMovement, 0);
	world.AddUpdateSystem(RandomBlockGenerator, 0);
	world.AddPostUpdateSystem(RandomCircleGenerator, 0);
	

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