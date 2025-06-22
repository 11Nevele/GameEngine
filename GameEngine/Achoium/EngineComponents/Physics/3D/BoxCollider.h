#pragma once
#include "../Collider.h"

namespace ac
{
    /**
     * @brief Box-shaped collision volume.
     * 
     * BoxCollider represents an oriented box collision shape defined
     * by its half-extents (half width, half height, half depth).
     */
    class BoxCollider : public Collider
    {
    public:
        glm::vec3 halfSize; ///< Half-extents of the box (half width, height, depth)
        
        /**
         * @brief Default constructor creates a 1x1x1 box.
         */
        BoxCollider();
        
        /**
         * @brief Creates a box with specified dimensions.
         * 
         * @param width Width of the box
         * @param height Height of the box
         * @param depth Depth of the box
         */
        BoxCollider(float width, float height, float depth);
        
        /**
         * @brief Creates a box with specified dimensions and offset.
         * 
         * @param size Full size of the box (width, height, depth)
         * @param _offset Local position offset from entity's transform
         */
        BoxCollider(const glm::vec3& size, const glm::vec3& _offset = glm::vec3(0.0f));

        /**
         * @brief Implements collision detection for this box collider.
         * 
         * Checks for collision with another collider and outputs collision data.
         */
        bool CheckCollision(
            const Collider* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const override;

    private:
        /**
         * @brief Box vs Box collision detection.
         */
        bool BoxVsBox(
            const BoxCollider* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const;

        /**
         * @brief Box vs Sphere collision detection.
         */
        bool BoxVsSphere(
            const class SphereCollider* sphere,
            const Transform& myTransform,
            const Transform& sphereTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const;
    };
}