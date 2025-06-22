#pragma once

namespace ac
{
    /**
     * @brief Test RigidBody component functionality.
     * 
     * Tests the initialization, force application, and impulse application
     * functionality of the RigidBody component.
     */
    void TestRigidBodyProperties();
    
    /**
     * @brief Test RigidBody force application.
     * 
     * Tests that forces properly affect velocity and position over time.
     */
    void TestRigidBodyForces();
    
    /**
     * @brief Test Collider base class functionality.
     * 
     * Tests the common functionality of all colliders.
     */
    void TestColliderBase();

    /**
     * @brief Test BoxCollider properties and collision detection.
     * 
     * Tests the initialization and box-box collision detection.
     */
    void TestBoxCollider();
    
    /**
     * @brief Test SphereCollider properties and collision detection.
     * 
     * Tests the initialization and sphere-sphere collision detection.
     */
    void TestSphereCollider();
    
    /**
     * @brief Test collisions between different collider types.
     * 
     * Tests box-sphere collision detection.
     */
    void TestMixedCollisions();
    
    /**
     * @brief Test physics system integration.
     * 
     * Tests that the physics step properly updates entities.
     */
    void TestPhysicsSystem();
    
    /**
     * @brief Test collision resolution.
     * 
     * Tests that colliding objects properly respond to collisions.
     */
    void TestCollisionResolution();
    
    /**
     * @brief Test trigger volumes.
     * 
     * Tests that trigger volumes generate events without physical response.
     */
    void TestTriggerVolumes();
    
    /**
     * @brief Run all physics test cases.
     * 
     * Executes all the physics system test units.
     */
    void RunPhysicsTests();
}