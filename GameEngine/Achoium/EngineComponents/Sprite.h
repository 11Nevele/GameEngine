#pragma once
#include <string>
#include "Render/Render.h"
#include "AssetManagement/AssetManager.h"
namespace ac
{
	struct Sprite
	{
	public:
		Sprite(uint32_t textureID, uint32_t width, uint32_t height);
		uint32_t width;
		uint32_t height;
		uint32_t textureID;

		static Sprite Create(const std::string& name, TextureManager& textureManager);
	};
}

