#include "acpch.h"
#include "CircleCollider2D.h"
#include "RectCollider2D.h"
#include "PolygonCollider2D.h"
#include "Math/Transform.h"
#include <glm/gtx/norm.hpp>

namespace ac
{
    CircleCollider2D::CircleCollider2D()
        : Collider()
        , radius(0.5f)
    {
    }
    
    CircleCollider2D::CircleCollider2D(float _radius)
        : Collider()
        , radius(_radius)
    {
    }
    
    CircleCollider2D::CircleCollider2D(float _radius, const glm::vec3& _offset, uint32_t _layer, bool _isTrigger)
        : Collider(_layer)
        , radius(_radius)
    {
        offset = _offset;
        isTrigger = _isTrigger;
    }
    
    bool CircleCollider2D::CheckCollision(
        const Collider* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Check what type of collider we're colliding with and delegate to specific function
        if (const CircleCollider2D* otherCircle = dynamic_cast<const CircleCollider2D*>(other))
        {
            return CircleVsCircle(otherCircle, myTransform, otherTransform, 
                collisionPoint, collisionNormal, penetrationDepth);
        }
        else if (const RectCollider2D* otherRect = dynamic_cast<const RectCollider2D*>(other))
        {
            return CircleVsRect(otherRect, myTransform, otherTransform, 
                collisionPoint, collisionNormal, penetrationDepth);
        }
        else if (const PolygonCollider2D* otherPolygon = dynamic_cast<const PolygonCollider2D*>(other))
        {
            return CircleVsPolygon(otherPolygon, myTransform, otherTransform, 
                collisionPoint, collisionNormal, penetrationDepth);
        }
        
        // Unsupported collider type
        return false;
    }
    
    bool CircleCollider2D::CircleVsCircle(
        const CircleCollider2D* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        glm::vec3 myPos = GetWorldPosition(myTransform);
        glm::vec3 otherPos = other->GetWorldPosition(otherTransform);
        
        // For 2D, we only care about the XY plane
        glm::vec2 myPos2D(myPos.x, myPos.y);
        glm::vec2 otherPos2D(otherPos.x, otherPos.y);
        
        // Calculate distance vector between centers in 2D
        glm::vec2 distance = otherPos2D - myPos2D;
        
        // Calculate squared distance to avoid square root
        float distanceSq = glm::length2(distance);
        
        // Get the sum of the radii
        float sumRadii = radius + other->radius;
        float sumRadiiSq = sumRadii * sumRadii;
        
        // Collision occurs if the distance between centers is less than the sum of radii
        if (distanceSq >= sumRadiiSq)
        {
            // No collision
            return false;
        }
        
        // We have a collision
        float dist = glm::sqrt(distanceSq);
        
        // Handle the case where the circles are at exactly the same position
        if (dist < 0.0001f)
        {
            collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f); // Default to up direction
            collisionPoint = myPos;
            penetrationDepth = sumRadii;
        }
        else
        {
            // Calculate collision normal
            glm::vec2 normalDir = distance / dist;
            collisionNormal = glm::vec3(normalDir.x, normalDir.y, 0.0f);
            
            // Calculate penetration depth
            penetrationDepth = sumRadii - dist;
            
            // Calculate collision point
            glm::vec2 collisionPoint2D = myPos2D + normalDir * radius;
            collisionPoint = glm::vec3(collisionPoint2D.x, collisionPoint2D.y, myPos.z);
        }
        
        return true;
    }
    
    bool CircleCollider2D::CircleVsRect(
        const RectCollider2D* rect,
        const Transform& myTransform,
        const Transform& rectTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get world positions
        glm::vec3 circleCenter = GetWorldPosition(myTransform);
        glm::vec3 rectCenter = rect->GetWorldPosition(rectTransform);
        
        // Convert to local rectange space
        glm::mat4 rectWorldToLocal = glm::inverse(rectTransform.asMat4());
        glm::vec4 circleCenterLocal = rectWorldToLocal * glm::vec4(circleCenter, 1.0f);
        
        // Find the closest point on the rectangle to the circle
        glm::vec3 closestPoint;
        closestPoint.x = glm::clamp(circleCenterLocal.x, -rect->halfSize.x, rect->halfSize.x);
        closestPoint.y = glm::clamp(circleCenterLocal.y, -rect->halfSize.y, rect->halfSize.y);
        closestPoint.z = circleCenterLocal.z; // Maintain Z coordinate
        
        // Calculate the distance from the closest point to the circle center in local space
        glm::vec3 difference = glm::vec3(circleCenterLocal) - closestPoint;
        float distanceSq = glm::length2(difference);
        
        // If the distance is less than the circle's radius, we have a collision
        if (distanceSq > radius * radius)
        {
            return false;
        }
        
        // Convert the closest point back to world space
        glm::vec4 closestPointWorld = rectTransform.asMat4() * glm::vec4(closestPoint, 1.0f);
        
        // If the circle center is inside the rectangle, we need to find the shortest exit direction
        if (distanceSq < 0.0001f)
        {
            // Find which rectangle edge is closest
            float dx = rect->halfSize.x - std::abs(circleCenterLocal.x);
            float dy = rect->halfSize.y - std::abs(circleCenterLocal.y);
            
            if (dx < dy)
            {
                // X axis is the shortest exit
                float sign = circleCenterLocal.x < 0.0f ? -1.0f : 1.0f;
                closestPoint.x = sign * rect->halfSize.x;
                difference = glm::vec3(circleCenterLocal) - closestPoint;
            }
            else
            {
                // Y axis is the shortest exit
                float sign = circleCenterLocal.y < 0.0f ? -1.0f : 1.0f;
                closestPoint.y = sign * rect->halfSize.y;
                difference = glm::vec3(circleCenterLocal) - closestPoint;
            }
            
            // Recalculate world position
            closestPointWorld = rectTransform.asMat4() * glm::vec4(closestPoint, 1.0f);
        }
        
        float distance = glm::sqrt(distanceSq);
        
        // Calculate collision normal (from rectangle to circle)
        if (distance < 0.0001f)
        {
            collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f); // Default up
        }
        else
        {
            // Calculate the collision normal in world space
            glm::vec3 localNormal = glm::normalize(difference);
            glm::vec4 worldNormal = glm::transpose(rectWorldToLocal) * glm::vec4(localNormal, 0.0f);
            collisionNormal = glm::normalize(glm::vec3(worldNormal));
        }
        
        // Calculate the collision point
        collisionPoint = glm::vec3(closestPointWorld);
        
        // Calculate the penetration depth
        penetrationDepth = radius - distance;
        
        return true;
    }
    
    bool CircleCollider2D::CircleVsPolygon(
        const PolygonCollider2D* polygon,
        const Transform& myTransform,
        const Transform& polygonTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get the circle's world position
        glm::vec3 circleCenter = GetWorldPosition(myTransform);
        
        // Convert circle center to polygon's local space
        glm::mat4 worldToLocal = glm::inverse(polygonTransform.asMat4());
        glm::vec4 circleCenterLocal = worldToLocal * glm::vec4(circleCenter, 1.0f);
        
        // Find the closest point on the polygon to the circle center
        glm::vec2 closestPoint2D = polygon->FindClosestPoint(glm::vec2(circleCenterLocal.x, circleCenterLocal.y));
        
        // Calculate distance from closest point to circle center
        glm::vec2 difference = glm::vec2(circleCenterLocal.x, circleCenterLocal.y) - closestPoint2D;
        float distanceSq = glm::length2(difference);
        
        // If the distance is greater than the radius squared, no collision
        if (distanceSq > radius * radius)
        {
            return false;
        }
        
        // Compute actual distance
        float distance = glm::sqrt(distanceSq);
        
        // Convert closest point back to world space
        glm::vec4 closestPointWorld = polygonTransform.asMat4() * glm::vec4(closestPoint2D.x, closestPoint2D.y, 0.0f, 1.0f);
        
        // Calculate collision normal (from polygon to circle)
        if (distance < 0.0001f)
        {
            // Circle center is on the polygon perimeter or inside, find the penetration normal
            collisionNormal = polygon->FindPenetrationNormal(glm::vec2(circleCenterLocal.x, circleCenterLocal.y), polygonTransform);
        }
        else
        {
            collisionNormal = glm::normalize(glm::vec3(circleCenter) - glm::vec3(closestPointWorld));
        }
        
        // Calculate collision data
        collisionPoint = glm::vec3(closestPointWorld);
        penetrationDepth = radius - distance;
        
        return true;
    }
}