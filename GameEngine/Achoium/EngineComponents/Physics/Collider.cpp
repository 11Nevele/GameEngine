#include "acpch.h"
#include "Collider.h"
#include "Math/Transform.h"

namespace ac
{
    Collider::Collider()
        : offset(0.0f)
        , isTrigger(false)
        , layer(0)
    {
    }
    
    Collider::Collider(uint32_t _layer)
        : offset(0.0f)
        , isTrigger(false)
        , layer(_layer)
    {
    }
    
    glm::vec3 Collider::GetWorldPosition(const Transform& transform) const
    {
        // Apply entity's transform to the collider's local offset
        return transform.position + transform.rotation * offset;
    }
}