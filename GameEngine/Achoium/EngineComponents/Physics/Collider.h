#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <cstdint>

namespace ac
{
    // Forward declaration of Transform
    class Transform;
    
    /**
     * @brief Base class for all collision shapes.
     * 
     * Collider is an abstract class that provides the interface
     * for all collision shape types in the physics system.
     */
    class Collider
    {
    public:
        glm::vec3 offset; ///< Local position offset from entity's transform
        bool isTrigger;   ///< If true, detects collisions but doesn't resolve them
        uint32_t layer;   ///< Collision layer this collider belongs to
        
        /**
         * @brief Default constructor.
         */
        Collider();
        
        /**
         * @brief Constructor with layer.
         * 
         * @param _layer The collision layer this collider belongs to
         */
        explicit Collider(uint32_t _layer);
        
        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~Collider() = default;
        
        /**
         * @brief Gets the world position of the collider based on entity transform.
         * 
         * @param transform The entity's transform component
         * @return World position of the collider
         */
        glm::vec3 GetWorldPosition(const Transform& transform) const;
        
        /**
         * @brief Abstract method for collision detection against another collider.
         * 
         * @param other The other collider to check collision against
         * @param myTransform This collider's entity transform
         * @param otherTransform The other collider's entity transform
         * @param collisionPoint Output parameter for the point of collision if detected
         * @param collisionNormal Output parameter for the collision normal if detected
         * @param penetrationDepth Output parameter for the penetration depth if detected
         * @return True if the colliders are intersecting
         */
        virtual bool CheckCollision(
            const Collider* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const = 0;
    };
}