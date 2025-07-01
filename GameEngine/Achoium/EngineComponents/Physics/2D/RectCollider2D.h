#pragma once
#include "Collider2D.h"

namespace ac
{
    class PolygonCollider2D;
    class CircleCollider2D;
    class RectCollider2D;
    /**
     * @brief 2D rectangle collision shape.
     * 
     * RectCollider2D represents a rectangular collision shape defined by its half-width and half-height.
     * It operates in the XY plane with the Z component ignored.
     */
    class RectCollider2D : public Collider2D
    {
    public:
        friend class PolygonCollider2D;
        friend class CircleCollider2D;
        glm::vec2 halfSize; ///< Half-extents of the rectangle (half width, height, depth)
        
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
        RectCollider2D(float width, float height, const glm::vec2& _offset, uint32_t _layer = 0, bool _isTrigger = false);

        /**
         * @brief Implements collision detection for this rectangle collider.
         * 
         * Checks for collision with another collider and outputs collision data.
         */
        virtual bool CheckCollision(
            const Collider2D* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint2D>& collisionPoints,
            glm::vec2& collisionNormal,
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
            std::vector<CollisionPoint2D>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const;

        /**
         * @brief Rectangle vs Circle collision detection.
         */
        bool RectVsCircle(
            const class CircleCollider2D* circle,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint2D>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const;
        
        /**
         * @brief Rectangle vs Polygon collision detection using SAT.
         */
        bool RectVsPolygon(
            const class PolygonCollider2D* polygon,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint2D>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const;

    private:
        /**
         * @brief Helper method for clipping a segment to a line.
         * 
         * @param v1 First vertex of the segment
         * @param v2 Second vertex of the segment
         * @param normal Normal vector of the line
         * @param offset Offset of the line
         * @param outPoints Output vector to store clipped points
         */
        void ClipSegmentToLine(
            const glm::vec2& v1,
            const glm::vec2& v2,
            const glm::vec2& normal,
            float offset,
            std::vector<glm::vec2>& outPoints
        ) const;
    };
}