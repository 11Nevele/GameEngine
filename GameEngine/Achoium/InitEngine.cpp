#include "acpch.h"
#include "InitEngine.h"
#include "Event/Event.hpp";
#include "Window/Window.h"
#include "Window/WinWindow.h"
#include "Render/Render.h"
#include "Util/util.h"
#include "Math/Transform.h"
#include "Input/Keycode.h"
#include "AssetManagement/AssetManager.h"
#include "AssetManagement/AudioManager.h"
#include "EngineComponents/Sprite.h"
#include "EngineComponents/Time.h"
#include "EngineComponents/Audio/Audio.h"
#include "EngineSystems/RenderSystems.h"
#include "EngineSystems/PhysicsSystem.h" // Added physics system
#include "EngineSystems/AudioSystem.h"
#include "EngineComponents/Physics/Physics.h"
#include "EngineComponents/Tilemap.h"
#include "EngineSystems/TimeSystem.h"
#include "Global.h"
#include "Input/InputManager.h"
#include <filesystem>
namespace ac
{

	//Do not change things unless you truely know what you are doing
	void InitEngine(World& world)
	{
		CURPATH = filesystem::current_path().string();
		world.RegisterType<Sprite>();
		world.RegisterType<Transform>();
		world.RegisterType<Collider>();
		world.RegisterType<RigidBody>();
		world.RegisterType<SphereCollider>();
		world.RegisterType<BoxCollider>();
		world.RegisterType<CircleCollider2D>();
		world.RegisterType<PolygonCollider2D>();
		world.RegisterType<RectCollider2D>();
		world.RegisterType<RigidBody2D>();
		world.RegisterType<Tilemap>();
		world.RegisterType<TilemapElement>();
		// 注册音频组件
		world.RegisterType<AudioSource>();
		world.RegisterType<AudioListener>();

		world.AddResource<ac::EventManager>(new EventManager());
		world.AddResource<Time>(new Time());	
		world.AddResource<WinWindow>(new WinWindow({ "AC", 1280, 720 }, world.GetResourse<EventManager>()));
		world.AddResource<OpenGLRenderer>(new OpenGLRenderer());
		world.AddResource<WindowsInput>(new WindowsInput(&world.GetResourse<WinWindow>()));
		world.AddResource<TextureManager>(new TextureManager());
		world.AddResource<ModelManager>(new ModelManager());
		world.AddResource<CollisionLayer>(new CollisionLayer());
		world.AddResource<InputManager>(new InputManager());
		// 添加音频管理器资源
		world.AddResource<AudioManager>(new AudioManager());

		std::string currentPath = std::filesystem::current_path().string();
		world.GetResourse<TextureManager>()
			.AddTexture("Default", currentPath + "/SandBox/Image/null.png")
			.AddTexture("Grass", currentPath + "/SandBox/Image/grass.png")
			.AddTexture("Red", currentPath + "/SandBox/Image/red.jpg");
		
		world.GetResourse<EventManager>()
			.AddListener<OnAdded<Sprite>>(OnSpriteAdded)
			.AddListener<OnDeleted<Sprite>>(OnSpriteDelete)
			.AddListener<OnAdded<AudioSource>>(OnAudioSourceAdded)
			.AddListener<OnDeleted<AudioSource>>(OnAudioSourceDeleted);

		world.AddPreUpdateSystem(UpdateTimeSystem, 0);
		world.AddPreUpdateSystem(InputManagerSystem::UpdateInput, 1); // Update input before other systems

		// Register physics systems
		world.AddUpdateSystem(PhysicsSystem::PhysicsStep, 1); // Run physics step early in update
		world.AddUpdateSystem(PhysicsSystem::CollisionSystem, 2); // Run collision detection after physics update
		// 注册音频系统
		world.AddPostUpdateSystem(AudioSystem::UpdateAudio, 0); // 在物理系统之后更新音频
		
		world.AddPostUpdateSystem(PhysicsSystem::Physics2DStep, 1);
		world.AddPostUpdateSystem(PhysicsSystem::Collision2DSystem, 2); // Run collision detection after physics update
		world.AddPostUpdateSystem(RenderSprite, 9);
		world.AddPostUpdateSystem(RenderTilemap, 9);
		

		
		//add event listener
		EventManager& eventManager = world.GetResourse<EventManager>();
		eventManager.AddListener<OnAdded<TilemapElement>>(OnTilemapElementAdded);


	}
}