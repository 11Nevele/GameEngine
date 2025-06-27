#pragma once
#include "Collider2D.h"

namespace ac
{
    /**
     * @brief 2D circular collision shape.
     * 
     * CircleCollider2D represents a circular collision shape defined by its radius.
     * It operates in the XY plane with the Z component ignored.
     */
    class CircleCollider2D : public Collider2D
    {
    public:
        float radius; ///< Radius of the circle
        
        /**
         * @brief Default constructor creates a circle with radius 0.5.
         */
        CircleCollider2D();
        
        /**
         * @brief Creates a circle with the specified radius.
         * 
         * @param _radius Radius of the circle
         */
        explicit CircleCollider2D(float _radius);
        
        /**
         * @brief Creates a circle with specified radius, offset, and layer.
         * 
         * @param _radius Radius of the circle
         * @param _offset Local position offset from entity's transform
         * @param _layer Collision layer this collider belongs to
         * @param _isTrigger Whether this collider is a trigger
         */
        CircleCollider2D(float _radius, const glm::vec2& _offset, uint32_t _layer = 0, bool _isTrigger = false);

        /**
         * @brief Implements collision detection for this circle collider.
         * 
         * Checks for collision with another collider and outputs collision data.
         */
        virtual bool CheckCollision(
            const Collider2D* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const override;

        
        /**
         * @brief Circle vs Circle collision detection.
         */
        bool CircleVsCircle(
            const CircleCollider2D* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const;

        /**
         * @brief Circle vs Rectangle collision detection.
         */
        bool CircleVsRect(
            const class RectCollider2D* rect,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const;
        
        /**
         * @brief Circle vs Polygon collision detection.
         */
        bool CircleVsPolygon(
            const class PolygonCollider2D* polygon,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const;
    };
}