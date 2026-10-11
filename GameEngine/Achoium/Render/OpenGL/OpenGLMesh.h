#pragma once
#include "acpch.h"
#include "OpenGLTexture2D.h"
#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"
#include "../Vertex.h"
#include "../Mesh.h"
namespace ac
{
	class OpenGLMesh: public Mesh
	{
	public:
		OpenGLMesh(const std::vector<Vertex>& vertices, 
			const std::vector<uint32_t>& indices, 
			std::vector<OpenGLTexture2D>&& diffuseTextures,
			std::vector<OpenGLTexture2D>&& specularTextures);

		virtual VertexArray* GetVertexArray() override;
		virtual void BindTextures() override;

	private:
		std::vector<OpenGLTexture2D> m_DiffuseTextures;
		std::vector<OpenGLTexture2D> m_SpecularTextures;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		void setupMesh();
		OpenGLVertexArray vao;
		//OpenGLIndexBuffer ebo;
		//OpenGLVertexBuffer vbo;

	};
}
