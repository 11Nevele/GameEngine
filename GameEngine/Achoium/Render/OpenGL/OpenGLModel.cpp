#include "acpch.h"
#include "OpenGLModel.h"
namespace ac
{
	ac::OpenGLModel::OpenGLModel(const std::string& path)
	{
		loadModel(path);
	}

	std::vector<std::shared_ptr<Mesh>>& OpenGLModel::GetMeshes()
	{
		return meshes;
	}

	void ac::OpenGLModel::loadModel(string path)
	{
		std::unordered_map<string, bool> loadedTextures;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			ACASSERT(false, "ERROR::ASSIMP:: when loading model " +
				path + " Error info: " + string(importer.GetErrorString()));
		}
		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene, loadedTextures);
	}

	void ac::OpenGLModel::processNode(aiNode* node, const aiScene* scene, std::unordered_map<std::string, bool>& loadedTexture)
	{
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.emplace_back(processMesh(mesh, scene, loadedTexture));
		}
		for (int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene, loadedTexture);
		}
	}

	std::shared_ptr<Mesh> ac::OpenGLModel::processMesh(aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, bool>& loadedTexture)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<OpenGLTexture2D> diffuseTextures;
		vector<OpenGLTexture2D> specularTextures;

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			vertices.emplace_back();
			Vertex& vertex = vertices.back();
			vertex.position = glm::vec3(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);
			vertex.normal = glm::vec3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
			if (mesh->mTextureCoords[0])
			{
				vertex.textureCord = glm::vec2(
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				);
			}
			else
			{
				vertex.textureCord = glm::vec2(0.0f, 0.0f);
			}
		}

		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
			{
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		if(mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", loadedTexture);
			specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", loadedTexture);
		}

		return make_shared<OpenGLMesh>(vertices, indices, std::move(diffuseTextures), std::move(specularTextures));
		
	}

	std::vector<OpenGLTexture2D> ac::OpenGLModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, std::unordered_map<std::string, bool>& textureLoaded)
	{
		vector<OpenGLTexture2D> textures;
		for(int i = 0; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			string texturePath = directory + '/' + string(str.C_Str());
			if(textureLoaded.find(texturePath) != textureLoaded.end())
			{
				continue;
			}
			textureLoaded[texturePath] = true;
			
			textures.emplace_back(texturePath);
			textures.back().Upload();
		}
		return textures;
	}

}
