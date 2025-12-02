#pragma once
#include "../Model.h"
#include "OpenGLMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace ac
{
	class OpenGLModel : public Model
	{
	public:
		OpenGLModel(const std::string& path);
		virtual std::vector<std::shared_ptr<Mesh>>& GetMeshes() override;

	private:
		vector<std::shared_ptr<Mesh>> meshes;
		std::string directory;
		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene, std::unordered_map<std::string, bool>& materialLoaded);
		std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, bool>& materialLoaded);
		std::vector<OpenGLTexture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			string typeName, std::unordered_map<std::string, bool>& materialLoaded);
	};
}
