#include "acpch.h"

#include "CircleCollider2D.h"
#include "RectCollider2D.h"
#include "PolygonCollider2D.h"
#include "Math/Transform.h"
#include <glm/gtx/norm.hpp>

namespace ac
{
    CircleCollider2D::CircleCollider2D()
        : Collider2D()
        , radius(0.5f)
    {
    }
    
    CircleCollider2D::CircleCollider2D(float _radius)
        : Collider2D()
        , radius(_radius)
    {
    }
    
    CircleCollider2D::CircleCollider2D(float _radius, const glm::vec2& _offset, uint32_t _layer, bool _isTrigger)
        : Collider2D(_layer)
        , radius(_radius)
    {
        offset = _offset;
        isTrigger = _isTrigger;
    }
    
    bool CircleCollider2D::CheckCollision(
        const Collider2D* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        std::vector<CollisionPoint2D>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Check what type of collider we're colliding with and delegate to specific function
        if (const CircleCollider2D* otherCircle = dynamic_cast<const CircleCollider2D*>(other))
        {
            return CircleVsCircle(otherCircle, myTransform, otherTransform, 
                collisionPoints, collisionNormal, penetrationDepth);
        }
        else if (const RectCollider2D* otherRect = dynamic_cast<const RectCollider2D*>(other))
        {
            bool res =  otherRect->RectVsCircle(this, otherTransform, myTransform,
                collisionPoints, collisionNormal, penetrationDepth);
            for (auto& t : collisionPoints)
                std::swap(t.rbA, t.rbB);
            collisionNormal = -collisionNormal;
            return res;
        }
        else if (const PolygonCollider2D* otherPolygon = dynamic_cast<const PolygonCollider2D*>(other))
        {
            bool res = otherPolygon->PolygonVsCircle(this, otherTransform, myTransform,
                collisionPoints, collisionNormal, penetrationDepth);
            for (auto& t : collisionPoints)
                std::swap(t.rbA, t.rbB);
            collisionNormal = -collisionNormal;
            return res;
        }
        
        // Unsupported collider type
        return false;
    }
    
    bool CircleCollider2D::CircleVsCircle(
        const CircleCollider2D* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        std::vector<CollisionPoint2D>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        glm::vec myPos = GetWorldPosition(myTransform);
        glm::vec otherPos = other->GetWorldPosition(otherTransform);
        
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
            penetrationDepth = sumRadii;
            collisionPoints.push_back({ myPos, otherPos });
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
            collisionPoints.push_back({ myPos2D + normalDir * radius, otherPos2D + -normalDir * other->radius });
        }
        
        return true;
    }
}