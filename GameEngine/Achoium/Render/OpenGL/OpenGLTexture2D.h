#pragma once
#include "Render/Texture.h"
#include <stb_image.h>
namespace ac
{
	/**
	 * @brief OpenGL implementation of a 2D texture.
	 * 
	 * This class provides the OpenGL-specific implementation of the abstract Texture2D interface.
	 * It handles loading, uploading and binding of 2D textures to be used in rendering.
	 */
	class OpenGLTexture2D :
		public Texture2D
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		OpenGLTexture2D() = default;
		
		/**
		 * @brief Move constructor.
		 * 
		 * @param other The texture to move from
		 */
		OpenGLTexture2D(OpenGLTexture2D&& other)noexcept;
		
		/**
		 * @brief Constructs a texture from an image file.
		 * 
		 * @param path Path to the image file
		 */
		OpenGLTexture2D(const std::string& path);
		
		/**
		 * @brief Copy constructor (deleted).
		 */
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		
		/**
		 * @brief Constructs a texture from raw pixel data.
		 * 
		 * @param data Pointer to the raw pixel data
		 * @param info Information about the texture dimensions and format
		 */
		OpenGLTexture2D(stbi_uc* data, TextureInfo info);
		
		/**
		 * @brief Sets or updates the texture data.
		 * 
		 * @param data Pointer to the raw pixel data
		 * @param info Information about the texture dimensions and format
		 */
		void SetData(stbi_uc* data, TextureInfo info);
		
		/**
		 * @brief Destructor. Cleans up texture resources.
		 */
		virtual ~OpenGLTexture2D();
		
		/**
		 * @brief Uploads the texture data to the GPU.
		 */
		virtual void Upload() override;
		
		/**
		 * @brief Deletes the texture from GPU memory.
		 */
		virtual void Delete() override;
		
		/**
		 * @brief Checks if the texture has been uploaded to GPU.
		 * 
		 * @return true If the texture is uploaded
		 * @return false If the texture is not uploaded
		 */
		virtual bool IsUploaded() const override;
		
		/**
		 * @brief Gets the width of the texture.
		 * 
		 * @return uint32_t The texture width in pixels
		 */
		virtual uint32_t GetWidth() const override;
		
		/**
		 * @brief Gets the height of the texture.
		 * 
		 * @return uint32_t The texture height in pixels
		 */
		virtual uint32_t GetHeight() const override;
		
		/**
		 * @brief Gets the texture information.
		 * 
		 * @return TextureInfo Structure containing texture dimensions and format
		 */
		virtual TextureInfo GetTextureInfo() const override;
		
		/**
		 * @brief Binds the texture to a texture unit for rendering.
		 * 
		 * @param slot The texture unit slot to bind to (default: 0)
		 */
		virtual void Bind(uint32_t slot = 0) const override;

	private:
		stbi_uc* data;        ///< Raw pixel data
		uint32_t m_RenderID;  ///< OpenGL handle to the texture
		TextureInfo textureInfo; ///< Information about texture dimensions and format
	};
}


