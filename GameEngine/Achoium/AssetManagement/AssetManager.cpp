#include "acpch.h"
#include "AssetManager.h"
#include "Util/util.h"
#include "Debug.h"
namespace ac
{

	const OpenGLTexture2D& ac::TextureManager::GetTexture(const std::string& name)
	{
		uint32_t id = GetTextureID(name);
		ACASSERT(id < textureList.size(), "ID out of bound at texture"
			<< name << " id: " << id);
		if (!textureList[id].IsUploaded())
		{
			textureList[id].Upload();
		}
		return textureList[id];

	}

	uint32_t ac::TextureManager::GetTextureID(const std::string& name)
	{
		auto it = textureNameToID.find(name);
		ACASSERT(it != textureNameToID.end(), "Fail to find texture: " << name);
		return it->second;
	}

	const OpenGLTexture2D& ac::TextureManager::GetTexture(uint32_t id)
	{
		ACASSERT(id < textureList.size(), "ID out of bound at id: " << id);
		if (!textureList[id].IsUploaded())
		{
			textureList[id].Upload();
		}
		return textureList[id];
	}

	TextureManager& ac::TextureManager::AddTexture(const std::string& name, const std::string& path)
	{
		uint32_t id = textureList.size();
		textureNameToID[name] = id;
		TextureInfo info;
		int width, height, channel;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channel, 0);
		info.height = height;
		info.width = width;
		if (channel == 4)
		{
			info.internalFormat = GL_RGBA8;
			info.dataFormat = GL_RGBA;
		}
		else if (channel == 3)
		{
			info.internalFormat = GL_RGB8;
			info.dataFormat = GL_RGB;
		}
		ACASSERT(data, "FAIL TO READ DATA FROM" << path);
		textureList.emplace_back(data, info);
		return *this;
	}
	ModelManager::ModelManager()
	{
		modelNameToID["Plane"] = 0;
		OpenGLVertexArray vao;
		vao.AddVertexBuffer(make_shared<OpenGLVertexBuffer>(OpenGLVertexBuffer(unique_ptr<float[]>(new float[] {
			0.0, 0.0, 0.0, 0.0, 0.0,
				1.0, 0.0, 0.0, 1.0, 0.0,
				1.0, 1.0, 0.0, 1.0, 1.0,
				0.0, 1.0, 0.0, 0.0, 1.0
			}), sizeof(float) * 20,
			{
				{ShaderDataType::Float3, "a"},
				{ShaderDataType::Float2, "b"}
			})));
			vao.SetIndexBuffer(make_shared< OpenGLIndexBuffer>(OpenGLIndexBuffer(unique_ptr<uint32_t[]>(new uint32_t[]
				{ 0, 1, 2,
				2, 3, 0 }), 6)));
			modelList.emplace_back(std::move(vao));
	}
	OpenGLVertexArray& ModelManager::GetModel(const std::string& name)
	{
		uint32_t id = GetModelID(name);
		ACASSERT(id < modelNameToID.size(), "ID out of bound at texture"
			<< name << " id: " << id);
		OpenGLVertexArray& m = modelList[id];
		if (!m.IsUploaded())
		{
			m.Upload();
		}
		return m;
	}
	uint32_t ModelManager::GetModelID(const std::string& name)
	{
		auto it = modelNameToID.find(name);
		ACASSERT(it != modelNameToID.end(), "Fail to find Model: " << name);
		return it->second;
	}
	OpenGLVertexArray& ModelManager::GetModel(uint32_t id)
	{
		ACASSERT(id < modelNameToID.size(), "ID out of bound at texture id: " << id);
		OpenGLVertexArray& m = modelList[id];
		if (!m.IsUploaded())
		{
			m.Upload();
		}
		return m;
	}
	ModelManager& ModelManager::AddModel(const std::string& name, const std::string& path)
	{
		return *this;
		// TODO: insert return statement here
	}
}

