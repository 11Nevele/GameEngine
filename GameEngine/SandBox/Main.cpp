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

	// ֪ͨ��Ⱦ�������ӿڴ�С
	mRenderer& renderer = world.GetResourse<mRenderer>();
	renderer.OnWindowResize(event.width, event.height);

	// ���������Ϸ�߼���Ҫ֪�����ڴ�С���������������

	return true; // ���� true ��ʾ�¼��ѱ�����
}

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
	world.GetResourse<TextureManager>().AddTexture("White", curPath + "/Assets/Image/White.png");
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
	InitEngine(world);
	
	Entity camera = world.CreateEntity();


	world.Add<Camera>(camera, Camera{ });
	world.Add<AudioListener>(camera, AudioListener());
	world.Add<Transform>(camera, Transform());
	world.Get<Transform>(camera).position = { 0, 0, 0 };
	world.Add<movement>(camera, movement{ AC_KEY_W, AC_KEY_S, AC_KEY_A, AC_KEY_D, {200.0f, 200.0f, 0.0f} });

	world.AddUpdateSystem(Movement, 0);
	EventManager& eventManager = world.GetResourse<EventManager>();
	eventManager.AddListener<WindowCloseEvent>(HandelGameExit);
	eventManager.AddListener<WindowResizeEvent>(HandleWindowResize);


	LoadAssets(world);

	Entity tilemap = world.CreateEntity();
	const int width = 10, height = 10;
	world.Add<ac::Tilemap>(tilemap, ac::Tilemap(width, height, 40, 40));

	world.GetResourse<AudioManager>().RegisterAudio("Test", CURPATH + "/Assets/Audio/test.wav");
	

	Entity e1 = world.CreateEntity("");
	//world.Add<Text>(TextEntity, Text("Hello world", 48, {0.5,0.5}));
	world.Add<Sprite>(e1, Sprite::Create("White", world.GetResourse<TextureManager>()));
	world.Add<Transform>(e1, Transform(glm::vec3{500,500,0.5}, 0.0f, 100.0f));
	world.Add<movement>(e1, movement{ AC_KEY_UP, AC_KEY_DOWN, AC_KEY_LEFT, AC_KEY_RIGHT, {200.0f, 200.0f, 0.0f} });
	
	Entity e2 = world.CreateEntity("");
	//world.Add<Text>(TextEntity, Text("Hello world", 48, {0.5,0.5}));
	world.Add<Sprite>(e2, Sprite::Create("Default", world.GetResourse<TextureManager>()));
	world.Add<Transform>(e2, Transform({0,0,0}));
	world.Add<movement>(e2, movement{ AC_KEY_8, AC_KEY_5, AC_KEY_4, AC_KEY_6, {200.0f, 200.0f, 0.0f} });

	Entity e3 = world.CreateEntity();
	world.Add<Text>(e3, Text("Hello world", 48, { 0.5,0.5 }, {1,0,0}));
	world.Add<Transform>(e3, Transform({ 500,500,-0.1 }));


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