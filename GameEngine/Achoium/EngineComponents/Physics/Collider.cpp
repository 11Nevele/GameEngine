#include "acpch.h"
#include "Collider.h"
#include "Math/Transform.h"

namespace ac
{
    Collider::Collider() : offset(0.0f), isTrigger(false)
    {
    }

    glm::vec3 Collider::GetWorldPosition(const Transform& transform) const
    {
        // Apply the entity's transform to the offset
        glm::mat4 transformMatrix = transform.asMat4(false);
        glm::vec4 worldPos = transformMatrix * glm::vec4(offset, 1.0f);
        return glm::vec3(worldPos);
    }
}