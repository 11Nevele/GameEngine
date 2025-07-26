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


struct movement
{
	int up, down, left, right;
	glm::vec3 velocity;
};


struct PlayerMovement
{

};
void Movement(ac::World& world)
{
	
	mInput& input = world.GetResourse<mInput>();
	mRenderer& rend = world.GetResourse<mRenderer>();
	Time& time = world.GetResourse<Time>();
	auto t = world.View<movement, Transform>().GetPacked();
	for (auto i : t)
	{
		auto& transform = get<1>(i.components);
		auto& m = get < 0> (i.components);
		if (input.IsKeyPressed(m.left))
			transform.position.x -= m.velocity.x * time.Delta();

		if (input.IsKeyPressed(m.right))
			transform.position.x += m.velocity.x * time.Delta();

		if (input.IsKeyPressed(m.down))
			transform.position.y -= m.velocity.y * time.Delta();

		if (input.IsKeyPressed(m.up))
			transform.position.y += m.velocity.y * time.Delta();
	}
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


	world.Add<Camera>(camera, Camera{ });
	world.Add<AudioListener>(camera, AudioListener());
	world.Add<Transform>(camera, Transform());
	world.Get<Transform>(camera).position = { 0, 0, 0 };
	world.Add<movement>(camera, movement{ AC_KEY_W, AC_KEY_S, AC_KEY_A, AC_KEY_D, {200.0f, 200.0f, 0.0f} });

	world.AddUpdateSystem(Movement, 0);


	LoadAssets(world);

	Entity tilemap = world.CreateEntity();
	const int width = 10, height = 10;
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(width, height, 40, 40));

	world.GetResourse<AudioManager>().RegisterAudio("Test", CURPATH + "/SandBox/Audio/test.wav");
	

	Entity TextEntity = world.CreateEntity("TextEntity");
	world.Add<Text>(TextEntity, Text("Hello world", 48, {0.5,0.5}));
	world.Add<Transform>(TextEntity, Transform());
	world.Add<movement>(TextEntity, movement{ AC_KEY_UP, AC_KEY_DOWN, AC_KEY_LEFT, AC_KEY_RIGHT, {200.0f, 200.0f, 0.0f} });
	


	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();
		
		mRenderer& renderer = world.GetResourse<mRenderer>();

		world.Update();

		win.OnUpdate();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}