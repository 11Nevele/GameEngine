#pragma once
#include "acpch.h"
#include "OpenGLTexture2D.h"
#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"
#include "../Vertex.h"
#include "../Mesh.h"
namespace ac
{
	class OpenGLMesh:Mesh
	{
	public:
		OpenGLMesh(const std::vector<Vertex>& vertices, 
			const std::vector<uint32_t>& indices, 
			std::vector<OpenGLTexture2D>&& textures);

		VertexArray* GetVertexArray();

	private:
		std::vector<OpenGLTexture2D> m_Textures;
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		void setupMesh();
		OpenGLVertexArray vao;
		OpenGLIndexBuffer ebo;
		OpenGLVertexBuffer vbo;

	};
}
