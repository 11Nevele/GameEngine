#include "acpch.h"
#include "RectCollider2D.h"
#include "CircleCollider2D.h"
#include "PolygonCollider2D.h"
#include "Math/Transform.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <numeric>
namespace ac
{
    RectCollider2D::RectCollider2D()
        : Collider()
        , halfSize(0.5f, 0.5f, 0.0f)
    {
    }
    
    RectCollider2D::RectCollider2D(float width, float height)
        : Collider()
        , halfSize(width * 0.5f, height * 0.5f, 0.0f)
    {
    }
    
    RectCollider2D::RectCollider2D(float width, float height, const glm::vec3& _offset, uint32_t _layer, bool _isTrigger)
        : Collider(_layer)
        , halfSize(width * 0.5f, height * 0.5f, 0.0f)
    {
        offset = _offset;
        isTrigger = _isTrigger;
    }
    
    std::vector<glm::vec2> RectCollider2D::GetLocalVertices() const
    {
        std::vector<glm::vec2> vertices(4);
        
        // Generate the vertices in counter-clockwise order
        vertices[0] = glm::vec2(-halfSize.x, -halfSize.y); // Bottom-left
        vertices[1] = glm::vec2(halfSize.x, -halfSize.y);  // Bottom-right
        vertices[2] = glm::vec2(halfSize.x, halfSize.y);   // Top-right
        vertices[3] = glm::vec2(-halfSize.x, halfSize.y);  // Top-left
        
        return vertices;
    }
    
    std::vector<glm::vec2> RectCollider2D::GetWorldVertices(const Transform& transform) const
    {
        std::vector<glm::vec2> localVertices = GetLocalVertices();
        std::vector<glm::vec2> worldVertices(4);
        
        // Create model matrix including offset
        glm::mat4 modelMatrix = transform.asMat4();
        
        // Apply offset to model matrix
        if (glm::length2(offset) > 0.0001f) {
            modelMatrix = glm::translate(modelMatrix, offset);
        }
        
        for (size_t i = 0; i < 4; ++i)
        {
            // Apply transform to each vertex
            glm::vec4 worldPos = modelMatrix * glm::vec4(localVertices[i].x, localVertices[i].y, 0.0f, 1.0f);
            worldVertices[i] = glm::vec2(worldPos.x, worldPos.y);
        }
        
        return worldVertices;
    }
    
    bool RectCollider2D::CheckCollision(
        const Collider* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Check what type of collider we're colliding with and delegate to specific function
        if (const RectCollider2D* otherRect = dynamic_cast<const RectCollider2D*>(other))
        {
            return RectVsRect(otherRect, myTransform, otherTransform, 
                collisionPoint, collisionNormal, penetrationDepth);
        }
        else if (const CircleCollider2D* otherCircle = dynamic_cast<const CircleCollider2D*>(other))
        {
            return RectVsCircle(otherCircle, myTransform, otherTransform, 
                collisionPoint, collisionNormal, penetrationDepth);
        }
        else if (const PolygonCollider2D* otherPolygon = dynamic_cast<const PolygonCollider2D*>(other))
        {
            return RectVsPolygon(otherPolygon, myTransform, otherTransform, 
                collisionPoint, collisionNormal, penetrationDepth);
        }
        
        // Unsupported collider type
        return false;
    }
    
    bool RectCollider2D::RectVsRect(
        const RectCollider2D* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get vertices in world space
        std::vector<glm::vec2> myVertices = GetWorldVertices(myTransform);
        std::vector<glm::vec2> otherVertices = other->GetWorldVertices(otherTransform);
        
        // Implement SAT for collision detection
        float minPenetration = std::numeric_limits<float>::max();
        glm::vec2 bestAxis;
        bool hasCollision = true;
        
        // Check my axes
        for (size_t i = 0; i < myVertices.size(); ++i)
        {
            size_t j = (i + 1) % myVertices.size();
            
            // Calculate the edge vector
            glm::vec2 edge = myVertices[j] - myVertices[i];
            
            // Calculate the perpendicular axis for projection (normal to the edge)
            glm::vec2 axis(-edge.y, edge.x);
            float length = glm::length(axis);
            if (length < 0.0001f) continue; // Skip degenerate edges
            
            axis = axis / length; // Normalize
            
            // Project both shapes onto the axis
            float minA = std::numeric_limits<float>::max();
            float maxA = -std::numeric_limits<float>::max();
            float minB = std::numeric_limits<float>::max();
            float maxB = -std::numeric_limits<float>::max();
            
            // Project vertices of rectangle A
            for (const auto& vertex : myVertices)
            {
                float projection = glm::dot(axis, vertex);
                minA = std::min(minA, projection);
                maxA = std::max(maxA, projection);
            }
            
            // Project vertices of rectangle B
            for (const auto& vertex : otherVertices)
            {
                float projection = glm::dot(axis, vertex);
                minB = std::min(minB, projection);
                maxB = std::max(maxB, projection);
            }
            
            // Check for separation
            if (minA > maxB || minB > maxA)
            {
                // Separating axis found, no collision
                hasCollision = false;
                break;
            }
            
            // Calculate penetration depth
            float overlap = std::min(maxA - minB, maxB - minA);
            
            // Track minimum penetration for collision response
            if (overlap < minPenetration)
            {
                minPenetration = overlap;
                bestAxis = axis;
                
                // Ensure the normal points from A to B
                float centerA = (minA + maxA) * 0.5f;
                float centerB = (minB + maxB) * 0.5f;
                if (centerA > centerB)
                {
                    bestAxis = -bestAxis;
                }
            }
        }
        
        if (!hasCollision) return false;
        
        // Check other's axes
        for (size_t i = 0; i < otherVertices.size(); ++i)
        {
            size_t j = (i + 1) % otherVertices.size();
            
            // Calculate the edge vector
            glm::vec2 edge = otherVertices[j] - otherVertices[i];
            
            // Calculate the perpendicular axis for projection
            glm::vec2 axis(-edge.y, edge.x);
            float length = glm::length(axis);
            if (length < 0.0001f) continue; // Skip degenerate edges
            
            axis = axis / length; // Normalize
            
            // Project both shapes onto the axis
            float minA = std::numeric_limits<float>::max();
            float maxA = -std::numeric_limits<float>::max();
            float minB = std::numeric_limits<float>::max();
            float maxB = -std::numeric_limits<float>::max();
            
            // Project vertices of rectangle A
            for (const auto& vertex : myVertices)
            {
                float projection = glm::dot(axis, vertex);
                minA = std::min(minA, projection);
                maxA = std::max(maxA, projection);
            }
            
            // Project vertices of rectangle B
            for (const auto& vertex : otherVertices)
            {
                float projection = glm::dot(axis, vertex);
                minB = std::min(minB, projection);
                maxB = std::max(maxB, projection);
            }
            
            // Check for separation
            if (minA > maxB || minB > maxA)
            {
                // Separating axis found, no collision
                hasCollision = false;
                break;
            }
            
            // Calculate penetration depth
            float overlap = std::min(maxA - minB, maxB - minA);
            
            // Track minimum penetration for collision response
            if (overlap < minPenetration)
            {
                minPenetration = overlap;
                bestAxis = axis;
                
                // Ensure the normal points from A to B
                float centerA = (minA + maxA) * 0.5f;
                float centerB = (minB + maxB) * 0.5f;
                if (centerA > centerB)
                {
                    bestAxis = -bestAxis;
                }
            }
        }
        
        if (!hasCollision) return false;
        
        // If we get here, there is a collision
        collisionNormal = glm::vec3(bestAxis.x, bestAxis.y, 0.0f);
        penetrationDepth = minPenetration;
        
        // Calculate an approximate collision point (center point of overlap)
        glm::vec3 myCenter = GetWorldPosition(myTransform);
        glm::vec3 otherCenter = other->GetWorldPosition(otherTransform);
        
        // Find point along the collision normal from the center of A
        glm::vec2 direction = glm::vec2(collisionNormal.x, collisionNormal.y);
        float distanceToContact = glm::dot(otherCenter - myCenter, collisionNormal);
        glm::vec3 contactPoint = myCenter + glm::vec3(direction, 0) * distanceToContact;
        
        collisionPoint = contactPoint;
        
        return true;
    }
    
    bool RectCollider2D::RectVsCircle(
        const CircleCollider2D* circle,
        const Transform& myTransform,
        const Transform& circleTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get world positions
        glm::vec3 rectCenter = GetWorldPosition(myTransform);
        glm::vec3 circleCenter = circle->GetWorldPosition(circleTransform);
        
        // Convert circle center to rectangle's local space
        // We need to invert the transform to get from world to local space
        glm::mat4 rectModelMatrix = myTransform.asMat4();
        if (glm::length2(offset) > 0.0001f) {
            rectModelMatrix = glm::translate(rectModelMatrix, offset);
        }
        
        glm::mat4 rectWorldToLocal = glm::inverse(rectModelMatrix);
        glm::vec4 circleCenterLocal = rectWorldToLocal * glm::vec4(circleCenter, 1.0f);
        
        // Find the closest point on the rectangle to the circle center
        glm::vec3 closestPoint;
        closestPoint.x = glm::clamp(circleCenterLocal.x, -halfSize.x, halfSize.x);
        closestPoint.y = glm::clamp(circleCenterLocal.y, -halfSize.y, halfSize.y);
        closestPoint.z = circleCenterLocal.z; // Keep the same Z value
        
        // Calculate the distance from the closest point to the circle center
        glm::vec3 difference = glm::vec3(circleCenterLocal) - closestPoint;
        float distanceSq = glm::length2(difference);
        
        // If the squared distance is greater than the circle's radius squared, no collision
        if (distanceSq > circle->radius * circle->radius)
        {
            return false;
        }
        
        // Convert the closest point back to world space
        glm::vec4 closestPointWorld = rectModelMatrix * glm::vec4(closestPoint, 1.0f);
        
        // Calculate collision normal (from rectangle to circle)
        float distance = glm::sqrt(distanceSq);
        
        if (distance < 0.0001f)
        {
            // Circle center is inside the rectangle, need to find the shortest exit direction
            
            // Find the penetration depth along each axis
            float dRight = halfSize.x - circleCenterLocal.x;  // Distance to right edge
            float dLeft = circleCenterLocal.x + halfSize.x;   // Distance to left edge
            float dTop = halfSize.y - circleCenterLocal.y;    // Distance to top edge
            float dBottom = circleCenterLocal.y + halfSize.y; // Distance to bottom edge
            
            // Find the minimum penetration
            float minPenetration = dRight;
            glm::vec3 localNormal(1.0f, 0.0f, 0.0f); // Right direction
            
            if (dLeft < minPenetration)
            {
                minPenetration = dLeft;
                localNormal = glm::vec3(-1.0f, 0.0f, 0.0f); // Left direction
            }
            
            if (dTop < minPenetration)
            {
                minPenetration = dTop;
                localNormal = glm::vec3(0.0f, 1.0f, 0.0f); // Up direction
            }
            
            if (dBottom < minPenetration)
            {
                minPenetration = dBottom;
                localNormal = glm::vec3(0.0f, -1.0f, 0.0f); // Down direction
            }
            
            // Transform the normal from local to world space
            glm::vec4 worldNormal = glm::transpose(rectWorldToLocal) * glm::vec4(localNormal, 0.0f);
            collisionNormal = glm::normalize(glm::vec3(worldNormal));
            
            // Calculate the penetration depth (include circle radius)
            penetrationDepth = minPenetration + circle->radius;
            
            // Set collision point at circle center
            collisionPoint = circleCenter;
        }
        else
        {
            // Normal conversion from local to world
            glm::vec3 localNormal = glm::normalize(difference);
            glm::vec4 worldNormal = glm::transpose(rectWorldToLocal) * glm::vec4(localNormal, 0.0f);
            collisionNormal = glm::normalize(glm::vec3(worldNormal));
            
            // Set collision point at the closest point on the rectangle
            collisionPoint = glm::vec3(closestPointWorld);
            
            // Calculate penetration depth
            penetrationDepth = circle->radius - distance;
        }
        
        return true;
    }
    
    bool RectCollider2D::RectVsPolygon(
        const PolygonCollider2D* polygon,
        const Transform& myTransform,
        const Transform& polygonTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Convert rectangle to polygon and use polygon-polygon collision
        std::vector<glm::vec2> myVertices = GetWorldVertices(myTransform);
        std::vector<glm::vec2> polyVertices = polygon->GetWorldVertices(polygonTransform);
        
        // Generate edges for rectangle
        std::vector<glm::vec2> myEdges(myVertices.size());
        for (size_t i = 0; i < myVertices.size(); ++i)
        {
            size_t j = (i + 1) % myVertices.size();
            myEdges[i] = myVertices[j] - myVertices[i];
        }
        
        // Generate edges for polygon
        std::vector<glm::vec2> polyEdges(polyVertices.size());
        for (size_t i = 0; i < polyVertices.size(); ++i)
        {
            size_t j = (i + 1) % polyVertices.size();
            polyEdges[i] = polyVertices[j] - polyVertices[i];
        }
        
        // Implement the SAT for collision detection
        float minPenetration = std::numeric_limits<float>::max();
        glm::vec2 bestAxis;
        bool hasCollision = true;
        
        // Check rectangle's axes
        for (const auto& edge : myEdges)
        {
            // Calculate the perpendicular axis for projection
            glm::vec2 axis(-edge.y, edge.x);
            float length = glm::length(axis);
            if (length < 0.0001f) continue; // Skip degenerate edges
            
            axis = axis / length; // Normalize
            
            // Project both shapes onto the axis
            float minA = std::numeric_limits<float>::max();
            float maxA = -std::numeric_limits<float>::max();
            float minB = std::numeric_limits<float>::max();
            float maxB = -std::numeric_limits<float>::max();
            
            // Project rectangle vertices
            for (const auto& vertex : myVertices)
            {
                float projection = glm::dot(axis, vertex);
                minA = std::min(minA, projection);
                maxA = std::max(maxA, projection);
            }
            
            // Project polygon vertices
            for (const auto& vertex : polyVertices)
            {
                float projection = glm::dot(axis, vertex);
                minB = std::min(minB, projection);
                maxB = std::max(maxB, projection);
            }
            
            // Check for separation
            if (minA > maxB || minB > maxA)
            {
                // Separating axis found, no collision
                hasCollision = false;
                break;
            }
            
            // Calculate penetration depth
            float overlap = std::min(maxA - minB, maxB - minA);
            
            // Track minimum penetration for collision response
            if (overlap < minPenetration)
            {
                minPenetration = overlap;
                bestAxis = axis;
                
                // Ensure the normal points from rectangle to polygon
                float centerA = (minA + maxA) * 0.5f;
                float centerB = (minB + maxB) * 0.5f;
                if (centerA > centerB)
                {
                    bestAxis = -bestAxis;
                }
            }
        }
        
        if (!hasCollision) return false;
        
        // Check polygon's axes
        for (const auto& edge : polyEdges)
        {
            // Calculate the perpendicular axis for projection
            glm::vec2 axis(-edge.y, edge.x);
            float length = glm::length(axis);
            if (length < 0.0001f) continue; // Skip degenerate edges
            
            axis = axis / length; // Normalize
            
            // Project both shapes onto the axis
            float minA = std::numeric_limits<float>::max();
            float maxA = -std::numeric_limits<float>::max();
            float minB = std::numeric_limits<float>::max();
            float maxB = -std::numeric_limits<float>::max();
            
            // Project rectangle vertices
            for (const auto& vertex : myVertices)
            {
                float projection = glm::dot(axis, vertex);
                minA = std::min(minA, projection);
                maxA = std::max(maxA, projection);
            }
            
            // Project polygon vertices
            for (const auto& vertex : polyVertices)
            {
                float projection = glm::dot(axis, vertex);
                minB = std::min(minB, projection);
                maxB = std::max(maxB, projection);
            }
            
            // Check for separation
            if (minA > maxB || minB > maxA)
            {
                // Separating axis found, no collision
                hasCollision = false;
                break;
            }
            
            // Calculate penetration depth
            float overlap = std::min(maxA - minB, maxB - minA);
            
            // Track minimum penetration for collision response
            if (overlap < minPenetration)
            {
                minPenetration = overlap;
                bestAxis = axis;
                
                // Ensure the normal points from rectangle to polygon
                float centerA = (minA + maxA) * 0.5f;
                float centerB = (minB + maxB) * 0.5f;
                if (centerA > centerB)
                {
                    bestAxis = -bestAxis;
                }
            }
        }
        
        if (!hasCollision) return false;
        
        // If we get here, there is a collision
        collisionNormal = glm::vec3(bestAxis.x, bestAxis.y, 0.0f);
        penetrationDepth = minPenetration;
        
        // Calculate contact points
        // Use the centroid of the overlap region as an approximation
        glm::vec2 rectCenter = std::accumulate(myVertices.begin(), myVertices.end(), glm::vec2(0.0f)) / 
                               static_cast<float>(myVertices.size());
        glm::vec2 polyCenter = std::accumulate(polyVertices.begin(), polyVertices.end(), glm::vec2(0.0f)) / 
                               static_cast<float>(polyVertices.size());
        
        collisionPoint = glm::vec3((rectCenter + polyCenter) * 0.5f, 0.0f);
        
        return true;
    }
}