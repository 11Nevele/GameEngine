#pragma once
#include <string>
namespace ac
{
	struct TextureInfo
	{
		uint32_t width;
		uint32_t height;
		GLenum dataFormat;
		GLenum internalFormat;
	};

	class Texture
	{
	public:
		Texture() = default;
		Texture(const Texture& other) = delete;
		virtual ~Texture() = default;
		virtual void Upload() = 0;
		virtual void Delete() = 0;
		virtual bool IsUploaded() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual TextureInfo GetTextureInfo() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{

	};
}


