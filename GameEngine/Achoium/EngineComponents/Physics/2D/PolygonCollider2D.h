#pragma once
#include "Collider2D.h"
#include <vector>

namespace ac
{
    /**
     * @brief 2D polygon collision shape.
     * 
     * PolygonCollider2D represents an arbitrary convex polygon collision shape
     * defined by a set of vertices. It operates in the XY plane with the Z component ignored.
     */
    class PolygonCollider2D : public Collider2D
    {
    public:
        /**
         * @brief Default constructor creates a triangle.
         */
        PolygonCollider2D();
        
        /**
         * @brief Creates a polygon with the specified vertices.
         * 
         * @param _vertices The local-space vertices of the polygon in counterclockwise order
         */
        explicit PolygonCollider2D(const std::vector<glm::vec2>& _vertices);
        
        /**
         * @brief Creates a polygon with the specified vertices, offset, and layer.
         * 
         * @param _vertices The local-space vertices of the polygon in counterclockwise order
         * @param _offset Local position offset from entity's transform
         * @param _layer Collision layer this collider belongs to
         * @param _isTrigger Whether this collider is a trigger
         */
        PolygonCollider2D(const std::vector<glm::vec2>& _vertices, const glm::vec2& _offset, uint32_t _layer = 0, bool _isTrigger = false);
        
        /**
         * @brief Gets the local-space vertices of the polygon.
         * 
         * @return The vertices in counterclockwise order
         */
        const std::vector<glm::vec2>& GetVertices() const { return m_vertices; }
        
        /**
         * @brief Gets the world-space vertices of the polygon.
         * 
         * @param transform The entity's transform component
         * @return The vertices in counterclockwise order
         */
        std::vector<glm::vec2> GetWorldVertices(const Transform& transform) const;
        
        /**
         * @brief Gets the local-space edges of the polygon.
         * 
         * @return The edges in counterclockwise order
         */
        std::vector<glm::vec2> GetEdges() const;

        /**
         * @brief Implements collision detection for this polygon collider.
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
         * @brief Finds the closest point on the polygon to the given point.
         * 
         * @param point The point to check against (in local space)
         * @return The closest point on the polygon perimeter
         */
        glm::vec2 FindClosestPoint(const glm::vec2& point) const;
        
        /**
         * @brief Finds the penetration normal for a point inside or on the polygon.
         * 
         * @param point The point to check (in local space)
         * @param transform The polygon's transform
         * @return The penetration normal pointing out of the polygon
         */
        glm::vec2 FindPenetrationNormal(const glm::vec2& point, const Transform& transform) const;
        
        /**
         * @brief Calculates the minimum penetration axis and depth using SAT.
         * 
         * @param edges1 Edges of the first polygon
         * @param vertices1 Vertices of the first polygon
         * @param edges2 Edges of the second polygon
         * @param vertices2 Vertices of the second polygon
         * @param axis Output parameter for the collision normal
         * @param depth Output parameter for the penetration depth
         * @return True if the polygons are colliding
         */
        static bool FindMinimumPenetration(
            const std::vector<glm::vec2>& edges1,
            const std::vector<glm::vec2>& vertices1,
            const std::vector<glm::vec2>& edges2,
            const std::vector<glm::vec2>& vertices2,
            glm::vec2& axis,
            float& depth
        );

        /**
         * @brief Polygon vs Polygon collision detection using SAT.
         */
        bool PolygonVsPolygon(
            const PolygonCollider2D* other,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const;

        std::vector<glm::vec2> m_vertices; ///< Local-space vertices of the polygon
        
        /**
         * @brief Circle vs Polygon collision detection.
         */
        bool PolygonVsCircle(
            const class CircleCollider2D* circle,
            const Transform& myTransform,
            const Transform& otherTransform,
            std::vector<CollisionPoint>& collisionPoints,
            glm::vec2& collisionNormal,
            float& penetrationDepth
        ) const;
    };
}