#include "acpch.h"
#include "TestPhysics.h"
#include "Achoium.h"
#include "EngineComponents/Physics/Physics.h"
#include "EngineSystems/PhysicsSystem.h"
#include "EngineSystems/TimeSystem.h"
#include "Math/Transform.h"

namespace ac
{
    // Helper function to create a world with physics systems
    World CreatePhysicsWorld()
    {
        World world;
        world.AddResource<EventManager>(new EventManager());
        world.AddResource<Time>(new Time());
        world.AddPreUpdateSystem(UpdateTimeSystem, 0);
        world.AddUpdateSystem(PhysicsSystem::PhysicsStep, 0);
        world.AddUpdateSystem(PhysicsSystem::CollisionSystem, 1);
        world.RegisterType<BoxCollider>();
        world.RegisterType<SphereCollider>();
        world.RegisterType<Transform>();
        world.RegisterType<Collider>();
        world.RegisterType<RigidBody>();
        return world;
    }

    // Helper function to create a world with 2D physics systems
    World Create2DPhysicsWorld()
    {
        World world;
        world.AddResource<EventManager>(new EventManager());
        world.AddResource<Time>(new Time());
        world.AddResource<CollisionLayer>(new CollisionLayer());
        world.AddPreUpdateSystem(UpdateTimeSystem, 0);
        world.AddUpdateSystem(PhysicsSystem::Physics2DStep, 0);
        world.AddUpdateSystem(PhysicsSystem::Collision2DSystem, 1);
        world.RegisterType<CircleCollider2D>();
        world.RegisterType<RectCollider2D>();
        world.RegisterType<PolygonCollider2D>();
        world.RegisterType<RigidBody2D>();
        world.RegisterType<Transform>();
        return world;
    }

    // Track collision events for testing
    struct CollisionTracker
    {
        bool collisionDetected = false;
        Entity entityA = NULL_ENTITY;
        Entity entityB = NULL_ENTITY;
        float penetrationDepth = 0.0f;

        void Reset()
        {
            collisionDetected = false;
            entityA = NULL_ENTITY;
            entityB = NULL_ENTITY;
            penetrationDepth = 0.0f;
        }
    };

    CollisionTracker collisionTracker;

    // Collision event handler for tests
    bool TestCollisionHandler(const OnCollision& collision)
    {
        collisionTracker.collisionDetected = true;
        collisionTracker.entityA = collision.data.entityA;
        collisionTracker.entityB = collision.data.entityB;
        collisionTracker.penetrationDepth = collision.data.penetrationDepth;
        return true;
    }

    // Trigger event handler for tests
    bool TestTriggerHandler(const OnTriggerEnter& trigger)
    {
        collisionTracker.collisionDetected = true;
        collisionTracker.entityA = trigger.data.entityA;
        collisionTracker.entityB = trigger.data.entityB;
        return true;
    }

    void TestRigidBodyProperties()
    {
        ACMSG("Testing RigidBody properties...");

        // Test default constructor
        RigidBody rb1;
        ACASSERT(rb1.mass == 1.0f, "Default mass should be 1.0");
        ACASSERT(rb1.inverseMass == 1.0f, "Default inverse mass should be 1.0");
        ACASSERT(glm::length(rb1.velocity) == 0.0f, "Default velocity should be zero");
        ACASSERT(rb1.useGravity == true, "Default useGravity should be true");
        ACASSERT(rb1.isKinematic == false, "Default isKinematic should be false");

        // Test parametrized constructor
        RigidBody rb2(5.0f);
        ACASSERT(rb2.mass == 5.0f, "Mass not set correctly");
        ACASSERT(rb2.inverseMass == 0.2f, "Inverse mass not calculated correctly");

        // Test static rigidbody
        RigidBody rb3(0.0f);
        ACASSERT(rb3.mass == 0.0f, "Static body mass should be 0");
        ACASSERT(rb3.inverseMass == 0.0f, "Static body inverse mass should be 0");

        ACMSG("RigidBody properties test passed");
    }

    void TestRigidBodyForces()
    {
        ACMSG("Testing RigidBody force application...");

        World world = CreatePhysicsWorld();

        // Create rigidbody entity
        Entity entity = world.CreateEntity("TestRigidbody");
        world.Add<Transform>(entity, Transform());
        world.Add<RigidBody>(entity, RigidBody(2.0f));

        Transform& transform = world.Get<Transform>(entity);
        RigidBody& rb = world.Get<RigidBody>(entity);

        // Test force application
        rb.ApplyForce(glm::vec3(10.0f, 0.0f, 0.0f));
        ACASSERT(rb.force.x == 10.0f, "Force not applied correctly");

        // Run physics step and check velocity update
        Sleep(16);
        world.Update();

        // Force should be applied as acceleration (F=ma, a=F/m)
        // With fixed timestep of 0.016s, velocity change should be F/m * dt
        ACASSERT(glm::abs(rb.velocity.x - 0.08f) < 0.001f,
            "Velocity not updated correctly from force");

        // Force should be reset after physics step
        ACASSERT(glm::length(rb.force) == 0.0f, "Force not reset after physics step");

        // Run another step to check position update
        Sleep(16);
        world.Update();

        // Check that position changed based on velocity
        ACASSERT(transform.position.x > 0.0f, "Position not updated by velocity");

        // Test impulse application
        rb.ApplyImpulse(glm::vec3(0.0f, 2.0f, 0.0f));

        // Impulse directly changes velocity (J = m*v, v = J/m)
        ACASSERT(glm::abs(rb.velocity.y - 1.0f) < 0.001f,
            "Velocity not updated correctly from impulse");

        ACMSG("RigidBody force application test passed");
    }

    void TestColliderBase()
    {
        ACMSG("Testing Collider base functionality...");

        // Create a box collider to test base functionality
        BoxCollider collider;
        Transform transform;
        transform.position = glm::vec3(1.0f, 2.0f, 3.0f);

        // Test default collider properties
        ACASSERT(collider.isTrigger == false, "Default isTrigger should be false");
        ACASSERT(glm::length(collider.offset) == 0.0f, "Default offset should be zero");

        // Test world position calculation
        glm::vec3 worldPos = collider.GetWorldPosition(transform);
        ACASSERT(glm::distance(worldPos, transform.position) < 0.001f,
            "World position doesn't match transform position");

        // Test with offset
        collider.offset = glm::vec3(0.0f, 1.0f, 0.0f);
        worldPos = collider.GetWorldPosition(transform);
        ACASSERT(glm::distance(worldPos, glm::vec3(1.0f, 3.0f, 3.0f)) < 0.001f,
            "World position with offset not calculated correctly");

        ACMSG("Collider base functionality test passed");
    }

    void TestBoxCollider()
    {
        ACMSG("Testing BoxCollider...");

        World world = CreatePhysicsWorld();
        collisionTracker.Reset();
        world.GetResourse<EventManager>().AddListener<OnCollision>(TestCollisionHandler);

        // Create two box entities
        Entity box1 = world.CreateEntity("Box1");
        Entity box2 = world.CreateEntity("Box2");

        world.Add<Transform>(box1, Transform());
        world.Add<Transform>(box2, Transform());

        Transform& transform1 = world.Get<Transform>(box1);
        Transform& transform2 = world.Get<Transform>(box2);
        // Position boxes so they overlap
        transform1.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform2.position = glm::vec3(0.9f, 0.0f, 0.0f);

        // Add box colliders
        world.Add<BoxCollider>(box1, BoxCollider(1.0f, 1.0f, 1.0f));
        world.Add<BoxCollider>(box2, BoxCollider(1.0f, 1.0f, 1.0f));

        BoxCollider& collider1 = world.Get<BoxCollider>(box1);
        BoxCollider& collider2 = world.Get<BoxCollider>(box2);
        // Test collision detection
        glm::vec3 collisionPoint, collisionNormal;
        float penetrationDepth;
        bool collides = collider1.CheckCollision(&collider2, transform1, transform2, 
                                               collisionPoint, collisionNormal, penetrationDepth);
        
        ACASSERT(collides, "Box-Box collision not detected");
        ACASSERT(penetrationDepth > 0.0f, "Penetration depth should be positive");
        ACASSERT(glm::length(collisionNormal) > 0.99f, "Collision normal should be normalized");
        
        // Test collision system integration
        Sleep(16);
        world.Update();
        
        ACASSERT(collisionTracker.collisionDetected, "Collision event not triggered");
        ACASSERT(collisionTracker.entityA == box1, "Collision entityA incorrect");
        ACASSERT(collisionTracker.entityB == box2, "Collision entityB incorrect");
        
        // Test non-overlapping boxes
        collisionTracker.Reset();
        transform2.position = glm::vec3(2.0f, 0.0f, 0.0f);
        
        collides = collider1.CheckCollision(&collider2, transform1, transform2, 
                                          collisionPoint, collisionNormal, penetrationDepth);
        
        ACASSERT(!collides, "Non-overlapping boxes incorrectly report collision");
        
        Sleep(16);
        world.Update();
        ACASSERT(!collisionTracker.collisionDetected, "Collision event incorrectly triggered");
        
        ACMSG("BoxCollider test passed");
    }
    
    void TestSphereCollider()
    {
        ACMSG("Testing SphereCollider...");
        
        World world = CreatePhysicsWorld();
        collisionTracker.Reset();
        world.GetResourse<EventManager>().AddListener<OnCollision>(TestCollisionHandler);
        
        // Create two sphere entities
        Entity sphere1 = world.CreateEntity("Sphere1");
        Entity sphere2 = world.CreateEntity("Sphere2");
        
        world.Add<Transform>(sphere1, Transform());
        world.Add<Transform>(sphere2, Transform());
        
        // Position spheres so they overlap
        world.Get<Transform>(sphere1).position = glm::vec3(0.0f, 0.0f, 0.0f);
        world.Get<Transform>(sphere2).position = glm::vec3(0.9f, 0.0f, 0.0f);
        
        // Add sphere colliders with radius 0.5
        world.Add<SphereCollider>(sphere1, SphereCollider(0.5f));
        world.Add<SphereCollider>(sphere2, SphereCollider(0.5f));
        
        // Test collision detection
        glm::vec3 collisionPoint, collisionNormal;
        float penetrationDepth;
        bool collides = world.Get<SphereCollider>(sphere1).CheckCollision(&world.Get<SphereCollider>(sphere2), world.Get<Transform>(sphere1), world.Get<Transform>(sphere2),
                                               collisionPoint, collisionNormal, penetrationDepth);
        
        ACASSERT(collides, "Sphere-Sphere collision not detected");
        ACASSERT(penetrationDepth > 0.0f, "Penetration depth should be positive");
        ACASSERT(glm::length(collisionNormal) > 0.99f, "Collision normal should be normalized");
        
        // Test with different radii
        world.Delete<SphereCollider>(sphere2);
        world.Add<SphereCollider>(sphere2, SphereCollider(1.0f));
        

        collides = world.Get<SphereCollider>(sphere1).CheckCollision(&world.Get<SphereCollider>(sphere2), world.Get<Transform>(sphere1), world.Get<Transform>(sphere2),
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collides, "Sphere-Sphere collision with different radii not detected");
        ACASSERT(penetrationDepth > 0.0f, "Penetration depth should be positive");
        
        // Test non-overlapping spheres
        world.Get<Transform>(sphere2).position = glm::vec3(2.0f, 0.0f, 0.0f);
        
        collides = world.Get<SphereCollider>(sphere1).CheckCollision(&world.Get<SphereCollider>(sphere2), world.Get<Transform>(sphere1), world.Get<Transform>(sphere2),
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(!collides, "Non-overlapping spheres incorrectly report collision");
        
        ACMSG("SphereCollider test passed");
    }
    
    void TestMixedCollisions()
    {
        ACMSG("Testing mixed collider types...");
        
        World world = CreatePhysicsWorld();
        
        // Create box and sphere entities
        Entity boxEntity = world.CreateEntity("Box");
        Entity sphereEntity = world.CreateEntity("Sphere");
        
        world.Add<Transform>(boxEntity, Transform());
        world.Add<Transform>(sphereEntity, Transform());
        
        
        world.Add<BoxCollider>(boxEntity, BoxCollider(1.0f, 1.0f, 1.0f));
        world.Add<SphereCollider>(sphereEntity, SphereCollider(0.5f));
        
        Transform& boxTransform = world.Get<Transform>(boxEntity);
        Transform& sphereTransform = world.Get<Transform>(sphereEntity);
        // Position for collision
        boxTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        sphereTransform.position = glm::vec3(0.9f, 0.0f, 0.0f);

        BoxCollider& boxCollider = world.Get<BoxCollider>(boxEntity);
        SphereCollider& sphereCollider = world.Get<SphereCollider>(sphereEntity);

        // Test box-sphere collision
        glm::vec3 collisionPoint, collisionNormal;
        float penetrationDepth;
        
        bool collides = boxCollider.CheckCollision(&sphereCollider, boxTransform, sphereTransform, 
                                                 collisionPoint, collisionNormal, penetrationDepth);
        
        ACASSERT(collides, "Box-Sphere collision not detected");
        
        // Test sphere-box collision (reverse)
        bool collidesReverse = sphereCollider.CheckCollision(&boxCollider, sphereTransform, boxTransform, 
                                                           collisionPoint, collisionNormal, penetrationDepth);
        
        ACASSERT(collidesReverse, "Sphere-Box collision not detected");
        
        // Test non-overlapping case
        sphereTransform.position = glm::vec3(2.0f, 0.0f, 0.0f);
        
        collides = boxCollider.CheckCollision(&sphereCollider, boxTransform, sphereTransform, 
                                            collisionPoint, collisionNormal, penetrationDepth);
        
        ACASSERT(!collides, "Non-overlapping box-sphere incorrectly reports collision");
        
        ACMSG("Mixed collisions test passed");
    }
    
    void TestPhysicsSystem()
    {
        ACMSG("Testing Physics System...");

        World world = CreatePhysicsWorld();

        // Create an entity with rigidbody
        Entity entity = world.CreateEntity("PhysicsObject");
        world.Add<Transform>(entity, Transform());
        world.Add<RigidBody>(entity, RigidBody(1.0f));

        Transform& transform = world.Get<Transform>(entity);
        RigidBody& rb = world.Get<RigidBody>(entity);
        // Set initial values
        transform.position = glm::vec3(0.0f, 10.0f, 0.0f);
        rb.velocity = glm::vec3(1.0f, 0.0f, 0.0f);

        // Store initial position
        glm::vec3 initialPos = transform.position;

        // Run physics step
        Sleep(16);
        world.Update();

        // Check horizontal movement from velocity
        ACASSERT(transform.position.x > initialPos.x, "Position not updated by velocity");

        // Check vertical movement from gravity
        ACASSERT(transform.position.y < initialPos.y, "Gravity not applied");

        // Test with disabled gravity
        rb.useGravity = false;
        initialPos = transform.position;
        Sleep(16);
        world.Update();

        // Should still move horizontally but not fall
        ACASSERT(transform.position.x > initialPos.x, "Position not updated by velocity");
        //ACASSERT(abs(transform.position.y - initialPos.y) < 0.01f, "Gravity incorrectly applied when disabled");

        // Test kinematic body
        rb.useGravity = false;
        rb.velocity = glm::vec3(0, 0, 0);
        rb.isKinematic = true;
        rb.ApplyForce(glm::vec3(100.0f, 0.0f, 0.0f));
        initialPos = transform.position;

        Sleep(16);
        world.Update();

        // Kinematic bodies should not respond to forces
        ACASSERT(glm::length(rb.velocity - glm::vec3(0.0f, 0.0f, 0.0f)) < 0.001f,
            "Kinematic body velocity incorrectly affected by force");

        ACMSG("Physics System test passed");
    }
    
    void TestCollisionResolution()
    {
        ACMSG("Testing Collision Resolution...");
        
        World world = CreatePhysicsWorld();
        collisionTracker.Reset();
        world.GetResourse<EventManager>().AddListener<OnCollision>(TestCollisionHandler);
        
        // Create two objects that will collide
        Entity entity1 = world.CreateEntity("Object1");
        Entity entity2 = world.CreateEntity("Object2");
        
        // Setup first object (moving toward second)
        world.Add<Transform>(entity1, Transform());
        
        world.Add<RigidBody>(entity1, RigidBody(1.0f));
        
        world.Add<BoxCollider>(entity1, BoxCollider(1.0f, 1.0f, 1.0f));
        
        // Setup second object (stationary)
        world.Add<Transform>(entity2, Transform());
        
        world.Add<RigidBody>(entity2, RigidBody(1.0f));
       
        world.Add<BoxCollider>(entity2, BoxCollider(1.0f, 1.0f, 1.0f));
        
        Transform& transform1 = world.Get<Transform>(entity1);
        RigidBody& rb1 = world.Get<RigidBody>(entity1);
        transform1.position = glm::vec3(-2.0f, 0.0f, 0.0f);
        rb1.useGravity = false;
        rb1.velocity = glm::vec3(1.0f, 0.0f, 0.0f);

        Transform& transform2 = world.Get<Transform>(entity2);
        RigidBody & rb2 = world.Get<RigidBody>(entity2);
        transform2.position = glm::vec3(0.0f, 0.0f, 0.0f);
        rb2.useGravity = false;
        // Run simulation until collision occurs
        float totalTime = 0.0f;
        bool collisionOccurred = false;
        
        for (int i = 0; i < 60 && !collisionOccurred; i++) // Run for up to 60 frames
        {
            world.Update();
            
            if (collisionTracker.collisionDetected) {
                collisionOccurred = true;
            }
            
			Sleep(16); // Simulate 60 FPS
        }
        
        ACASSERT(collisionOccurred, "Collision did not occur during simulation");
        
        // Check that velocities changed due to collision
        ACASSERT(rb1.velocity.x < 1.0f, "First object velocity not affected by collision");
        ACASSERT(rb2.velocity.x > 0.0f, "Second object velocity not affected by collision");
        
        // Check momentum conservation (approximately)
        float initialMomentum = 1.0f * 1.0f; // mass * velocity
        float finalMomentum = rb1.mass * rb1.velocity.x + rb2.mass * rb2.velocity.x;
        
        ACASSERT(std::abs(finalMomentum - initialMomentum) < 0.01f, 
                "Momentum not conserved in collision");
        
        ACMSG("Collision Resolution test passed");
    }
    
    void TestTriggerVolumes()
    {
        ACMSG("Testing Trigger Volumes...");

        World world = CreatePhysicsWorld();
        collisionTracker.Reset();
        world.GetResourse<EventManager>().AddListener<OnTriggerEnter>(TestTriggerHandler);

        // Create a trigger volume
        Entity triggerEntity = world.CreateEntity("TriggerVolume");
        world.Add<Transform>(triggerEntity, Transform());
        world.Add<BoxCollider>(triggerEntity, BoxCollider(1.0f, 1.0f, 1.0f));

        

        // Create an entity that will enter the trigger
        Entity movingEntity = world.CreateEntity("MovingObject");
        world.Add<Transform>(movingEntity, Transform());
        world.Add<RigidBody>(movingEntity, RigidBody(1.0f));
        world.Add<BoxCollider>(movingEntity, BoxCollider(1.0f, 1.0f, 1.0f));

        Transform& triggerTransform = world.Get<Transform>(triggerEntity);
        BoxCollider& triggerCollider = world.Get<BoxCollider>(triggerEntity);

        triggerTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        triggerCollider.isTrigger = true;

        Transform& movingTransform = world.Get<Transform>(movingEntity);
        RigidBody& rb = world.Get<RigidBody>(movingEntity);

        movingTransform.position = glm::vec3(-2.0f, 0.0f, 0.0f);
        rb.useGravity = false;
        rb.velocity = glm::vec3(1.0f, 0.0f, 0.0f);

        // Run simulation until trigger event occurs
        bool triggerEntered = false;
        for (int i = 0; i < 60 && !triggerEntered; i++) // Run for up to 60 frames
        {

            world.Update();

            if (collisionTracker.collisionDetected) {
                triggerEntered = true;
            }
            Sleep(16); // Simulate 60 FPS
        }

        ACASSERT(triggerEntered, "Trigger event not detected");

        // Object should continue moving through the trigger (no physical response)
        ACASSERT(glm::abs(rb.velocity.x - 1.0f) < 0.001f,
            "Moving object velocity incorrectly affected by trigger");

        ACMSG("Trigger Volumes test passed");
    }
    // Existing test implementations...

    void TestRigidBody2DProperties()
    {
        ACMSG("Testing RigidBody2D properties...");

        // Test default constructor
        RigidBody2D rb1;
        ACASSERT(rb1.mass == 1.0f, "Default mass should be 1.0");
        ACASSERT(rb1.inverseMass == 1.0f, "Default inverse mass should be 1.0");
        ACASSERT(glm::length(rb1.velocity) == 0.0f, "Default velocity should be zero");
        ACASSERT(rb1.angularVelocity == 0.0f, "Default angular velocity should be zero");
        ACASSERT(rb1.useGravity == true, "Default useGravity should be true");
        ACASSERT(rb1.isKinematic == false, "Default isKinematic should be false");
        ACASSERT(rb1.freezeRotation == false, "Default freezeRotation should be false");
        ACASSERT(rb1.restitution == 0.5f, "Default restitution should be 0.5");
        ACASSERT(rb1.friction == 0.3f, "Default friction should be 0.3");

        // Test parametrized constructor with mass
        RigidBody2D rb2(5.0f);
        ACASSERT(rb2.mass == 5.0f, "Mass not set correctly");
        ACASSERT(rb2.inverseMass == 0.2f, "Inverse mass not calculated correctly");

        // Test static rigidbody
        RigidBody2D rb3(0.0f);
        ACASSERT(rb3.mass == 0.0f, "Static body mass should be 0");
        ACASSERT(rb3.inverseMass == 0.0f, "Static body inverse mass should be 0");

        // Test full constructor
        RigidBody2D rb4(2.0f, 0.8f, 0.4f, false, true, true);
        ACASSERT(rb4.mass == 2.0f, "Mass not set correctly in full constructor");
        ACASSERT(rb4.restitution == 0.8f, "Restitution not set correctly in full constructor");
        ACASSERT(rb4.friction == 0.4f, "Friction not set correctly in full constructor");
        ACASSERT(rb4.useGravity == false, "useGravity not set correctly in full constructor");
        ACASSERT(rb4.isKinematic == true, "isKinematic not set correctly in full constructor");
        ACASSERT(rb4.freezeRotation == true, "freezeRotation not set correctly in full constructor");

        ACMSG("RigidBody2D properties test passed");
    }

    void TestRigidBody2DForces()
    {
        ACMSG("Testing RigidBody2D force application...");

        World world = Create2DPhysicsWorld();

        // Create rigidbody entity
        Entity entity = world.CreateEntity("TestRigidbody2D");
        world.Add<Transform>(entity, Transform());
        world.Add<RigidBody2D>(entity, RigidBody2D(2.0f));

        Transform& transform = world.Get<Transform>(entity);
        RigidBody2D& rb = world.Get<RigidBody2D>(entity);

        // Test force application
        rb.ApplyForce(glm::vec2(10.0f, 0.0f));
        ACASSERT(rb.force.x == 10.0f, "Force not applied correctly to RigidBody2D");

        // Run physics step and check velocity update
        world.Update();

        // Force should be applied as acceleration (F=ma, a=F/m)
        // With fixed timestep of 0.016s, velocity change should be F/m * dt
        ACASSERT(glm::abs(rb.velocity.x - 0.08f) < 0.001f,
            "Velocity not updated correctly from force in RigidBody2D");

        // Force should be reset after physics step
        ACASSERT(glm::length(rb.force) == 0.0f, "Force not reset after physics step in RigidBody2D");
        Sleep(16);
        // Run another step to check position update
        world.Update();

        // Check that position changed based on velocity
        ACASSERT(transform.position.x > 0.0f, "Position not updated by velocity in RigidBody2D");

        // Test impulse application
        rb.ApplyImpulse(glm::vec2(0.0f, 2.0f));

        // Impulse directly changes velocity (J = m*v, v = J/m)
        ACASSERT(glm::abs(rb.velocity.y - 1.0f) < 0.001f,
            "Velocity not updated correctly from impulse in RigidBody2D");

        // Test force at position (applying torque)
        rb.ApplyForceAtPosition(glm::vec2(0.0f, 5.0f), glm::vec2(1.0f, 0.0f));
        ACASSERT(rb.torque > 0.0f, "Torque not applied correctly in RigidBody2D");

        // Test with rotation frozen
        rb.torque = 0.0f;
        rb.freezeRotation = true;
        rb.ApplyForceAtPosition(glm::vec2(0.0f, 5.0f), glm::vec2(1.0f, 0.0f));
        ACASSERT(rb.torque == 0.0f, "Torque incorrectly applied when rotation is frozen in RigidBody2D");

        ACMSG("RigidBody2D force application test passed");
    }

    void TestCircleCollider2D()
    {
        ACMSG("Testing CircleCollider2D...");

        World world = Create2DPhysicsWorld();
        collisionTracker.Reset();
        world.GetResourse<EventManager>().AddListener<OnCollision>(TestCollisionHandler);

        // Create two circle entities
        Entity circle1 = world.CreateEntity("Circle1");
        Entity circle2 = world.CreateEntity("Circle2");

        world.Add<Transform>(circle1, Transform());
        world.Add<Transform>(circle2, Transform());

        // Position circles so they overlap
        world.Get<Transform>(circle1).position = glm::vec3(0.0f, 0.0f, 0.0f);
        world.Get<Transform>(circle2).position = glm::vec3(0.9f, 0.0f, 0.0f);

        // Add circle colliders with radius 0.5
        world.Add<CircleCollider2D>(circle1, CircleCollider2D(0.5f));
        world.Add<CircleCollider2D>(circle2, CircleCollider2D(0.5f));

        CircleCollider2D& collider1 = world.Get<CircleCollider2D>(circle1);
        CircleCollider2D& collider2 = world.Get<CircleCollider2D>(circle2);
        Transform& transform1 = world.Get<Transform>(circle1);
        Transform& transform2 = world.Get<Transform>(circle2);

        // Test collision detection
        glm::vec3 collisionPoint, collisionNormal;
        float penetrationDepth;
        bool collides = collider1.CheckCollision(&collider2, transform1, transform2,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collides, "CircleCollider2D collision not detected");
        ACASSERT(penetrationDepth > 0.0f, "CircleCollider2D penetration depth should be positive");
        ACASSERT(glm::length(collisionNormal) > 0.99f, "CircleCollider2D collision normal should be normalized");

        // Test with different radii
        world.Delete<CircleCollider2D>(circle2);
        world.Add<CircleCollider2D>(circle2, CircleCollider2D(1.0f));

        collider2 = world.Get<CircleCollider2D>(circle2);

        collides = collider1.CheckCollision(&collider2, transform1, transform2,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collides, "CircleCollider2D collision with different radii not detected");
        ACASSERT(penetrationDepth > 0.0f, "CircleCollider2D penetration depth should be positive for different radii");

        // Test non-overlapping circles
        transform2.position = glm::vec3(2.0f, 0.0f, 0.0f);

        collides = collider1.CheckCollision(&collider2, transform1, transform2,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(!collides, "Non-overlapping CircleCollider2D incorrectly reports collision");

        // Test with offset
        transform2.position = glm::vec3(1.4f, 0.0f, 0.0f);
        collider2.offset = glm::vec3(-0.5f, 0.0f, 0.0f);

        collides = collider1.CheckCollision(&collider2, transform1, transform2,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collides, "CircleCollider2D collision with offset not detected");

        ACMSG("CircleCollider2D test passed");
    }

    void TestRectCollider2D()
    {
        ACMSG("Testing RectCollider2D...");

        World world = Create2DPhysicsWorld();
        collisionTracker.Reset();
        world.GetResourse<EventManager>().AddListener<OnCollision>(TestCollisionHandler);

        // Create two rectangle entities
        Entity rect1 = world.CreateEntity("Rect1");
        Entity rect2 = world.CreateEntity("Rect2");

        world.Add<Transform>(rect1, Transform());
        world.Add<Transform>(rect2, Transform());

        Transform& transform1 = world.Get<Transform>(rect1);
        Transform& transform2 = world.Get<Transform>(rect2);

        // Position rectangles so they overlap
        transform1.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform2.position = glm::vec3(0.9f, 0.0f, 0.0f);

        // Add rectangle colliders
        world.Add<RectCollider2D>(rect1, RectCollider2D(1.0f, 1.0f));
        world.Add<RectCollider2D>(rect2, RectCollider2D(1.0f, 1.0f));

        RectCollider2D& collider1 = world.Get<RectCollider2D>(rect1);
        RectCollider2D& collider2 = world.Get<RectCollider2D>(rect2);

        // Test collision detection
        glm::vec3 collisionPoint, collisionNormal;
        float penetrationDepth;
        bool collides = collider1.CheckCollision(&collider2, transform1, transform2,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collides, "RectCollider2D collision not detected");
        ACASSERT(penetrationDepth > 0.0f, "RectCollider2D penetration depth should be positive");
        ACASSERT(glm::length(collisionNormal) > 0.99f, "RectCollider2D collision normal should be normalized");

        // Test with rotation
        transform2.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        collides = collider1.CheckCollision(&collider2, transform1, transform2,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collides, "Rotated RectCollider2D collision not detected");

        // Test non-overlapping rectangles
        transform2.position = glm::vec3(2.0f, 0.0f, 0.0f);
        transform2.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion

        collides = collider1.CheckCollision(&collider2, transform1, transform2,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(!collides, "Non-overlapping rectangles incorrectly report collision");

        // Test vertex functions
        std::vector<glm::vec2> localVertices = collider1.GetLocalVertices();
        ACASSERT(localVertices.size() == 4, "Rectangle should have 4 vertices");

        std::vector<glm::vec2> worldVertices = collider1.GetWorldVertices(transform1);
        ACASSERT(worldVertices.size() == 4, "Rectangle should have 4 world vertices");

        ACMSG("RectCollider2D test passed");
    }

    void TestMixed2DCollisions()
    {
        ACMSG("Testing mixed 2D collider types...");

        World world = Create2DPhysicsWorld();

        // Create circle and rectangle entities
        Entity circleEntity = world.CreateEntity("Circle");
        Entity rectEntity = world.CreateEntity("Rect");

        world.Add<Transform>(circleEntity, Transform());
        world.Add<Transform>(rectEntity, Transform());

        Transform& circleTransform = world.Get<Transform>(circleEntity);
        Transform& rectTransform = world.Get<Transform>(rectEntity);

        // Position for collision
        circleTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        rectTransform.position = glm::vec3(1.0f, 0.0f, 0.0f);

        // Add colliders
        world.Add<CircleCollider2D>(circleEntity, CircleCollider2D(0.6f));
        world.Add<RectCollider2D>(rectEntity, RectCollider2D(1.0f, 1.0f));

        CircleCollider2D& circleCollider = world.Get<CircleCollider2D>(circleEntity);
        RectCollider2D& rectCollider = world.Get<RectCollider2D>(rectEntity);

        // Test circle-rect collision
        glm::vec3 collisionPoint, collisionNormal;
        float penetrationDepth;

        bool collides = circleCollider.CheckCollision(&rectCollider, circleTransform, rectTransform,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collides, "Circle-Rect collision not detected");

        // Test rect-circle collision (reverse)
        bool collidesReverse = rectCollider.CheckCollision(&circleCollider, rectTransform, circleTransform,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collidesReverse, "Rect-Circle collision not detected");

        // Test non-overlapping case
        rectTransform.position = glm::vec3(2.0f, 0.0f, 0.0f);

        collides = circleCollider.CheckCollision(&rectCollider, circleTransform, rectTransform,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(!collides, "Non-overlapping Circle-Rect incorrectly reports collision");

        // Test circle against rotated rectangle
        rectTransform.position = glm::vec3(1.0f, 0.0f, 0.0f);
        rectTransform.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        collides = circleCollider.CheckCollision(&rectCollider, circleTransform, rectTransform,
            collisionPoint, collisionNormal, penetrationDepth);

        ACASSERT(collides, "Circle vs rotated Rect collision not detected");

        ACMSG("Mixed 2D collisions test passed");
    }

    void TestPhysics2DSystem()
    {
        ACMSG("Testing 2D Physics System...");

        World world = Create2DPhysicsWorld();

        // Create an entity with rigidbody2D
        Entity entity = world.CreateEntity("PhysicsObject2D");
        world.Add<Transform>(entity, Transform());
        world.Add<RigidBody2D>(entity, RigidBody2D(1.0f));

        Transform& transform = world.Get<Transform>(entity);
        RigidBody2D& rb = world.Get<RigidBody2D>(entity);

        // Set initial values
        transform.position = glm::vec3(0.0f, 10.0f, 0.0f);
        rb.velocity = glm::vec2(1.0f, 0.0f);

        // Store initial position
        glm::vec3 initialPos = transform.position;
        Sleep(16);
        // Run physics step
        world.Update();

        // Check horizontal movement from velocity
        ACASSERT(transform.position.x > initialPos.x, "Position not updated by velocity in Physics2D");

        // Check vertical movement from gravity
        ACASSERT(transform.position.y < initialPos.y, "Gravity not applied in Physics2D");

        // Test with disabled gravity
        rb.useGravity = false;
        initialPos = transform.position;
		rb.velocity = glm::vec2(1.0f, 0.0f); // Reset velocity to ensure horizontal movement

        Sleep(16);
        world.Update();

        // Should still move horizontally but not fall
        ACASSERT(transform.position.x > initialPos.x, "Position not updated by velocity with gravity disabled in Physics2D");
        ACASSERT(abs(transform.position.y - initialPos.y) < 0.01f, "Gravity incorrectly applied when disabled in Physics2D");

        // Test kinematic body
        rb.isKinematic = true;
        rb.velocity = glm::vec2(0.0f, 0.0f);
        rb.ApplyForce(glm::vec2(100.0f, 0.0f));
        initialPos = transform.position;

        Sleep(16);
        world.Update();

        // Kinematic bodies should not respond to forces
        ACASSERT(glm::length(rb.velocity) < 0.001f, "Kinematic body velocity incorrectly affected by force in Physics2D");

        // Test angular velocity
        rb.isKinematic = false;
        rb.freezeRotation = false;
        rb.angularVelocity = 1.0f;
        float initialAngle = glm::eulerAngles(transform.rotation).z;

        world.Update();

        float newAngle = glm::eulerAngles(transform.rotation).z;
        ACASSERT(newAngle != initialAngle, "Rotation not updated by angular velocity in Physics2D");

        ACMSG("2D Physics System test passed");
    }

    void TestCollision2DResolution()
    {
        ACMSG("Testing 2D Collision Resolution...");

        World world = Create2DPhysicsWorld();
        collisionTracker.Reset();
        world.GetResourse<EventManager>().AddListener<OnCollision>(TestCollisionHandler);

        // Create two objects that will collide
        Entity entity1 = world.CreateEntity("Object1-2D");
        Entity entity2 = world.CreateEntity("Object2-2D");

        // Setup first object (moving toward second)
        world.Add<Transform>(entity1, Transform());
        world.Add<RigidBody2D>(entity1, RigidBody2D(1.0f));
        world.Add<CircleCollider2D>(entity1, CircleCollider2D(0.5f));

        // Setup second object (stationary)
        world.Add<Transform>(entity2, Transform());
        world.Add<RigidBody2D>(entity2, RigidBody2D(2.0f));
        world.Add<CircleCollider2D>(entity2, CircleCollider2D(0.5f));

        Transform& transform1 = world.Get<Transform>(entity1);
        RigidBody2D& rb1 = world.Get<RigidBody2D>(entity1);
        transform1.position = glm::vec3(-2.0f, 0.0f, 0.0f);
        rb1.useGravity = false;
        rb1.velocity = glm::vec2(1.0f, 0.0f);
		rb1.restitution = 1.0f; // Default restitution

        Transform& transform2 = world.Get<Transform>(entity2);
        RigidBody2D& rb2 = world.Get<RigidBody2D>(entity2);
        transform2.position = glm::vec3(0.0f, 0.0f, 0.0f);
        rb2.useGravity = false;
		rb2.restitution = 1.0f; // Default restitution

        // Run simulation until collision occurs
        bool collisionOccurred = false;

        for (int i = 0; i < 60 && !collisionOccurred; i++) // Run for up to 60 frames
        {
            world.Update();

            if (collisionTracker.collisionDetected) {
                collisionOccurred = true;
            }
            Sleep(16);
        }

        ACASSERT(collisionOccurred, "2D Collision did not occur during simulation");

        // Check that velocities changed due to collision
        ACASSERT(rb1.velocity.x < 1.0f, "First object velocity not affected by 2D collision");
        ACASSERT(rb2.velocity.x > 0.0f, "Second object velocity not affected by 2D collision");

        // Check momentum conservation (approximately)
        float initialMomentum = 1.0f * 1.0f; // mass * velocity
        float finalMomentum = rb1.mass * rb1.velocity.x + rb2.mass * rb2.velocity.x;

        ACASSERT(std::abs(finalMomentum - initialMomentum) < 0.01f,
            "Momentum not conserved in 2D collision");

        // Test collision with different restitution values
        collisionTracker.Reset();
        rb1.restitution = 0.8f;
        rb2.restitution = 0.8f;

        transform1.position = glm::vec3(-2.0f, 0.0f, 0.0f);
        rb1.velocity = glm::vec2(1.0f, 0.0f);
        rb2.velocity = glm::vec2(0.0f, 0.0f);

        collisionOccurred = false;
        for (int i = 0; i < 60 && !collisionOccurred; i++)
        {
            world.Update();
            if (collisionTracker.collisionDetected) {
                collisionOccurred = true;
            }
            Sleep(16);
        }

        ACASSERT(collisionOccurred, "2D Elastic collision did not occur");
        ACASSERT(rb1.velocity.x < 0.0f, "First object should bounce back with negative velocity in elastic collision");

        ACMSG("2D Collision Resolution test passed");
    }

    void TestTrigger2DVolumes()
    {
        ACMSG("Testing 2D Trigger Volumes...");

        World world = Create2DPhysicsWorld();
        collisionTracker.Reset();
        world.GetResourse<EventManager>().AddListener<OnTriggerEnter>(TestTriggerHandler);

        // Create a trigger volume
        Entity triggerEntity = world.CreateEntity("TriggerVolume2D");
        world.Add<Transform>(triggerEntity, Transform());
        world.Add<CircleCollider2D>(triggerEntity, CircleCollider2D(0.5f));
        world.Get<CircleCollider2D>(triggerEntity).isTrigger = true;

        // Create an entity that will enter the trigger
        Entity movingEntity = world.CreateEntity("MovingObject2D");
        world.Add<Transform>(movingEntity, Transform());
        world.Add<RigidBody2D>(movingEntity, RigidBody2D(1.0f));
        world.Add<CircleCollider2D>(movingEntity, CircleCollider2D(0.5f));

        Transform& triggerTransform = world.Get<Transform>(triggerEntity);
        triggerTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);

        Transform& movingTransform = world.Get<Transform>(movingEntity);
        RigidBody2D& rb = world.Get<RigidBody2D>(movingEntity);

        movingTransform.position = glm::vec3(-2.0f, 0.0f, 0.0f);
        rb.useGravity = false;
        rb.velocity = glm::vec2(1.0f, 0.0f);

        // Run simulation until trigger event occurs
        bool triggerEntered = false;
        for (int i = 0; i < 60 && !triggerEntered; i++) // Run for up to 60 frames
        {
            world.Update();
            if (collisionTracker.collisionDetected) {
                triggerEntered = true;
            }
            Sleep(16);
        }

        ACASSERT(triggerEntered, "2D Trigger event not detected");

        // Object should continue moving through the trigger (no physical response)
        ACASSERT(glm::abs(rb.velocity.x - 1.0f) < 0.001f,
            "Moving object velocity incorrectly affected by 2D trigger");

        // Test different shapes - rect trigger with circle object
        collisionTracker.Reset();
        world.Delete<CircleCollider2D>(triggerEntity);
        world.Add<RectCollider2D>(triggerEntity, RectCollider2D(1.0f, 1.0f));
        world.Get<RectCollider2D>(triggerEntity).isTrigger = true;

        movingTransform.position = glm::vec3(-2.0f, 0.0f, 0.0f);

        triggerEntered = false;
        for (int i = 0; i < 60 && !triggerEntered; i++)
        {
            world.Update();
            if (collisionTracker.collisionDetected) {
                triggerEntered = true;
            }
            Sleep(16);
        }

        ACASSERT(triggerEntered, "Different shaped 2D triggers not working");

        ACMSG("2D Trigger Volumes test passed");
    }
    void RunPhysicsTests()
    {
        ACMSG("=== Running Physics System Tests ===");
        
        TestRigidBodyProperties();
        //TestRigidBodyForces();
        TestColliderBase();
        TestBoxCollider();
        TestSphereCollider();
        TestMixedCollisions();
        TestPhysicsSystem();
        TestCollisionResolution();
        TestTriggerVolumes();

        // 2D Physics Tests
        TestRigidBody2DProperties();
        //TestRigidBody2DForces();
        TestCircleCollider2D();
        TestRectCollider2D();
        TestMixed2DCollisions();
        TestPhysics2DSystem();
        TestCollision2DResolution();
        TestTrigger2DVolumes();
        
        ACMSG("=== All Physics System Tests Completed ===");
    }
}