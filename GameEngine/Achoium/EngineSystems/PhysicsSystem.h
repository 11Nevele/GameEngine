#pragma once
#include "Core/World.hpp"
#include <glm/glm.hpp>
#include "EngineComponents/Physics/Physics.h"
namespace ac
{
    /**
     * @brief Collision data structure containing information about a collision event.
     */
    struct CollisionData2D
    {
        Entity entityA = 0; ///< First entity in the collision
        Entity entityB = 0; ///< Second entity in the collision
        int collisionPointCnt = 0;
        CollisionPoint2D collisionPoint1{};
        CollisionPoint2D collisionPoint2{};
        glm::vec2 collisionNormal{0,0}; ///< Normal of the collision surface (points from A to B)
        float penetrationDepth = 0; ///< How far the objects are interpenetrating
    };

    
    class PhysicsSystem
    {
    public:
        /**
         * @brief System that handles 3D physics simulation.
         */
        static void PhysicsStep(World& world);

        /**
         * @brief System that handles 2D physics simulation.
         */
        static void Physics2DStep(World& world);

        /**
         * @brief System that performs 3D collision detection and resolution.
         */
        static void CollisionSystem(World& world);
        
        /**
         * @brief System that performs 2D collision detection and resolution.
         */
        static void Collision2DSystem(World& world);

        static void DebugPhysics(World& world);
    };

    /**
     * @brief Collision event broadcasted when two colliders intersect.
     */
    struct OnCollision
    {
        CollisionData2D data; ///< Data about the collision
        World& world; ///< Reference to the world
    };

    ALLOWTOKEN(OnCollision, friend class PhysicsSystem;)

    /**
     * @brief Trigger event broadcasted when a collider enters a trigger.
     */
    struct OnTriggerEnter
    {
        CollisionData2D data; ///< Data about the trigger
        World& world; ///< Reference to the world
    };

    ALLOWTOKEN(OnTriggerEnter, friend class PhysicsSystem;)
}