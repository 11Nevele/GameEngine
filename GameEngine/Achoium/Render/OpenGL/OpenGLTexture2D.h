#pragma once
#include "Render/Texture.h"
#include <stb_image.h>
namespace ac
{
	class OpenGLTexture2D :
		public Texture2D
	{
	public:
		OpenGLTexture2D() = default;
		OpenGLTexture2D(OpenGLTexture2D&& other)noexcept;
		OpenGLTexture2D(const std::string& path) ;
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D(stbi_uc* data, TextureInfo info);
		void SetData(stbi_uc* data, TextureInfo info);
		virtual ~OpenGLTexture2D();
		virtual void Upload() override;
		virtual void Delete() override;
		virtual bool IsUploaded() const override;
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual TextureInfo GetTextureInfo() const override;
		virtual void Bind(uint32_t slot = 0) const override;

	private:
		stbi_uc* data;
		uint32_t m_RenderID;
		TextureInfo textureInfo;
	};
}


