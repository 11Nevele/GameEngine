# Achoium Game Engine

A modern C++20 game engine built with Entity-Component-System (ECS) architecture, featuring 2D/3D graphics, physics simulation, audio management, and comprehensive asset management systems.

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Getting Started](#getting-started)
- [Core Systems](#core-systems)
- [Components](#components)
- [Systems](#systems)
- [Asset Management](#asset-management)
- [Examples](#examples)
- [API Reference](#api-reference)
- [Contributing](#contributing)
- [License](#license)

## Features

### Core Engine Features
- **Entity-Component-System (ECS) Architecture**: High-performance data-oriented design
- **OpenGL Rendering**: Modern OpenGL-based 2D/3D graphics rendering
- **Physics Simulation**: Both 2D and 3D physics with collision detection
- **Audio System**: SoLoud-based audio management with 3D spatial audio
- **Input Handling**: Cross-platform input management
- **Event System**: Type-safe event handling and dispatching
- **Asset Management**: Texture, model, and audio asset management
- **Text Rendering**: TrueType font rendering system
- **Tilemap Support**: 2D tilemap rendering for level design

### Supported Platforms
- Windows (Primary)
- OpenGL 3.3+ compatible systems

## Architecture

The Achoium Engine follows a strict Entity-Component-System architecture:

### Core Classes

- **World**: Central ECS manager handling entities, components, and systems
- **Entity**: Lightweight ID representing game objects
- **Components**: Data containers (Transform, Sprite, RigidBody, etc.)
- **Systems**: Logic processors that operate on components

### System Execution Order

1. **Pre-Update Systems**: Time updates, input processing
2. **Update Systems**: Game logic, physics simulation
3. **Post-Update Systems**: Rendering, audio updates

## Getting Started

### Prerequisites

- Visual Studio 2022 or compatible C++20 compiler
- OpenGL 3.3+ compatible graphics driver
- Windows SDK

### Basic Setup

```cpp
#include "Achoium.h"
using namespace ac;

int main()
{
    // Create and initialize the world
    World world;
    InitEngine(world);
    
    // Create a camera entity
    Entity camera = world.CreateEntity("MainCamera");
    world.Add<Camera>(camera, Camera{});
    world.Add<AudioListener>(camera, AudioListener());
    world.Add<Transform>(camera, Transform());
    
    // Main game loop
    while (true)
    {
        world.Update();
        
        // Handle window updates and rendering
        auto& window = world.GetResourse<WinWindow>();
        auto& renderer = world.GetResourse<OpenGLRenderer>();
        
        window.OnUpdate();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    return 0;
}
```

## Core Systems

### World Management

The `World` class is the heart of the ECS system:

```cpp
// Entity management
Entity entity = world.CreateEntity("MyEntity");
world.DeleteEntity(entity);

// Component management
world.Add<Transform>(entity, Transform({0, 0, 0}));
world.Delete<Transform>(entity);
Transform& transform = world.Get<Transform>(entity);

// System management
world.AddUpdateSystem(MySystem, 0); // Priority 0-9
```

### Resource Management

Resources are singleton-like objects managed by the World:

```cpp
// Access engine resources
TextureManager& textures = world.GetResourse<TextureManager>();
AudioManager& audio = world.GetResourse<AudioManager>();
InputManager& input = world.GetResourse<InputManager>();
EventManager& events = world.GetResourse<EventManager>();
```

## Components

### Transform Component

Handles position, rotation, and scale in 3D space:

```cpp
Transform transform;
transform.position = {100.0f, 200.0f, 0.0f};
transform.rotation = glm::quat(glm::vec3(0, 0, glm::radians(45.0f)));
transform.scale = {2.0f, 2.0f, 1.0f};
```

### Sprite Component

2D graphics rendering:

```cpp
// Create sprite from texture
Sprite sprite = Sprite::Create("MyTexture", world.GetResourse<TextureManager>());
world.Add<Sprite>(entity, std::move(sprite));
```

### Audio Components

```cpp
// Audio source for playing sounds
AudioSource source;
source.audioID = audioManager.GetAudioID("MySound");
world.Add<AudioSource>(entity, source);

// Audio listener for receiving audio (typically on camera)
world.Add<AudioListener>(camera, AudioListener());
```

### Physics Components

#### 3D Physics
```cpp
// Rigid body for physics simulation
RigidBody rigidBody;
rigidBody.mass = 1.0f;
rigidBody.useGravity = true;
world.Add<RigidBody>(entity, rigidBody);

// Box collider
BoxCollider boxCollider;
boxCollider.size = {1.0f, 1.0f, 1.0f};
world.Add<BoxCollider>(entity, boxCollider);
```

#### 2D Physics
```cpp
// 2D rigid body
RigidBody2D rigidBody2D;
rigidBody2D.mass = 1.0f;
world.Add<RigidBody2D>(entity, rigidBody2D);

// Circle collider 2D
CircleCollider2D circleCollider;
circleCollider.radius = 0.5f;
world.Add<CircleCollider2D>(entity, circleCollider);
```

### Text Component

Text rendering with customizable fonts and colors:

```cpp
Text text("Hello World", 48.0f, Text::PivotMiddleCenter, {1.0f, 1.0f, 1.0f});
world.Add<Text>(entity, text);
```

## Systems

### Custom System Creation

```cpp
void MyCustomSystem(World& world)
{
    // Access resources
    Time& time = world.GetResourse<Time>();
    InputManager& input = world.GetResourse<InputManager>();
    
    // Process entities with specific components
    auto view = world.View<Transform, Sprite>();
    for (Entity entity : view)
    {
        Transform& transform = world.Get<Transform>(entity);
        Sprite& sprite = world.Get<Sprite>(entity);
        
        // System logic here
        transform.position.x += 100.0f * time.Delta();
    }
}

// Register the system
world.AddUpdateSystem(MyCustomSystem, 1);
```

### Built-in Systems

- **Physics Systems**: Handle 2D/3D physics simulation and collision detection
- **Render Systems**: Process sprite, text, and tilemap rendering
- **Audio Systems**: Update 3D audio positioning and playback
- **Input Systems**: Process keyboard and mouse input

## Asset Management

### Texture Management

```cpp
TextureManager& textureManager = world.GetResourse<TextureManager>();

// Load textures
textureManager.AddTexture("Player", "Assets/Images/player.png");
textureManager.AddTexture("Background", "Assets/Images/bg.jpg");

// Use textures
uint32_t textureID = textureManager.GetTextureID("Player");
Sprite sprite = Sprite::Create("Player", textureManager);
```

### Audio Management

```cpp
AudioManager& audioManager = world.GetResourse<AudioManager>();

// Register audio files
AudioID soundID = audioManager.RegisterAudio("Jump", "Assets/Audio/jump.wav");
AudioID musicID = audioManager.RegisterAudio("BGM", "Assets/Audio/background.ogg");

// Play audio
audioManager.Play(soundID);
audioManager.PlayByName("BGM", true, 0.7f); // Loop with 70% volume
```

## Examples

### Creating a Player Entity

```cpp
// Create player entity
Entity player = world.CreateEntity("Player");

// Add visual representation
Sprite playerSprite = Sprite::Create("PlayerTexture", world.GetResourse<TextureManager>());
world.Add<Sprite>(player, std::move(playerSprite));

// Add transform for positioning
world.Add<Transform>(player, Transform({400, 300, 0}));

// Add custom movement component
struct PlayerMovement {
    float speed = 200.0f;
    int leftKey = AC_KEY_A;
    int rightKey = AC_KEY_D;
    int upKey = AC_KEY_W;
    int downKey = AC_KEY_S;
};
world.Add<PlayerMovement>(player, PlayerMovement{});
```

### Player Movement System

```cpp
void PlayerMovementSystem(World& world)
{
    InputManager& input = world.GetResourse<InputManager>();
    Time& time = world.GetResourse<Time>();
    
    auto players = world.View<Transform, PlayerMovement>();
    for (Entity entity : players)
    {
        Transform& transform = world.Get<Transform>(entity);
        PlayerMovement& movement = world.Get<PlayerMovement>(entity);
        
        glm::vec3 velocity = {0, 0, 0};
        
        if (input.IsKeyPressed(movement.leftKey))
            velocity.x -= movement.speed;
        if (input.IsKeyPressed(movement.rightKey))
            velocity.x += movement.speed;
        if (input.IsKeyPressed(movement.upKey))
            velocity.y += movement.speed;
        if (input.IsKeyPressed(movement.downKey))
            velocity.y -= movement.speed;
            
        transform.position += velocity * time.Delta();
    }
}

// Register the system
world.AddUpdateSystem(PlayerMovementSystem, 1);
```

### Event Handling

```cpp
// Define custom events
struct PlayerDeathEvent {
    Entity playerEntity;
    glm::vec3 deathPosition;
};

// Event handler
bool OnPlayerDeath(const PlayerDeathEvent& event)
{
    ACMSG("Player died at position: " << event.deathPosition.x << ", " << event.deathPosition.y);
    // Handle player death logic
    return true;
}

// Register event listener
EventManager& eventManager = world.GetResourse<EventManager>();
eventManager.AddListener<PlayerDeathEvent>(OnPlayerDeath);

// Trigger event
PlayerDeathEvent deathEvent{playerEntity, transform.position};
eventManager.Invoke(deathEvent, AllowToken<PlayerDeathEvent>());
```

## API Reference

### Key Classes

#### World
- `Entity CreateEntity(string_view tag = "")` - Create new entity
- `void DeleteEntity(Entity id)` - Remove entity and all components
- `void Add<T>(Entity id, T&& component)` - Add component to entity
- `void Delete<T>(Entity id)` - Remove component from entity
- `T& Get<T>(Entity id)` - Get component reference
- `bool Has<T>(Entity id)` - Check if entity has component
- `SimpleView<Components...> View<Components...>()` - Create component view
- `T& GetResourse<T>()` - Access engine resource

#### Transform
- `glm::vec3 position` - World position
- `glm::quat rotation` - Rotation quaternion
- `glm::vec3 scale` - Scale factors

#### Sprite
- `uint32_t textureID` - Texture identifier
- `uint32_t width, height` - Sprite dimensions
- `glm::vec4 color` - Tint color
- `static Sprite Create(string name, TextureManager& manager)` - Factory method

### Input System

#### InputManager
- `bool IsKeyPressed(int keycode)` - Check if key is currently held
- `bool IsKeyDown(int keycode)` - Check if key was just pressed
- `bool IsKeyUp(int keycode)` - Check if key was just released
- `bool IsMouseButtonPressed(int button)` - Check mouse button state

### Key Codes
Common key codes defined in the engine:
- `AC_KEY_W`, `AC_KEY_A`, `AC_KEY_S`, `AC_KEY_D` - WASD keys
- `AC_KEY_SPACE` - Space bar
- `AC_KEY_ENTER` - Enter key
- `AC_KEY_UP`, `AC_KEY_DOWN`, `AC_KEY_LEFT`, `AC_KEY_RIGHT` - Arrow keys

## Building and Running

1. Open the solution in Visual Studio 2022
2. Set the SandBox project as startup project
3. Build the solution (F7)
4. Run the application (F5)

### Project Structure

```
GameEngine/
©À©¤©¤ Achoium/              # Core engine code
©¦   ©À©¤©¤ Core/             # ECS implementation
©¦   ©À©¤©¤ Event/            # Event system
©¦   ©À©¤©¤ Render/           # Rendering systems
©¦   ©À©¤©¤ Input/            # Input management
©¦   ©À©¤©¤ AssetManagement/  # Asset managers
©¦   ©¸©¤©¤ EngineSystems/    # Built-in systems
©À©¤©¤ SandBox/              # Example application
©¦   ©À©¤©¤ Assets/           # Game assets
©¦   ©¸©¤©¤ GameComponents/   # Custom components
©¸©¤©¤ Dependency/           # Third-party libraries
```

## Contributing

When contributing to the Achoium Engine:

1. Follow the existing code style and conventions
2. Add appropriate documentation for new features
3. Test thoroughly before submitting changes
4. Update this README if adding new major features
5. Ensure your code follows C++20 standards
6. All contributions should be made under the terms of the MIT License

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Third-Party Libraries

The Achoium Engine uses several third-party libraries, each with their own licenses:

- **OpenGL**: Industry standard graphics API
- **SoLoud**: Audio engine (zlib/libpng license)
- **GLM**: OpenGL Mathematics library (MIT License)
- **FreeType**: Font rendering library (FreeType License)
- **stb_image**: Image loading library (Public Domain)

Please refer to the respective library documentation and licenses for more information.

---

*This documentation covers the core features of the Achoium Game Engine. For more detailed information about specific systems or advanced usage, please refer to the source code documentation and examples in the SandBox project.*