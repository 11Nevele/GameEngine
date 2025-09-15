# GameEngine

A powerful and flexible game engine built in C++ with support for both 2D and 3D game development.

![GameEngine Logo](https://via.placeholder.com/800x200?text=GameEngine)

## Overview

GameEngine is a C++ based game development framework designed to provide developers with a robust foundation for creating both 2D and 3D games. Built with performance and flexibility in mind, this engine leverages modern C++ practices and industry-standard libraries to deliver a seamless development experience.

## Features

- **Dual-Mode Support**: Develop both 2D and 3D games using a unified API
- **Entity Component System (ECS)**: Efficient and flexible game object management
- **Event Management System**: Robust event handling and propagation
- **2D Physics**: Integrated physics system for realistic 2D interactions
- **Cross-Platform Compatibility**: Core functionality designed with portability in mind

## Technology Stack

GameEngine is built on top of several powerful open-source technologies:

- **OpenGL**: Hardware-accelerated rendering
- **GLFW**: Window management and input handling
- **GLM (OpenGL Mathematics)**: Mathematical operations optimized for graphics
- **SoLoud**: Audio engine for sound effects and music
- **FreeType**: Font rendering system

## Technical Details

### Entity Component System (ECS)

The engine uses a modern ECS architecture that separates data from logic:
- **Entities**: Unique identifiers, in this case uint64, representing game objects
- **Components**: Pure data containers attached to entities
- **Systems**: Pure logic that handels the interaction between components

### Event Manager

The event system allows for decoupled communication between different parts of the game:
- Event creation, subscription, and dispatch
- Priority-based event handling
- Currently only support for both immediate events
- Any class can be an event
- Permission dispatch event is managed by ALLOWTOKEN

### 2D Physics System

The integrated physics system provides:
- Collision detection and resolution
- Impulsed based
- Rigid body dynamics
- Various collision shapes (circles, rectangles, polygons)
- Physics material properties

## Deployment

### Current Requirements

- **Visual Studio**: The engine currently only supports development through Visual Studio
- Manual conversion is required for other IDEs or development environments

### Planned Support

- Additional IDE support coming in future updates
- Build system improvements for easier cross-platform development

## API Reference

GameEngine provides a comprehensive API for game development. For detailed documentation:
- Review the code comments throughout the repository
- Check example implementations in the codebase
- Refer to specific component documentation in their respective directories

## Getting Started

1. **Clone the repository**
   ```
   git clone https://github.com/11Nevele/GameEngine.git
   ```

2. **Open in Visual Studio**
   - Open the solution file in Visual Studio
   - Build the solution to generate the engine library

3. **Create a new game project**
   - Set up a new project that references the engine
   - Include the necessary headers
   - Initialize the engine and start building your game

## Examples

```cpp
#include "Achoium.h"
using namespace ac;

World world;

struct Movement
{
    glm::vec3 spd;
};

void MovementSystem(World& world)
{
    InputManager input = world.GetResource<InputManager>();
    world.View<Movement>([](World& world)
    {
      
    });
}

int main() {
    // Initialize the engine
    InitEngine(world);

    Entity camera = world.CreateEntity();
  	world.Add<Camera>(camera, Camera{ });
  	world.Add<AudioListener>(camera, AudioListener());
  	world.Add<Transform>(camera, Transform());
  	world.Get<Transform>(camera).position = { 0, 0, 0 };
    
    Entity TestObject = world.CreateEntity();
    world.Add<Transform>(TestObject, Transform());
    world.Add<S
    
    while (true)
  	{
  		WinWindow& win = world.GetResourse<WinWindow>();
  		
  		OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
  
  		world.Update();
  		if (exitgame)
  			break;
  
  		win.OnUpdate();
  		glClearColor(0.1, 0.1, 0.1, 1);
  		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	}
    
    return 0;
}
```

## Contributing

Contributions to GameEngine are welcome! Feel free to fork the repository, make improvements, and submit pull requests.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
