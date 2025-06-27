//#pragma once
//
//namespace ac
//{
//    /**
//     * @brief Test RigidBody component functionality.
//     * 
//     * Tests the initialization, force application, and impulse application
//     * functionality of the RigidBody component.
//     */
//    void TestRigidBodyProperties();
//    
//    /**
//     * @brief Test RigidBody force application.
//     * 
//     * Tests that forces properly affect velocity and position over time.
//     */
//    void TestRigidBodyForces();
//    
//    /**
//     * @brief Test Collider base class functionality.
//     * 
//     * Tests the common functionality of all colliders.
//     */
//    void TestColliderBase();
//
//    /**
//     * @brief Test BoxCollider properties and collision detection.
//     * 
//     * Tests the initialization and box-box collision detection.
//     */
//    void TestBoxCollider();
//    
//    /**
//     * @brief Test SphereCollider properties and collision detection.
//     * 
//     * Tests the initialization and sphere-sphere collision detection.
//     */
//    void TestSphereCollider();
//    
//    /**
//     * @brief Test collisions between different collider types.
//     * 
//     * Tests box-sphere collision detection.
//     */
//    void TestMixedCollisions();
//    
//    /**
//     * @brief Test physics system integration.
//     * 
//     * Tests that the physics step properly updates entities.
//     */
//    void TestPhysicsSystem();
//    
//    /**
//     * @brief Test collision resolution.
//     * 
//     * Tests that colliding objects properly respond to collisions.
//     */
//    void TestCollisionResolution();
//    
//    /**
//     * @brief Test trigger volumes.
//     * 
//     * Tests that trigger volumes generate events without physical response.
//     */
//    void TestTriggerVolumes();
//    
//    /**
//     * @brief Test RigidBody2D component functionality.
//     * 
//     * Tests the initialization, force application, and impulse application
//     * functionality of the RigidBody2D component.
//     */
//    void TestRigidBody2DProperties();
//    
//    /**
//     * @brief Test RigidBody2D force application.
//     * 
//     * Tests that forces properly affect velocity and position over time in 2D.
//     */
//    void TestRigidBody2DForces();
//    
//    /**
//     * @brief Test CircleCollider2D properties and collision detection.
//     * 
//     * Tests the initialization and circle-circle collision detection.
//     */
//    void TestCircleCollider2D();
//    
//    /**
//     * @brief Test RectCollider2D properties and collision detection.
//     * 
//     * Tests the initialization and rect-rect collision detection.
//     */
//    void TestRectCollider2D();
//    
//    /**
//     * @brief Test collisions between different 2D collider types.
//     * 
//     * Tests circle-rect collision detection.
//     */
//    void TestMixed2DCollisions();
//    
//    /**
//     * @brief Test 2D physics system integration.
//     * 
//     * Tests that the 2D physics step properly updates entities.
//     */
//    void TestPhysics2DSystem();
//    
//    /**
//     * @brief Test 2D collision resolution.
//     * 
//     * Tests that colliding 2D objects properly respond to collisions.
//     */
//    void TestCollision2DResolution();
//    
//    /**
//     * @brief Test 2D trigger volumes.
//     * 
//     * Tests that 2D trigger volumes generate events without physical response.
//     */
//    void TestTrigger2DVolumes();
//    
//    /**
//     * @brief Run all physics test cases.
//     * 
//     * Executes all the physics system test units.
//     */
//    void RunPhysicsTests();
//}