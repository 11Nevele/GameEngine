#include "acpch.h"
#include "Collider2D.h"
#include "Math/Transform.h"

namespace ac
{
    Collider2D::Collider2D()
        : offset(0.0f)
        , isTrigger(false)
        , layer(0)
    {
    }
    
    Collider2D::Collider2D(uint32_t _layer)
        : offset(0.0f)
        , isTrigger(false)
        , layer(_layer)
    {
    }
    
    glm::vec2 Collider2D::GetWorldPosition(const Transform& transform) const
    {
        // Apply entity's transform to the collider's local offset
        return transform.position + transform.rotation * glm::vec3(offset, 0);
    }
}