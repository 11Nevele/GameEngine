#pragma once
#include <unordered_map>
#include <Render/Render.h>
#include <stb_image.h>
#include <string>
namespace ac
{
	class TextureManager
	{
	public:
		const OpenGLTexture2D& GetTexture(const std::string& name);
		uint32_t GetTextureID(const std::string& name);
		const OpenGLTexture2D& GetTexture(uint32_t id);
		TextureManager& AddTexture(const std::string& name, const std::string& path);
	private:
		std::unordered_map<std::string, uint32_t> textureNameToID;
		std::vector<OpenGLTexture2D> textureList;
	};

	class ModelManager
	{
	public:
		ModelManager();
		OpenGLVertexArray& GetModel(const std::string& name);
		uint32_t GetModelID(const std::string& name);
		OpenGLVertexArray& GetModel(uint32_t id);
		ModelManager& AddModel(const std::string& name, const std::string& path);
	private:
		std::unordered_map<std::string, uint32_t> modelNameToID;
		std::vector<OpenGLVertexArray> modelList;
	};
}


