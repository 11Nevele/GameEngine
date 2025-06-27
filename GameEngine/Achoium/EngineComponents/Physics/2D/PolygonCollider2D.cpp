#include "acpch.h"
#include "PolygonCollider2D.h"
#include "CircleCollider2D.h"
#include "Math/Transform.h"
#include <glm/gtx/norm.hpp>
#include <limits>
#include <algorithm>
#include <numeric> // Add this include directive at the top of the file
namespace ac
{
    PolygonCollider2D::PolygonCollider2D()
        : Collider2D()
    {
        // Create a default triangle
        m_vertices.push_back(glm::vec2(-0.5f, -0.5f));
        m_vertices.push_back(glm::vec2(0.5f, -0.5f));
        m_vertices.push_back(glm::vec2(0.0f, 0.5f));
    }
    
    PolygonCollider2D::PolygonCollider2D(const std::vector<glm::vec2>& _vertices)
        : Collider2D()
        , m_vertices(_vertices)
    {
        // Ensure we have at least 3 vertices for a valid polygon
        if (m_vertices.size() < 3)
        {
            // Create a default triangle as fallback
            m_vertices.clear();
            m_vertices.push_back(glm::vec2(-0.5f, -0.5f));
            m_vertices.push_back(glm::vec2(0.5f, -0.5f));
            m_vertices.push_back(glm::vec2(0.0f, 0.5f));
        }
    }
    
    PolygonCollider2D::PolygonCollider2D(const std::vector<glm::vec2>& _vertices, const glm::vec2& _offset, uint32_t _layer, bool _isTrigger)
        : Collider2D(_layer)
        , m_vertices(_vertices)
    {
        offset = _offset;
        isTrigger = _isTrigger;
        
        // Ensure we have at least 3 vertices for a valid polygon
        if (m_vertices.size() < 3)
        {
            // Create a default triangle as fallback
            m_vertices.clear();
            m_vertices.push_back(glm::vec2(-0.5f, -0.5f));
            m_vertices.push_back(glm::vec2(0.5f, -0.5f));
            m_vertices.push_back(glm::vec2(0.0f, 0.5f));
        }
    }
    
    std::vector<glm::vec2> PolygonCollider2D::GetWorldVertices(const Transform& transform) const
    {
        std::vector<glm::vec2> worldVertices(m_vertices.size());
        
        glm::mat4 modelMatrix = transform.asMat4();
        
        for (size_t i = 0; i < m_vertices.size(); ++i)
        {
            // Apply transform to each vertex
            glm::vec4 worldPos = modelMatrix * glm::vec4(m_vertices[i].x, m_vertices[i].y, 0.0f, 1.0f);
            worldVertices[i] = glm::vec2(worldPos.x, worldPos.y);
        }
        
        return worldVertices;
    }
    
    std::vector<glm::vec2> PolygonCollider2D::GetEdges() const
    {
        std::vector<glm::vec2> edges(m_vertices.size());
        
        for (size_t i = 0; i < m_vertices.size(); ++i)
        {
            size_t j = (i + 1) % m_vertices.size();
            edges[i] = m_vertices[j] - m_vertices[i];
        }
        
        return edges;
    }
    
    bool PolygonCollider2D::CheckCollision(
        const Collider2D* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        std::vector<CollisionPoint>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Check what type of collider we're colliding with and delegate to specific function
        if (const PolygonCollider2D* otherPolygon = dynamic_cast<const PolygonCollider2D*>(other))
        {
            return PolygonVsPolygon(otherPolygon, myTransform, otherTransform, 
                collisionPoints, collisionNormal, penetrationDepth);
        }
        else if (const CircleCollider2D* otherCircle = dynamic_cast<const CircleCollider2D*>(other))
        {
            return PolygonVsCircle(otherCircle, myTransform, otherTransform, 
                collisionPoints, collisionNormal, penetrationDepth);
        }
        
        // Unsupported collider type
        return false;
    }
    
    glm::vec2 PolygonCollider2D::FindClosestPoint(const glm::vec2& point) const
    {
        // If there are no vertices, return the point itself
        if (m_vertices.empty())
        {
            return point;
        }
        
        // Initialize with first vertex
        glm::vec2 closestPoint = m_vertices[0];
        float minDistSq = glm::length2(point - m_vertices[0]);
        
        // Check each edge for closest point
        for (size_t i = 0; i < m_vertices.size(); ++i)
        {
            size_t j = (i + 1) % m_vertices.size();
            
            glm::vec2 edge = m_vertices[j] - m_vertices[i];
            float edgeLenSq = glm::length2(edge);
            
            // Skip degenerate edges
            if (edgeLenSq < 0.0001f)
                continue;
            
            // Calculate projection of point onto edge
            glm::vec2 toPoint = point - m_vertices[i];
            float t = glm::dot(toPoint, edge) / edgeLenSq;
            
            // Clamp to edge
            t = glm::clamp(t, 0.0f, 1.0f);
            
            // Calculate closest point on edge
            glm::vec2 edgePoint = m_vertices[i] + t * edge;
            
            // Check if this is closer than the current closest point
            float distSq = glm::length2(point - edgePoint);
            if (distSq < minDistSq)
            {
                minDistSq = distSq;
                closestPoint = edgePoint;
            }
        }
        
        return closestPoint;
    }
    
    glm::vec2 PolygonCollider2D::FindPenetrationNormal(const glm::vec2& point, const Transform& transform) const
    {
        // Get edges in local space
        std::vector<glm::vec2> edges = GetEdges();
        
        // Find the edge with the minimum penetration
        float minPenetration = std::numeric_limits<float>::max();
        glm::vec2 bestNormal;
        
        for (size_t i = 0; i < m_vertices.size(); ++i)
        {
            size_t j = (i + 1) % m_vertices.size();
            
            // Calculate edge normal (perpendicular to edge)
            glm::vec2 edge = m_vertices[j] - m_vertices[i];
            glm::vec2 normal(-edge.y, edge.x);
            normal = glm::normalize(normal);
            
            // Calculate distance from point to edge along the normal
            glm::vec2 toPoint = point - m_vertices[i];
            float distance = glm::dot(toPoint, normal);
            
            // If outside the polygon (shouldn't happen if using correct winding order)
            if (distance > 0)
            {
                normal = -normal;
                distance = -distance;
            }
            
            // Track the minimum penetration edge
            if (std::abs(distance) < minPenetration)
            {
                minPenetration = std::abs(distance);
                bestNormal = normal;
            }
        }
        
        // Convert to world space
        glm::mat4 rotationMat = transform.asMat4();
        glm::vec4 worldNormal = rotationMat * glm::vec4(bestNormal, 0.0f, 0.0f);
        
        return glm::normalize(glm::vec3(worldNormal));
    }
    
    bool PolygonCollider2D::FindMinimumPenetration(
        const std::vector<glm::vec2>& edges1,
        const std::vector<glm::vec2>& vertices1,
        const std::vector<glm::vec2>& edges2,
        const std::vector<glm::vec2>& vertices2,
        glm::vec2& axis,
        float& depth)
    {
        depth = std::numeric_limits<float>::max();
        
        // Check all axes from shape 1
        for (size_t i = 0; i < edges1.size(); ++i)
        {
            // Get the normal to this edge
            glm::vec2 normal(-edges1[i].y, edges1[i].x);
            normal = glm::normalize(normal);
            
            // Project both shapes onto the normal
            float min1 = std::numeric_limits<float>::max();
            float max1 = -std::numeric_limits<float>::max();
            float min2 = std::numeric_limits<float>::max();
            float max2 = -std::numeric_limits<float>::max();
            
            // Project shape 1
            for (const auto& vertex : vertices1)
            {
                float projection = glm::dot(vertex, normal);
                min1 = std::min(min1, projection);
                max1 = std::max(max1, projection);
            }
            
            // Project shape 2
            for (const auto& vertex : vertices2)
            {
                float projection = glm::dot(vertex, normal);
                min2 = std::min(min2, projection);
                max2 = std::max(max2, projection);
            }
            
            // Check for separation
            if (min1 > max2 || min2 > max1)
            {
                return false; // Separating axis found
            }
            
            // Calculate penetration depth
            float overlap = std::min(max1 - min2, max2 - min1);
            
            // Track minimum penetration
            if (overlap < depth)
            {
                depth = overlap;
                axis = normal;
                
                // Ensure the axis points from shape1 to shape2
                float center1 = (min1 + max1) * 0.5f;
                float center2 = (min2 + max2) * 0.5f;
                
                if (center1 > center2)
                {
                    axis = -axis;
                }
            }
        }
        
        return true;
    }
    
    bool PolygonCollider2D::PolygonVsPolygon(
        const PolygonCollider2D* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        std::vector<CollisionPoint>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get vertices and edges in world space
        std::vector<glm::vec2> myVerticesWorld = GetWorldVertices(myTransform);
        std::vector<glm::vec2> otherVerticesWorld = other->GetWorldVertices(otherTransform);
        
        // Calculate edges
        std::vector<glm::vec2> myEdges(myVerticesWorld.size());
        std::vector<glm::vec2> otherEdges(otherVerticesWorld.size());
        
        // Generate edges for my shape
        for (size_t i = 0; i < myVerticesWorld.size(); ++i)
        {
            size_t j = (i + 1) % myVerticesWorld.size();
            myEdges[i] = myVerticesWorld[j] - myVerticesWorld[i];
        }
        
        // Generate edges for other shape
        for (size_t i = 0; i < otherVerticesWorld.size(); ++i)
        {
            size_t j = (i + 1) % otherVerticesWorld.size();
            otherEdges[i] = otherVerticesWorld[j] - otherVerticesWorld[i];
        }
        
        // Check my axes
        glm::vec2 axisA;
        float depthA;
        bool collideA = FindMinimumPenetration(
            myEdges, myVerticesWorld, otherEdges, otherVerticesWorld, 
            axisA, depthA
        );
        
        if (!collideA)
            return false;
        
        // Check other's axes
        glm::vec2 axisB;
        float depthB;
        bool collideB = FindMinimumPenetration(
            otherEdges, otherVerticesWorld, myEdges, myVerticesWorld, 
            axisB, depthB
        );
        
        if (!collideB)
            return false;
        
        // Use the smaller penetration for the result
        if (depthA < depthB)
        {
            collisionNormal = glm::vec3(axisA.x, axisA.y, 0.0f);
            penetrationDepth = depthA;
        }
        else
        {
            collisionNormal = glm::vec3(-axisB.x, -axisB.y, 0.0f);
            penetrationDepth = depthB;
        }
        
        // Calculate contact point (approximate with centroid of intersection)
        glm::vec2 center1 = std::accumulate(myVerticesWorld.begin(), myVerticesWorld.end(), glm::vec2(0.0f)) 
                            / static_cast<float>(myVerticesWorld.size());
        glm::vec2 center2 = std::accumulate(otherVerticesWorld.begin(), otherVerticesWorld.end(), glm::vec2(0.0f)) 
                            / static_cast<float>(otherVerticesWorld.size());
        
        
        return true;
    }
    
    bool PolygonCollider2D::PolygonVsCircle(
        const CircleCollider2D* circle,
        const Transform& myTransform,
        const Transform& otherTransform,
        std::vector<CollisionPoint>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Delegate to the circle's implementation but flip the normal
        bool result = circle->CircleVsPolygon(this, otherTransform, myTransform,
                                            collisionPoints, collisionNormal, penetrationDepth);
        
        if (result)
        {
            collisionNormal = -collisionNormal;
        }
        
        return result;
    }
}