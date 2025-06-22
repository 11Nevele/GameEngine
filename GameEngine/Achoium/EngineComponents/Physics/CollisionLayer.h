#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <vector>

namespace ac
{
    /**
     * @brief Maximum number of collision layers supported.
     */
    constexpr uint32_t MAX_COLLISION_LAYERS = 32;
    
    /**
     * @brief Resource that defines which collision layers interact with each other.
     */
    class CollisionLayer
    {
    public:
        /**
         * @brief Default constructor initializes all layers to collide with each other.
         */
        CollisionLayer();
        
        /**
         * @brief Sets whether two layers should collide with each other.
         * 
         * @param layer1 First layer
         * @param layer2 Second layer
         * @param shouldCollide True if the layers should collide
         */
        void SetLayerCollision(uint32_t layer1, uint32_t layer2, bool shouldCollide);
        
        /**
         * @brief Checks if two layers should collide with each other.
         * 
         * @param layer1 First layer
         * @param layer2 Second layer
         * @return True if the layers should collide
         */
        bool ShouldCollide(uint32_t layer1, uint32_t layer2) const;
        
        /**
         * @brief Gets a named layer by index.
         * 
         * @param index Layer index
         * @return Name of the layer
         */
        const std::string& GetLayerName(uint32_t index) const;
        
        /**
         * @brief Sets a name for a layer.
         * 
         * @param index Layer index
         * @param name Name to assign to the layer
         */
        void SetLayerName(uint32_t index, const std::string& name);
        
    private:
        // Collision matrix stored as a 32x32 array of booleans
        std::array<std::array<bool, MAX_COLLISION_LAYERS>, MAX_COLLISION_LAYERS> m_collisionMatrix;
        
        // Names for each layer
        std::array<std::string, MAX_COLLISION_LAYERS> m_layerNames;
    };
}