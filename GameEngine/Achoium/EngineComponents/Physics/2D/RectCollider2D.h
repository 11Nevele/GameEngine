#pragma once
#include "../Collider.h"

namespace ac
{
    /**
     * @brief 2D rectangle collision shape.
     * 
     * RectCollider2D represents a rectangular collision shape defined by its half-width and half-height.
     * It operates in the XY plane with the Z component ignored.
     */
    class RectCollider2D : public Collider
    {
    public:
        glm::vec3 halfSize; ///< Half-extents of the rectangle (half width, height, depth)
        
        /**
         * @brief Default constructor creates a 1x1 rectangle.
         */
        RectCollider2D();
        
        /**
         * @brief Creates a rectangle with specified dimensions.
         * 
         * @param width Width of the rectangle
         * @param height Height of the rectangle
         */
        RectCollider2D(float width, float height);
        
        /**
         * @brief Creates a rectangle with specified dimensions, offset, and layer.
         * 
         * @param width Width of the rectangle
         * @param height Height of the rectangle
         * @param _offset Local position offset from entity's transform
         * @param _layer Collision layer this collider belongs to
         * @param _isTrigger Whether this collider is a trigger
         */
        RectCollider2D(float width, float height, const glm::vec3& _offset, uint32_t _layer = 0, bool _isTrigger = false);

        /**
         * @brief Implements collision detection for this rectangle collider.
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
        
        /**
         * @brief Gets the vertices of the rectangle in local space.
         * 
         * @return The four vertices of the rectangle in counterclockwise order
         */
        std::vector<glm::vec2> GetLocalVertices() const;
        
        /**
         * @brief Gets the vertices of the rectangle in world space.
         * 
         * @param transform The entity's transform component
         * @return The four vertices of the rectangle in counterclockwise order
         */
        std::vector<glm::vec2> GetWorldVertices(const Transform& transform) const;

        /**
         * @brief Rectangle vs Rectangle collision detection using SAT.
         */
        bool RectVsRect(
            const RectCollider2D* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const;

        /**
         * @brief Rectangle vs Circle collision detection.
         */
        bool RectVsCircle(
            const class CircleCollider2D* circle,
            const Transform& myTransform,
            const Transform& circleTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const;
        
        /**
         * @brief Rectangle vs Polygon collision detection using SAT.
         */
        bool RectVsPolygon(
            const class PolygonCollider2D* polygon,
            const Transform& myTransform,
            const Transform& polygonTransform,
            glm::vec3& collisionPoint,
            glm::vec3& collisionNormal,
            float& penetrationDepth
        ) const;
    };
}