#include "acpch.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Math/Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <algorithm>

namespace ac
{
    BoxCollider::BoxCollider() : halfSize(0.5f, 0.5f, 0.5f)
    {
    }

    BoxCollider::BoxCollider(float width, float height, float depth)
        : halfSize(width * 0.5f, height * 0.5f, depth * 0.5f)
    {
    }

    BoxCollider::BoxCollider(const glm::vec3& size, const glm::vec3& _offset)
        : halfSize(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
    {
        offset = _offset;
    }

    bool BoxCollider::CheckCollision(
        const Collider* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Check the type of the other collider and dispatch to the appropriate collision function
        if (const BoxCollider* box = dynamic_cast<const BoxCollider*>(other))
        {
            return BoxVsBox(box, myTransform, otherTransform, collisionPoint, collisionNormal, penetrationDepth);
        }
        else if (const SphereCollider* sphere = dynamic_cast<const SphereCollider*>(other))
        {
            return BoxVsSphere(sphere, myTransform, otherTransform, collisionPoint, collisionNormal, penetrationDepth);
        }

        return false;
    }

    bool BoxCollider::BoxVsBox(
        const BoxCollider* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Implementation of the Separating Axis Theorem (SAT) for OBBs
        // For simplicity, we'll do an AABB check using world coordinates
        // A more accurate implementation would consider rotations

        // Get world positions
        glm::vec3 myPos = GetWorldPosition(myTransform);
        glm::vec3 otherPos = other->GetWorldPosition(otherTransform);
        
        // Apply scale
        glm::vec3 myHalfSize = halfSize * myTransform.scale;
        glm::vec3 otherHalfSize = other->halfSize * otherTransform.scale;

        // Calculate min/max for both boxes
        glm::vec3 myMin = myPos - myHalfSize;
        glm::vec3 myMax = myPos + myHalfSize;
        glm::vec3 otherMin = otherPos - otherHalfSize;
        glm::vec3 otherMax = otherPos + otherHalfSize;

        // Check for overlap
        if (myMin.x > otherMax.x || myMax.x < otherMin.x ||
            myMin.y > otherMax.y || myMax.y < otherMin.y ||
            myMin.z > otherMax.z || myMax.z < otherMin.z)
        {
            return false; // No collision
        }

        // Find the axis with minimal penetration
        float penX = std::min(myMax.x - otherMin.x, otherMax.x - myMin.x);
        float penY = std::min(myMax.y - otherMin.y, otherMax.y - myMin.y);
        float penZ = std::min(myMax.z - otherMin.z, otherMax.z - myMin.z);

        // Choose the axis with the smallest penetration
        penetrationDepth = std::min(std::min(penX, penY), penZ);
        
        // Calculate normal based on the minimal penetration axis
        // IMPORTANT: We need to ensure the normal points from B to A (not from A to B)
        // This ensures that when objects move toward each other, velocityAlongNormal is negative
        if (penetrationDepth == penX)
        {
            // Check which face has the contact - use direction from B to A
            bool usePositiveX = (otherPos.x < myPos.x);
            collisionNormal = usePositiveX ? glm::vec3(1, 0, 0) : glm::vec3(-1, 0, 0);
        }
        else if (penetrationDepth == penY)
        {
            bool usePositiveY = (otherPos.y < myPos.y);
            collisionNormal = usePositiveY ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0);
        }
        else
        {
            bool usePositiveZ = (otherPos.z < myPos.z);
            collisionNormal = usePositiveZ ? glm::vec3(0, 0, 1) : glm::vec3(0, 0, -1);
        }

        // Calculate approximate collision point (center of intersection volume)
        glm::vec3 intersectionMin = glm::max(myMin, otherMin);
        glm::vec3 intersectionMax = glm::min(myMax, otherMax);
        collisionPoint = (intersectionMin + intersectionMax) * 0.5f;

        return true;
    }

    bool BoxCollider::BoxVsSphere(
        const SphereCollider* sphere,
        const Transform& myTransform,
        const Transform& sphereTransform,
        glm::vec3& collisionPoint,
        glm::vec3& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Let the SphereCollider handle this collision check
        // Reverse the parameters so the normal is correct
        bool result = sphere->CheckCollision(
            this, 
            sphereTransform, 
            myTransform, 
            collisionPoint, 
            collisionNormal, 
            penetrationDepth
        );
        
        // Reverse the normal since we flipped the parameters
        if (result)
            collisionNormal = -collisionNormal;
            
        return result;
    }
}