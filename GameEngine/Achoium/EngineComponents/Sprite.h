#pragma once
#include <string>
#include "Render/Render.h"
#include "AssetManagement/AssetManager.h"
namespace ac
{
	/**
	 * @brief Represents a 2D graphic element in the game.
	 * 
	 * The Sprite struct encapsulates information about a 2D graphical element,
	 * including its dimensions and the texture to be displayed.
	 */
	struct Sprite
	{
	public:
		/**
		 * @brief Constructs a Sprite with specified texture and dimensions.
		 * 
		 * @param textureID Identifier for the texture to be used by this sprite.
		 * @param width The width of the sprite in pixels.
		 * @param height The height of the sprite in pixels.
		 */
		Sprite(uint32_t textureID, uint32_t width, uint32_t height, const glm::vec4& color = glm::vec4(1,1,1,1));

		
		uint32_t width;   ///< The width of the sprite in pixels.
		uint32_t height;  ///< The height of the sprite in pixels.
		uint32_t textureID; ///< Identifier for the texture used by this sprite.
		glm::vec4 color;

		/**
		 * @brief Creates a sprite by looking up a texture in the provided texture manager.
		 * 
		 * This factory method uses the name to look up the texture ID from the texture manager
		 * and creates a sprite with the appropriate dimensions.
		 * 
		 * @param name The name of the texture to use for this sprite.
		 * @param textureManager Reference to the texture manager containing the texture.
		 * @return Sprite A new sprite instance configured with the found texture.
		 */
		static Sprite Create(const std::string& name, TextureManager& textureManager);
	};
}

