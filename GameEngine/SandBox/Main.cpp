#include "acpch.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glfw3.h>
#include "Achoium.h"
#include <fstream>

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
		if (input.IsKeyPressed(AC_KEY_A))
			transform.position.x -= camera.movementSpeed.x * time.Delta();

		if (input.IsKeyPressed(AC_KEY_D))
			transform.position.x += camera.movementSpeed.x * time.Delta();

		if (input.IsKeyPressed(AC_KEY_W))
			transform.position.z -= camera.movementSpeed.z * time.Delta();

		if (input.IsKeyPressed(AC_KEY_S))
			transform.position.z += camera.movementSpeed.z * time.Delta();

		if (input.IsKeyPressed(AC_KEY_LEFT_SHIFT))
			transform.position.y += camera.movementSpeed.y * time.Delta();

		if (input.IsKeyPressed(AC_KEY_LEFT_CONTROL))
			transform.position.y -= camera.movementSpeed.y * time.Delta();
		
		rend.UpdateCamera(transform.asMat4(true));
	}
}

void RandomCircleGenerator(World& world)
{
	WindowsInput& input = world.GetResourse<WindowsInput>();
	mWindow& win = world.GetResourse<mWindow>();
	static bool pressed = false;
	if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2) && pressed == false)
	{
		pressed = true;
		//randomly generate width height and angular  velocity
		float radius = rand() % 100 + 50; // Width between 50 and 150
		float angularVelocity = 0.0f; // Angular velocity between -5 and 5
		float mass = radius * radius * 0.01f;
		float v = rand() % 200 + 100;


		glm::vec2 mousePos = input.GetMousePosition();
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
	if (!input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
		pressed = false;
}


void LoadAssets(World& world)
{
	std::string curPath = filesystem::current_path().string();
	world.GetResourse<TextureManager>().AddTexture("White", curPath + "/SandBox/Image/White.png");
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

int main()
{
	

	srand(time(0));
	
	ac::World world;
	InitEngine(world);
	
	Entity camera = world.CreateEntity();


	world.Add<Camera>(camera, Camera{ { 100.0f, 100.0f, 0 } });
	world.Add<AudioListener>(camera, AudioListener());
	world.Add<Transform>(camera, Transform());
	world.Get<Transform>(camera).position = { 0, 0, 0 };

	world.AddUpdateSystem(CameraMovement, 0);
	world.AddPostUpdateSystem(RandomCircleGenerator, 0);

	//GenerateOrganisms(world);

	//world.AddPostUpdateSystem(RenderCollider, 9);

	LoadAssets(world);

	Entity tilemap = world.CreateEntity();
	const int width = 10, height = 10;
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(width, height, 40, 40));

	world.GetResourse<AudioManager>().RegisterAudio("Test", CURPATH + "/SandBox/Audio/test.wav");
	
	Entity audioTest = world.CreateEntity();
	world.Add<AudioSource>(audioTest, AudioSource::Create(world, "Test", true, 0.5f, AudioType::Music, true));
	world.Add<Sprite>(audioTest, Sprite::Create("Red", world.GetResourse<TextureManager>()));
	world.Get<Sprite>(audioTest).width = 100;
	world.Get<Sprite>(audioTest).height = 100;
	world.Add<Transform>(audioTest, Transform());

	world.AddUpdateSystem(TestAudioSystem, 0);

	for(int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			Entity renderObject = world.CreateEntity();
			world.Add<ac::TilemapElement>(renderObject, ac::TilemapElement(tilemap, i, j));
			world.Add<Sprite>(renderObject, Sprite::Create("White", world.GetResourse<TextureManager>()));
			world.Get<Sprite>(renderObject).color = {0,0,0,1};
			world.Get<Sprite>(renderObject).width = 40;
			world.Get<Sprite>(renderObject).height = 40;
		}
	


	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();
		
		world.Update();

		win.OnUpdate();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}