#include "acpch.h"  
#include "AssetManager.h"  
#include "Util/util.h"  
#include "Debug.h"  

namespace ac  
{  
/// Retrieves a texture by name.  
/// If the texture is not uploaded, it uploads the texture before returning it.  
/// @param name The name of the texture.  
/// @return A reference to the requested texture.  
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

/// Retrieves the ID of a texture by name.  
/// @param name The name of the texture.  
/// @return The ID of the texture.  
uint32_t ac::TextureManager::GetTextureID(const std::string& name)  
{  
	auto it = textureNameToID.find(name);  
	ACASSERT(it != textureNameToID.end(), "Fail to find texture: " << name);  
	return it->second;  
}  

/// Retrieves a texture by ID.  
/// If the texture is not uploaded, it uploads the texture before returning it.  
/// @param id The ID of the texture.  
/// @return A reference to the requested texture.  
const OpenGLTexture2D& ac::TextureManager::GetTexture(uint32_t id)  
{  
	ACASSERT(id < textureList.size(), "ID out of bound at id: " << id);  
	if (!textureList[id].IsUploaded())  
	{  
		textureList[id].Upload();  
	}  
	return textureList[id];  
}  

void TextureManager::AddReference(uint32_t id)
{
	ACASSERT(id < textureList.size(), "ID out of bound at texture"
		 << " id: " << id);
	if (referenceCount[id] == 0)
	{
		textureList[id].Upload();
	}
	referenceCount[id]++;
	return;
}

void TextureManager::AddReference(const std::string& name)
{
	uint32_t id = GetTextureID(name);
	ACASSERT(id < textureList.size(), "ID out of bound at texture"
		<< name << " id: " << id);
	if (referenceCount[id] == 0)
	{
		//textureList[id].Upload();
	}
	referenceCount[id]++;
	return;
}

void TextureManager::DeleteReference(uint32_t id)
{
	ACASSERT(id < textureList.size(), "ID out of bound at texture"
		<< " id: " << id);
	referenceCount[id]--;
	if (referenceCount[id] == 0)
	{
		//textureList[id].Delete();
	}
	return;
}

void TextureManager::DeleteReference(const std::string& name)
{
	uint32_t id = GetTextureID(name);
	ACASSERT(id < textureList.size(), "ID out of bound at texture"
		<< name << " id: " << id);
	referenceCount[id]--;
	if (referenceCount[id] == 0)
	{
		//textureList[id].Delete();
	}
	return;
}

/// Adds a new texture to the manager.  
/// Reads the texture data from the specified path and stores it.  
/// @param name The name of the texture.  
/// @param path The file path to the texture.  
/// @return A reference to the TextureManager instance.  
TextureManager& ac::TextureManager::AddTexture(const std::string& name, const std::string& path)  
{  
	uint32_t id = textureList.size();  
	textureNameToID[name] = id;  
	TextureInfo info;  
	int width, height, channel;  
	stbi_set_flip_vertically_on_load(true);
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
	referenceCount.emplace_back(0);
	textureList.back().Upload();
	return *this;  
}  



/// Constructor for the ModelManager class.  
/// Initializes the manager with a default "Plane" model.  
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
	
	referenceCount.emplace_back(0);
}  

/// Retrieves a model by name.  
/// If the model is not uploaded, it uploads the model before returning it.  
/// @param name The name of the model.  
/// @return A reference to the requested model.  
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

/// Retrieves the ID of a model by name.  
/// @param name The name of the model.  
/// @return The ID of the model.  
uint32_t ModelManager::GetModelID(const std::string& name)  
{  
	auto it = modelNameToID.find(name);  
	ACASSERT(it != modelNameToID.end(), "Fail to find Model: " << name);  
	return it->second;  
}  

/// Retrieves a model by ID.  
/// If the model is not uploaded, it uploads the model before returning it.  
/// @param id The ID of the model.  
/// @return A reference to the requested model.  
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

/// Adds a new model to the manager.  
/// Currently, this function is a placeholder and does not perform any operations.  
/// @param name The name of the model.  
/// @param path The file path to the model.  
/// @return A reference to the ModelManager instance.  
ModelManager& ModelManager::AddModel(const std::string& name, const std::string& path)  
{  
	return *this;  
	// TODO: insert return statement here  
}  
void ModelManager::AddReference(uint32_t id)
{
	ACASSERT(id < modelList.size(), "ID out of bound at texture"
		<< " id: " << id);
	if (referenceCount[id] == 0)
	{
		modelList[id].Upload();
	}
	referenceCount[id]++;
	return;
}

void ModelManager::AddReference(const std::string& name)
{
	uint32_t id = GetModelID(name);
	ACASSERT(id < modelList.size(), "ID out of bound at texture"
		<< name << " id: " << id);
	if (referenceCount[id] == 0)
	{
		modelList[id].Upload();
	}
	referenceCount[id]++;
	return;
}

void ModelManager::DeleteReference(uint32_t id)
{
	ACASSERT(id < modelList.size(), "ID out of bound at texture"
		<< " id: " << id);
	referenceCount[id]--;
	if (referenceCount[id] == 0)
	{
		modelList[id].Delete();
	}
	return;
}

void ModelManager::DeleteReference(const std::string& name)
{
	uint32_t id = GetModelID(name);
	ACASSERT(id < modelList.size(), "ID out of bound at texture"
		<< name << " id: " << id);
	referenceCount[id]--;
	if (referenceCount[id] == 0)
	{
		modelList[id].Delete();
	}
	return;
}

}
