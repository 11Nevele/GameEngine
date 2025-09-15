# ECS Architecture Guide

This document provides an in-depth look at the Entity-Component-System architecture used in the Achoium Game Engine.

## Overview

The Achoium Engine implements a pure ECS architecture where:
- **Entities** are unique identifiers (just numbers)
- **Components** are plain data structures 
- **Systems** contain all the logic and operate on components

This design provides excellent performance, cache-friendliness, and modularity.

## Core Concepts

### Entities

Entities are represented as simple integer IDs (`Entity` type). They serve as indices to link components together.

```cpp
// Entity creation
Entity player = world.CreateEntity("Player");
Entity enemy = world.CreateEntity("Enemy");
Entity projectile = world.CreateEntity(); // No name
```

**Key Features:**
- Automatic ID assignment and recycling
- Optional string tags for debugging
- Lightweight (just an integer)
- Maximum of 4,294,967,295 entities supported

### Components

Components are pure data structures containing no logic:

```cpp
// Good component - pure data
struct Velocity {
    glm::vec3 velocity;
    float maxSpeed;
};

// Bad component - contains logic
struct BadVelocity {
    glm::vec3 velocity;
    void Update(float deltaTime); // Logic belongs in systems!
};
```

**Component Registration:**
```cpp
// Explicit registration
world.RegisterType<MyComponent>();

// Automatic registration when first used
world.Add<MyComponent>(entity, MyComponent{});
```

### Systems

Systems contain all game logic and operate on entities with specific component combinations:

```cpp
void MovementSystem(World& world)
{
    // Get required resources
    Time& time = world.GetResourse<Time>();
    
    // Process all entities with Transform and Velocity
    auto movers = world.View<Transform, Velocity>();
    for (Entity entity : movers)
    {
        Transform& transform = world.Get<Transform>(entity);
        Velocity& velocity = world.Get<Velocity>(entity);
        
        transform.position += velocity.velocity * time.Delta();
    }
}
```

## Performance Characteristics

### Memory Layout

The ECS uses **SparseSet** data structures for optimal performance:

- **Dense arrays** store components contiguously in memory
- **Sparse arrays** provide O(1) entity-to-component lookups
- **Cache-friendly** iteration over components

### System Performance

Systems iterate over packed component arrays, providing:
- Excellent cache locality
- Vectorization opportunities
- Predictable memory access patterns

## Advanced Features

### Component Events

The system automatically generates events when components are added or removed:

```cpp
// Event handlers
bool OnSpriteAdded(const OnAdded<Sprite>& event)
{
    Entity entity = event.entityID;
    Sprite& sprite = event.component;
    World& world = event.world;
    
    // Initialize sprite for rendering
    return true;
}

bool OnSpriteRemoved(const OnDeleted<Sprite>& event)
{
    // Cleanup sprite resources
    return true;
}

// Register event listeners
EventManager& events = world.GetResourse<EventManager>();
events.AddListener<OnAdded<Sprite>>(OnSpriteAdded);
events.AddListener<OnDeleted<Sprite>>(OnSpriteRemoved);
```

### Views and Queries

Views provide efficient iteration over entities with specific component combinations:

```cpp
// Simple view - entities with both Transform and Sprite
auto view = world.View<Transform, Sprite>();
for (Entity entity : view)
{
    // Process entity
}

// Packed view - get all components at once
auto packedView = world.View<Transform, Velocity, Sprite>().GetPacked();
for (auto& packed : packedView)
{
    Entity entity = packed.entity;
    auto& [transform, velocity, sprite] = packed.components;
    // All components are readily available
}
```

### System Priorities

Systems execute in priority order (0-9, lower numbers first):

```cpp
// High priority systems (run first)
world.AddPreUpdateSystem(InputSystem, 0);
world.AddPreUpdateSystem(TimeSystem, 1);

// Medium priority systems
world.AddUpdateSystem(PhysicsSystem, 2);
world.AddUpdateSystem(GameLogicSystem, 3);

// Low priority systems (run last)
world.AddPostUpdateSystem(RenderSystem, 8);
world.AddPostUpdateSystem(AudioSystem, 9);
```

## Best Practices

### Component Design

1. **Keep components simple** - Only data, no methods
2. **Use appropriate data types** - Prefer value types over pointers
3. **Group related data** - Don't over-decompose components
4. **Avoid large components** - Split complex data into multiple components

```cpp
// Good: Focused, simple components
struct Position { glm::vec3 position; };
struct Velocity { glm::vec3 velocity; };
struct Health { float current, maximum; };

// Bad: Monolithic component
struct GameObject {
    glm::vec3 position, velocity, acceleration;
    float health, maxHealth, mana, maxMana;
    int level, experience;
    std::string name, description;
    // ... many more fields
};
```

### System Design

1. **Single responsibility** - Each system should have one clear purpose
2. **Avoid system dependencies** - Use events for inter-system communication
3. **Process in batches** - Operate on all relevant entities at once
4. **Use appropriate execution phase** - Pre/Update/Post as needed

```cpp
// Good: Focused system
void MovementSystem(World& world)
{
    auto movers = world.View<Transform, Velocity>();
    for (Entity entity : movers)
    {
        // Update positions based on velocities
    }
}

// Good: Event-driven communication
void HealthSystem(World& world)
{
    auto view = world.View<Health>();
    for (Entity entity : view)
    {
        Health& health = world.Get<Health>(entity);
        if (health.current <= 0)
        {
            // Send death event instead of directly handling death
            world.GetResourse<EventManager>().Invoke(
                DeathEvent{entity}, AllowToken<DeathEvent>()
            );
        }
    }
}
```

### Entity Composition

Use composition to create different entity types:

```cpp
// Player entity
Entity player = world.CreateEntity("Player");
world.Add<Transform>(player, Transform{{0,0,0}});
world.Add<Sprite>(player, Sprite::Create("PlayerSprite", textures));
world.Add<PlayerInput>(player, PlayerInput{});
world.Add<Health>(player, Health{100, 100});
world.Add<Velocity>(player, Velocity{});

// Enemy entity (different component combination)
Entity enemy = world.CreateEntity("Enemy");
world.Add<Transform>(enemy, Transform{{100,0,0}});
world.Add<Sprite>(enemy, Sprite::Create("EnemySprite", textures));
world.Add<AIController>(enemy, AIController{});
world.Add<Health>(enemy, Health{50, 50});
world.Add<Velocity>(enemy, Velocity{});

// Projectile entity (minimal components)
Entity projectile = world.CreateEntity("Projectile");
world.Add<Transform>(projectile, Transform{{0,0,0}});
world.Add<Sprite>(projectile, Sprite::Create("BulletSprite", textures));
world.Add<Velocity>(projectile, Velocity{});
world.Add<Lifetime>(projectile, Lifetime{5.0f}); // Auto-destroy after 5 seconds
```

## Debugging and Profiling

### Entity Inspection

The World class provides debug utilities:

```cpp
// Get entity count
size_t entityCount = world.GetEntityCount();

// Check what components an entity has
bool hasTransform = world.Has<Transform>(entity);
bool hasSprite = world.Has<Sprite>(entity);
bool hasBoth = world.HasAll<Transform, Sprite>(entity);
bool hasEither = world.HasAny<Transform, Sprite>(entity);
```

### Performance Monitoring

Monitor system performance by timing system execution:

```cpp
void TimedMovementSystem(World& world)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    // System logic here
    auto movers = world.View<Transform, Velocity>();
    for (Entity entity : movers)
    {
        // Process movement
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    if (duration.count() > 1000) // Log if system takes > 1ms
    {
        ACMSG("MovementSystem took " << duration.count() << " microseconds");
    }
}
```

## Migration from OOP

If you're coming from object-oriented game programming, here are the key differences:

### OOP Approach
```cpp
class GameObject {
public:
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
protected:
    Transform transform;
};

class Player : public GameObject {
public:
    void Update(float deltaTime) override {
        // Player-specific logic
    }
    void Render() override {
        // Player-specific rendering
    }
private:
    PlayerInput input;
    Health health;
    Sprite sprite;
};
```

### ECS Approach
```cpp
// Components (data only)
struct Transform { /* ... */ };
struct PlayerInput { /* ... */ };
struct Health { /* ... */ };
struct Sprite { /* ... */ };

// Systems (logic only)
void PlayerInputSystem(World& world) {
    auto players = world.View<Transform, PlayerInput>();
    // Process all player input
}

void HealthSystem(World& world) {
    auto entities = world.View<Health>();
    // Process all health logic
}

void RenderSystem(World& world) {
    auto renderables = world.View<Transform, Sprite>();
    // Render all sprites
}
```

**Key Benefits of ECS:**
- Better performance through data locality
- More flexible entity composition
- Easier to add/remove behaviors
- Better parallelization opportunities
- Cleaner separation of concerns

## Conclusion

The ECS architecture in Achoium provides a powerful, flexible, and performant foundation for game development. By following the principles of data-oriented design and keeping logic separate from data, you can build complex games that scale well and are easy to maintain.

Remember: **Entities are IDs, Components are data, Systems are logic.**