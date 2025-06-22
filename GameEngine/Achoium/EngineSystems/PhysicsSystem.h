#pragma once
#include "Core/World.hpp"
#include <glm/glm.hpp>
namespace ac
{
    /**
     * @brief Collision data structure containing information about a collision event.
     */
    struct CollisionData
    {
        Entity entityA; ///< First entity in the collision
        Entity entityB; ///< Second entity in the collision
        glm::vec3 collisionPoint; ///< Point of contact in world space
        glm::vec3 collisionNormal; ///< Normal of the collision surface (points from A to B)
        float penetrationDepth; ///< How far the objects are interpenetrating
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
    };

    /**
     * @brief Collision event broadcasted when two colliders intersect.
     */
    struct OnCollision
    {
        CollisionData data; ///< Data about the collision
        World& world; ///< Reference to the world
    };

    ALLOWTOKEN(OnCollision, friend class PhysicsSystem;)

    /**
     * @brief Trigger event broadcasted when a collider enters a trigger.
     */
    struct OnTriggerEnter
    {
        CollisionData data; ///< Data about the trigger
        World& world; ///< Reference to the world
    };

    ALLOWTOKEN(OnTriggerEnter, friend class PhysicsSystem;)
}