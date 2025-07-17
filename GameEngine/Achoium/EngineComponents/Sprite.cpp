#include "acpch.h"
#include "Sprite.h"
#include "Util/util.h"
namespace ac
{

	Sprite::Sprite(uint32_t textureID, uint32_t width, uint32_t height, const glm::vec4& color):
		textureID(textureID),width(width), height(height), color(color)
	{
	}
	Sprite Sprite::Create(const std::string& name, TextureManager& textureManager)
	{
		uint32_t id = textureManager.GetTextureID(name);
		auto& t = textureManager.GetTexture(id);

		return Sprite(id, t.GetWidth(), t.GetHeight());
	}
}

