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
#include "EngineComponents/Sprite.h"
#include "EngineComponents/Time.h"
#include "EngineSystems/RenderSystems.h"
#include "EngineSystems/PhysicsSystem.h" // Added physics system
#include "EngineComponents/Physics/Physics.h"
#include "EngineSystems/TimeSystem.h"
namespace ac
{

	//Do not change things unless you truely know what you are doing
	void InitEngine(World& world)
	{

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

		world.AddResource<ac::EventManager>(new EventManager());
		world.AddResource<Time>(new Time());	
		world.AddResource<WinWindow>(new WinWindow({ "AC", 1280, 720 }, world.GetResourse<EventManager>()));
		world.AddResource<OpenGLRenderer>(new OpenGLRenderer());
		world.AddResource<WindowsInput>(new WindowsInput());
		world.AddResource<TextureManager>(new TextureManager());
		world.AddResource<ModelManager>(new ModelManager());
		world.AddResource<CollisionLayer>(new CollisionLayer());

		world.GetResourse<TextureManager>()
			.AddTexture("Default", "C:/C++Projet/GameEngine/GameEngine/SandBox/Image/null.png")
			.AddTexture("Grass", "C:/C++Projet/GameEngine/GameEngine/SandBox/Image/grass.png")
			.AddTexture("Red", "C:/C++Projet/GameEngine/GameEngine/SandBox/Image/red.jpg");
		
		world.GetResourse<EventManager>()
			.AddListener<OnAdded<Sprite>>(OnSpriteAdded)
			.AddListener<OnDeleted<Sprite>>(OnSpriteDelete);

		world.AddPreUpdateSystem(UpdateTimeSystem, 0);

		// Register physics systems
		world.AddUpdateSystem(PhysicsSystem::PhysicsStep, 1); // Run physics step early in update
		world.AddUpdateSystem(PhysicsSystem::CollisionSystem, 2); // Run collision detection after physics update
		
		world.AddPostUpdateSystem(RenderCircle, 1);
		world.AddPostUpdateSystem(PhysicsSystem::Physics2DStep, 1);
		world.AddPostUpdateSystem(PhysicsSystem::Collision2DSystem, 2); // Run collision detection after physics update
		world.AddPostUpdateSystem(RenderSprite, 9);
		

	}
}