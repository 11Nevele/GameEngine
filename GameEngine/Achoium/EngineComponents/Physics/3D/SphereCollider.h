#pragma once
#include "../Collider.h"

namespace ac
{
    /**
     * @brief Spherical collision volume.
     * 
     * SphereCollider represents a spherical collision shape defined by its radius.
     */
    class SphereCollider : public Collider
    {
    public:
        float radius; ///< Radius of the sphere
        
        /**
         * @brief Default constructor creates a sphere with radius 0.5.
         */
        SphereCollider();
        
        /**
         * @brief Creates a sphere with the specified radius.
         * 
         * @param _radius Radius of the sphere
         */
        explicit SphereCollider(float _radius);
        
        /**
         * @brief Creates a sphere with specified radius and offset.
         * 
         * @param _radius Radius of the sphere
         * @param _offset Local position offset from entity's transform
         */
        SphereCollider(float _radius, const glm::vec3& _offset);

        /**
         * @brief Implements collision detection for this sphere collider.
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
         * @brief Sphere vs Sphere collision detection.
         */
        bool SphereVsSphere(
            const SphereCollider* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const;

        /**
         * @brief Sphere vs Box collision detection.
         */
        bool SphereVsBox(
            const class BoxCollider* box,
            const Transform& myTransform,
            const Transform& boxTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const;
    };
}