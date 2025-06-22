#pragma once
#include <unordered_map>
#include <Render/Render.h>
#include <stb_image.h>
#include <string>
namespace ac
{
	/**
	 * @brief Manages texture assets in the game engine.
	 * 
	 * The TextureManager class provides functionality for loading, storing, and retrieving
	 * texture assets. It maintains an internal registry of textures that can be accessed
	 * by name or ID.
	 */
	class TextureManager
	{
	public:
		/**
		 * @brief Retrieves a texture by its name.
		 * 
		 * @param name The unique identifier for the texture.
		 * @return const OpenGLTexture2D& A reference to the requested texture.
		 */
		const OpenGLTexture2D& GetTexture(const std::string& name);
		
		/**
		 * @brief Gets the internal ID of a texture by its name.
		 * 
		 * @param name The unique identifier for the texture.
		 * @return uint32_t The internal ID of the texture.
		 */
		uint32_t GetTextureID(const std::string& name);
		
		/**
		 * @brief Retrieves a texture by its internal ID.
		 * 
		 * @param id The internal ID of the texture.
		 * @return const OpenGLTexture2D& A reference to the requested texture.
		 */
		const OpenGLTexture2D& GetTexture(uint32_t id);
		
		/**
		 * @brief Loads and adds a new texture to the manager.
		 * 
		 * @param name The unique identifier to assign to the texture.
		 * @param path The file path where the texture is located.
		 * @return TextureManager& Reference to this manager for method chaining.
		 */

		void AddReference(uint32_t id);
		void AddReference(const std::string& name);
		void DeleteReference(uint32_t id);
		void DeleteReference(const std::string& name);
		TextureManager& AddTexture(const std::string& name, const std::string& path);
	private:
		std::unordered_map<std::string, uint32_t> textureNameToID; ///< Maps texture names to their internal IDs
		std::vector<OpenGLTexture2D> textureList;                  ///< Stores all loaded textures
		std::vector<uint32_t> referenceCount;
	};

	/**
	 * @brief Manages 3D models in the game engine.
	 * 
	 * The ModelManager class provides functionality for loading, storing, and retrieving
	 * 3D model assets. It maintains an internal registry of models that can be accessed
	 * by name or ID.
	 */
	class ModelManager
	{
	public:
		/**
		 * @brief Constructor for the ModelManager.
		 * 
		 * Initializes the model manager and prepares it for use.
		 */
		ModelManager();
		
		/**
		 * @brief Retrieves a model by its name.
		 * 
		 * @param name The unique identifier for the model.
		 * @return OpenGLVertexArray& A reference to the requested model.
		 */
		OpenGLVertexArray& GetModel(const std::string& name);
		
		/**
		 * @brief Gets the internal ID of a model by its name.
		 * 
		 * @param name The unique identifier for the model.
		 * @return uint32_t The internal ID of the model.
		 */
		uint32_t GetModelID(const std::string& name);
		
		/**
		 * @brief Retrieves a model by its internal ID.
		 * 
		 * @param id The internal ID of the model.
		 * @return OpenGLVertexArray& A reference to the requested model.
		 */
		OpenGLVertexArray& GetModel(uint32_t id);
		
		/**
		 * @brief Loads and adds a new model to the manager.
		 * 
		 * @param name The unique identifier to assign to the model.
		 * @param path The file path where the model is located.
		 * @return ModelManager& Reference to this manager for method chaining.
		 */
		ModelManager& AddModel(const std::string& name, const std::string& path);

		void AddReference(uint32_t id);
		void AddReference(const std::string& name);
		void DeleteReference(uint32_t id);
		void DeleteReference(const std::string& name);
	private:
		std::unordered_map<std::string, uint32_t> modelNameToID; ///< Maps model names to their internal IDs
		std::vector<OpenGLVertexArray> modelList;               ///< Stores all loaded models
		std::vector<uint32_t> referenceCount;
	};
}


