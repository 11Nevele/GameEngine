#include "acpch.h"
#include "CollisionLayer.h"

namespace ac
{
    CollisionLayer::CollisionLayer()
    {
        // Initialize all layers to collide with each other
        for (uint32_t i = 0; i < MAX_COLLISION_LAYERS; ++i)
        {
            for (uint32_t j = 0; j < MAX_COLLISION_LAYERS; ++j)
            {
                m_collisionMatrix[i][j] = true;
            }
            
            // Set default layer names
            m_layerNames[i] = "Layer " + std::to_string(i);
        }
        
        // Set common layer names
        m_layerNames[0] = "Default";
        m_layerNames[1] = "Static";
        m_layerNames[2] = "Dynamic";
        m_layerNames[3] = "Player";
        m_layerNames[4] = "Enemy";
        m_layerNames[5] = "Projectile";
        m_layerNames[6] = "Trigger";
    }
    
    void CollisionLayer::SetLayerCollision(uint32_t layer1, uint32_t layer2, bool shouldCollide)
    {
        if (layer1 < MAX_COLLISION_LAYERS && layer2 < MAX_COLLISION_LAYERS)
        {
            m_collisionMatrix[layer1][layer2] = shouldCollide;
            m_collisionMatrix[layer2][layer1] = shouldCollide;
        }
    }
    
    bool CollisionLayer::ShouldCollide(uint32_t layer1, uint32_t layer2) const
    {
        if (layer1 < MAX_COLLISION_LAYERS && layer2 < MAX_COLLISION_LAYERS)
        {
            return m_collisionMatrix[layer1][layer2];
        }
        return false;
    }
    
    const std::string& CollisionLayer::GetLayerName(uint32_t index) const
    {
        if (index < MAX_COLLISION_LAYERS)
        {
            return m_layerNames[index];
        }
        
        static const std::string invalidLayer = "Invalid Layer";
        return invalidLayer;
    }
    
    void CollisionLayer::SetLayerName(uint32_t index, const std::string& name)
    {
        if (index < MAX_COLLISION_LAYERS)
        {
            m_layerNames[index] = name;
        }
    }
}