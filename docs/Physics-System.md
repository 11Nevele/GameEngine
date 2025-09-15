# Physics System Documentation

The Achoium Engine includes comprehensive physics simulation supporting both 2D and 3D physics with collision detection, rigid body dynamics, and spatial optimization.

## Overview

The physics system is built on a component-based architecture with the following key components:
- **RigidBody/RigidBody2D**: Mass, velocity, and force properties
- **Colliders**: Shape definitions for collision detection
- **Transform**: Position and orientation in world space

## 3D Physics

### RigidBody Component

The `RigidBody` component handles physical properties and forces:

```cpp
struct RigidBody {
    float mass = 1.0f;                    // Object mass
    glm::vec3 velocity = {0, 0, 0};       // Current velocity
    glm::vec3 acceleration = {0, 0, 0};   // Current acceleration
    glm::vec3 force = {0, 0, 0};          // Accumulated forces
    float drag = 0.0f;                    // Air resistance
    bool useGravity = true;               // Apply gravity
    bool isKinematic = false;             // Kinematic (not affected by forces)
    bool isStatic = false;                // Static (never moves)
};

// Usage
Entity dynamicObject = world.CreateEntity("DynamicBox");
world.Add<RigidBody>(dynamicObject, RigidBody{
    .mass = 2.0f,
    .useGravity = true,
    .drag = 0.1f
});
```

### 3D Colliders

#### BoxCollider
```cpp
struct BoxCollider : public Collider {
    glm::vec3 size = {1, 1, 1};           // Box dimensions
    glm::vec3 center = {0, 0, 0};         // Local center offset
};

// Create a box collider
BoxCollider boxCollider;
boxCollider.size = {2.0f, 1.0f, 1.0f};
boxCollider.isTrigger = false;
world.Add<BoxCollider>(entity, boxCollider);
```

#### SphereCollider
```cpp
struct SphereCollider : public Collider {
    float radius = 0.5f;                  // Sphere radius
    glm::vec3 center = {0, 0, 0};         // Local center offset
};

// Create a sphere collider
SphereCollider sphereCollider;
sphereCollider.radius = 1.5f;
world.Add<SphereCollider>(entity, sphereCollider);
```

### Force Application

Apply forces to rigid bodies for realistic physics:

```cpp
void ApplyForceToEntity(World& world, Entity entity, const glm::vec3& force)
{
    if (world.Has<RigidBody>(entity))
    {
        RigidBody& rb = world.Get<RigidBody>(entity);
        rb.force += force;
    }
}

// Example: Apply jump force
ApplyForceToEntity(world, player, {0, 500, 0}); // Upward force

// Example: Apply wind effect
void WindSystem(World& world)
{
    glm::vec3 windForce = {10, 0, 0}; // Wind blowing right
    
    auto entities = world.View<RigidBody, Transform>();
    for (Entity entity : entities)
    {
        RigidBody& rb = world.Get<RigidBody>(entity);
        if (!rb.isStatic && !rb.isKinematic)
        {
            rb.force += windForce;
        }
    }
}
```

## 2D Physics

### RigidBody2D Component

Similar to 3D but optimized for 2D simulation:

```cpp
struct RigidBody2D {
    float mass = 1.0f;
    glm::vec2 velocity = {0, 0};
    glm::vec2 acceleration = {0, 0};
    glm::vec2 force = {0, 0};
    float angularVelocity = 0.0f;         // Rotation speed
    float angularDrag = 0.0f;             // Rotational damping
    float drag = 0.0f;
    float gravityScale = 1.0f;            // Gravity multiplier
    bool useGravity = true;
    bool isKinematic = false;
    bool isStatic = false;
};
```

### 2D Colliders

#### CircleCollider2D
```cpp
struct CircleCollider2D : public Collider {
    float radius = 0.5f;
    glm::vec2 center = {0, 0};
};

// Usage
CircleCollider2D circleCollider;
circleCollider.radius = 32.0f; // 32 pixel radius
world.Add<CircleCollider2D>(entity, circleCollider);
```

#### RectCollider2D
```cpp
struct RectCollider2D : public Collider {
    glm::vec2 size = {1, 1};
    glm::vec2 center = {0, 0};
};

// Usage
RectCollider2D rectCollider;
rectCollider.size = {64, 32}; // 64x32 pixel rectangle
world.Add<RectCollider2D>(entity, rectCollider);
```

#### PolygonCollider2D
```cpp
struct PolygonCollider2D : public Collider {
    std::vector<glm::vec2> points;        // Polygon vertices
    glm::vec2 center = {0, 0};
};

// Create a triangle collider
PolygonCollider2D triangleCollider;
triangleCollider.points = {
    {0, 1},     // Top
    {-1, -1},   // Bottom left
    {1, -1}     // Bottom right
};
world.Add<PolygonCollider2D>(entity, triangleCollider);
```

## Collision Detection

### Collision Events

The physics system generates collision events automatically:

```cpp
// Define collision event handler
bool OnCollisionEnter(const OnCollision& collision)
{
    Entity entityA = collision.data.entityA;
    Entity entityB = collision.data.entityB;
    
    ACMSG("Collision between entity " << entityA << " and " << entityB);
    
    // Handle collision logic
    if (IsPlayer(entityA) && IsEnemy(entityB))
    {
        // Player hit enemy
        ApplyDamage(entityA, 10);
    }
    
    return true;
}

// Register collision handler
EventManager& events = world.GetResourse<EventManager>();
events.AddListener<OnCollision>(OnCollisionEnter);
```

### Trigger Volumes

Use triggers for non-physical collision detection:

```cpp
// Create a trigger zone
Entity triggerZone = world.CreateEntity("TriggerZone");
world.Add<Transform>(triggerZone, Transform{{100, 100, 0}});

BoxCollider triggerCollider;
triggerCollider.isTrigger = true; // Make it a trigger
triggerCollider.size = {50, 50, 50};
world.Add<BoxCollider>(triggerZone, triggerCollider);

// Handle trigger events
bool OnTriggerEnter(const OnTriggerEnter& trigger)
{
    Entity triggerEntity = trigger.data.triggerEntity;
    Entity otherEntity = trigger.data.otherEntity;
    
    // Player entered pickup zone
    if (IsPickupZone(triggerEntity) && IsPlayer(otherEntity))
    {
        CollectItem(otherEntity);
    }
    
    return true;
}
```

## Physics Systems

### Built-in Physics Systems

The engine automatically registers these physics systems:

1. **PhysicsStep** (Priority 1): Updates 3D rigid body physics
2. **CollisionSystem** (Priority 2): Detects and resolves 3D collisions
3. **Physics2DStep** (Priority 1): Updates 2D rigid body physics
4. **Collision2DSystem** (Priority 2): Detects and resolves 2D collisions

### Custom Physics Systems

Create custom physics behavior by implementing your own systems:

```cpp
void GravityWellSystem(World& world)
{
    // Find all gravity wells
    auto gravityWells = world.View<Transform, GravityWell>();
    auto rigidbodies = world.View<Transform, RigidBody>();
    
    for (Entity wellEntity : gravityWells)
    {
        Transform& wellTransform = world.Get<Transform>(wellEntity);
        GravityWell& well = world.Get<GravityWell>(wellEntity);
        
        for (Entity rbEntity : rigidbodies)
        {
            Transform& rbTransform = world.Get<Transform>(rbEntity);
            RigidBody& rb = world.Get<RigidBody>(rbEntity);
            
            if (rb.isStatic) continue;
            
            // Calculate gravitational force
            glm::vec3 direction = wellTransform.position - rbTransform.position;
            float distance = glm::length(direction);
            
            if (distance > 0 && distance < well.radius)
            {
                direction = glm::normalize(direction);
                float force = (well.strength * rb.mass) / (distance * distance);
                rb.force += direction * force;
            }
        }
    }
}

// Register custom system
world.AddUpdateSystem(GravityWellSystem, 0); // Run before built-in physics
```

## Collision Layers

Use collision layers to control what objects can collide:

```cpp
// Define collision layers
enum CollisionLayers {
    LAYER_DEFAULT = 0,
    LAYER_PLAYER = 1,
    LAYER_ENEMY = 2,
    LAYER_PROJECTILE = 3,
    LAYER_ENVIRONMENT = 4
};

// Set collision layer for entity
Collider& collider = world.Get<BoxCollider>(entity);
collider.layer = LAYER_PLAYER;

// Configure layer interactions in CollisionLayer resource
CollisionLayer& collisionLayer = world.GetResourse<CollisionLayer>();
collisionLayer.SetLayerCollision(LAYER_PLAYER, LAYER_ENEMY, true);      // Players collide with enemies
collisionLayer.SetLayerCollision(LAYER_PROJECTILE, LAYER_PLAYER, false); // Projectiles don't hit their owner
```

## Performance Optimization

### Spatial Partitioning

For large numbers of objects, consider implementing spatial partitioning:

```cpp
// Custom spatial partitioning system
void SpatialPartitioningSystem(World& world)
{
    static SpatialGrid grid(1000, 1000, 50); // 1000x1000 world, 50 unit cells
    grid.Clear();
    
    // Add all colliders to spatial grid
    auto colliders = world.View<Transform, BoxCollider>();
    for (Entity entity : colliders)
    {
        Transform& transform = world.Get<Transform>(entity);
        grid.AddEntity(entity, transform.position);
    }
    
    // Only check collisions between nearby objects
    for (Entity entity : colliders)
    {
        auto nearby = grid.GetNearbyEntities(entity);
        for (Entity other : nearby)
        {
            if (entity != other)
            {
                CheckCollision(world, entity, other);
            }
        }
    }
}
```

### Sleeping Bodies

Automatically put idle bodies to sleep to save computation:

```cpp
struct SleepingBody {
    float sleepTimer = 0.0f;
    bool isSleeping = false;
    float sleepThreshold = 0.1f;    // Velocity threshold for sleeping
    float sleepTime = 2.0f;         // Time before sleeping
};

void SleepingSystem(World& world)
{
    Time& time = world.GetResourse<Time>();
    
    auto sleepingBodies = world.View<RigidBody, SleepingBody>();
    for (Entity entity : sleepingBodies)
    {
        RigidBody& rb = world.Get<RigidBody>(entity);
        SleepingBody& sleep = world.Get<SleepingBody>(entity);
        
        float speed = glm::length(rb.velocity);
        
        if (speed < sleep.sleepThreshold)
        {
            sleep.sleepTimer += time.Delta();
            if (sleep.sleepTimer > sleep.sleepTime)
            {
                sleep.isSleeping = true;
                rb.velocity = {0, 0, 0}; // Stop the body
            }
        }
        else
        {
            sleep.sleepTimer = 0.0f;
            sleep.isSleeping = false;
        }
    }
}
```

## Integration Examples

### Character Controller

```cpp
struct CharacterController {
    float moveSpeed = 5.0f;
    float jumpForce = 400.0f;
    bool isGrounded = false;
};

void CharacterControllerSystem(World& world)
{
    InputManager& input = world.GetResourse<InputManager>();
    
    auto characters = world.View<Transform, RigidBody, CharacterController>();
    for (Entity entity : characters)
    {
        RigidBody& rb = world.Get<RigidBody>(entity);
        CharacterController& controller = world.Get<CharacterController>(entity);
        
        // Movement
        glm::vec3 movement = {0, 0, 0};
        if (input.IsKeyPressed(AC_KEY_A)) movement.x -= 1;
        if (input.IsKeyPressed(AC_KEY_D)) movement.x += 1;
        if (input.IsKeyPressed(AC_KEY_W)) movement.z -= 1;
        if (input.IsKeyPressed(AC_KEY_S)) movement.z += 1;
        
        if (glm::length(movement) > 0)
        {
            movement = glm::normalize(movement) * controller.moveSpeed;
            rb.velocity.x = movement.x;
            rb.velocity.z = movement.z;
        }
        
        // Jump
        if (input.IsKeyDown(AC_KEY_SPACE) && controller.isGrounded)
        {
            rb.force.y += controller.jumpForce;
            controller.isGrounded = false;
        }
    }
}
```

### Projectile Physics

```cpp
struct Projectile {
    float lifetime = 5.0f;
    float damage = 25.0f;
    Entity owner;
};

void ProjectileSystem(World& world)
{
    Time& time = world.GetResourse<Time>();
    
    auto projectiles = world.View<Projectile>();
    for (Entity entity : projectiles)
    {
        Projectile& projectile = world.Get<Projectile>(entity);
        
        projectile.lifetime -= time.Delta();
        if (projectile.lifetime <= 0)
        {
            world.DeleteEntity(entity);
        }
    }
}

// Create projectile
Entity CreateProjectile(World& world, glm::vec3 position, glm::vec3 velocity, Entity owner)
{
    Entity projectile = world.CreateEntity("Projectile");
    
    world.Add<Transform>(projectile, Transform{position});
    world.Add<RigidBody>(projectile, RigidBody{
        .mass = 0.1f,
        .velocity = velocity,
        .useGravity = false
    });
    world.Add<SphereCollider>(projectile, SphereCollider{.radius = 0.1f});
    world.Add<Projectile>(projectile, Projectile{.owner = owner});
    
    return projectile;
}
```

## Debugging Physics

### Debug Visualization

```cpp
void PhysicsDebugSystem(World& world)
{
    auto& renderer = world.GetResourse<OpenGLRenderer>();
    
    // Draw collider outlines
    auto boxColliders = world.View<Transform, BoxCollider>();
    for (Entity entity : boxColliders)
    {
        Transform& transform = world.Get<Transform>(entity);
        BoxCollider& collider = world.Get<BoxCollider>(entity);
        
        // Draw wireframe box
        DrawWireframeBox(renderer, transform.position, collider.size, {0, 1, 0, 1});
    }
    
    // Draw velocity vectors
    auto rigidbodies = world.View<Transform, RigidBody>();
    for (Entity entity : rigidbodies)
    {
        Transform& transform = world.Get<Transform>(entity);
        RigidBody& rb = world.Get<RigidBody>(entity);
        
        if (glm::length(rb.velocity) > 0.1f)
        {
            DrawLine(renderer, transform.position, 
                    transform.position + rb.velocity, {1, 0, 0, 1});
        }
    }
}
```

### Physics Profiling

```cpp
void ProfilePhysics()
{
    static int frameCount = 0;
    static float totalPhysicsTime = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Run physics systems here
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    totalPhysicsTime += duration.count();
    frameCount++;
    
    if (frameCount >= 60) // Every 60 frames
    {
        float avgTime = totalPhysicsTime / frameCount;
        ACMSG("Average physics time: " << avgTime << " microseconds");
        
        frameCount = 0;
        totalPhysicsTime = 0;
    }
}
```

The Achoium physics system provides a solid foundation for both 2D and 3D physics simulation with good performance characteristics and flexibility for custom behavior.