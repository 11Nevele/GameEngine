#include "acpch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Math/Transform.h"

namespace ac
{
    SphereCollider::SphereCollider() : radius(0.5f)
    {
    }

    SphereCollider::SphereCollider(float _radius) : radius(_radius)
    {
    }

    SphereCollider::SphereCollider(float _radius, const glm::vec3& _offset) : radius(_radius)
    {
        offset = _offset;
    }

    bool SphereCollider::CheckCollision(
        const Collider* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Check the type of the other collider and dispatch to the appropriate collision function
        if (const SphereCollider* sphere = dynamic_cast<const SphereCollider*>(other))
        {
            return SphereVsSphere(sphere, myTransform, otherTransform, collisionPoint, collisionNormal, penetrationDepth);
        }
        else if (const BoxCollider* box = dynamic_cast<const BoxCollider*>(other))
        {
            return SphereVsBox(box, myTransform, otherTransform, collisionPoint, collisionNormal, penetrationDepth);
        }

        return false;
    }

    bool SphereCollider::SphereVsSphere(
        const SphereCollider* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get world positions of both spheres
        glm::vec3 myPos = GetWorldPosition(myTransform);
        glm::vec3 otherPos = other->GetWorldPosition(otherTransform);
        
        // Calculate scaled radii (assuming uniform scale)
        float myScaledRadius = radius * glm::max(myTransform.scale.x, glm::max(myTransform.scale.y, myTransform.scale.z));
        float otherScaledRadius = other->radius * glm::max(otherTransform.scale.x, glm::max(otherTransform.scale.y, otherTransform.scale.z));

        // Calculate the distance between sphere centers
        glm::vec3 direction = otherPos - myPos;
        float distance = glm::length(direction);
        
        // Sum of radii
        float radiusSum = myScaledRadius + otherScaledRadius;
        
        // Check for collision
        if (distance >= radiusSum)
            return false;
        
        // Normalize direction for the collision normal
        collisionNormal = direction;
        if (distance > 0.0001f)
            collisionNormal /= distance;
        else // If spheres are at the same position, use arbitrary normal
            collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            
        penetrationDepth = radiusSum - distance;
        
        // Calculate collision point halfway between sphere surfaces
        collisionPoint = myPos + collisionNormal * (myScaledRadius - penetrationDepth * 0.5f);
        
        return true;
    }

    bool SphereCollider::SphereVsBox(
        const BoxCollider* box,
        const Transform& myTransform,
        const Transform& boxTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get world positions
        glm::vec3 spherePos = GetWorldPosition(myTransform);
        glm::vec3 boxPos = box->GetWorldPosition(boxTransform);
        
        // Get scaled radius
        float scaledRadius = radius * glm::max(myTransform.scale.x, glm::max(myTransform.scale.y, myTransform.scale.z));
        
        // Get scaled box half-size
        glm::vec3 boxHalfSize = box->halfSize * boxTransform.scale;
        
        // For simplicity, we'll use an AABB approach
        // Find the closest point on the box to the sphere center
        glm::vec3 closestPoint;
        
        // For each axis, clamp the sphere's position to the box's bounds
        for (int i = 0; i < 3; i++)
        {
            float v = spherePos[i];
            if (v < boxPos[i] - boxHalfSize[i]) v = boxPos[i] - boxHalfSize[i];
            if (v > boxPos[i] + boxHalfSize[i]) v = boxPos[i] + boxHalfSize[i];
            closestPoint[i] = v;
        }
        
        // Vector from closest point to sphere center
        glm::vec3 dir = spherePos - closestPoint;
        float distSq = glm::dot(dir, dir);
        
        // Check if the closest point is within the sphere's radius
        if (distSq > scaledRadius * scaledRadius)
            return false;
            
        // Calculate the distance and penetration
        float dist = glm::sqrt(distSq);
        
        // If sphere center is inside the box, we need to find the closest face
        if (dist < 0.0001f)
        {
            // Find closest face by checking distances to each face
            float minDist = FLT_MAX;
            int minAxis = -1;
            int minDir = 0;
            
            for (int i = 0; i < 3; i++)
            {
                float d1 = std::abs((boxPos[i] + boxHalfSize[i]) - spherePos[i]);
                float d2 = std::abs(spherePos[i] - (boxPos[i] - boxHalfSize[i]));
                
                if (d1 < minDist)
                {
                    minDist = d1;
                    minAxis = i;
                    minDir = 1;
                }
                if (d2 < minDist)
                {
                    minDist = d2;
                    minAxis = i;
                    minDir = -1;
                }
            }
            
            // Set normal based on closest face
            collisionNormal = glm::vec3(0.0f);
            collisionNormal[minAxis] = (float)minDir;
            
            // Set penetration depth
            penetrationDepth = minDist + scaledRadius;
        }
        else
        {
            // Normal points from the box to the sphere
            collisionNormal = dir / dist;
            penetrationDepth = scaledRadius - dist;
        }
        
        // Collision point is on the sphere surface in the direction of the normal
        collisionPoint = spherePos - collisionNormal * scaledRadius;
        
        return true;
    }
}