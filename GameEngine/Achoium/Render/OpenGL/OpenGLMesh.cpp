#include "acpch.h"
#include "OpenGLMesh.h"

namespace ac
{
	OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices,
		std::vector<OpenGLTexture2D>&& textures)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(std::move(textures)),
		ebo(m_Indices.data(), static_cast<uint32_t>(indices.size()))
		
	{
		vbo.SetData(reinterpret_cast<float*>(m_Vertices.data()), sizeof(Vertex)* m_Vertices.size());
		vao.AddVertexBuffer(make_shared<OpenGLVertexBuffer>(vbo));
		vao.SetIndexBuffer(make_shared<OpenGLIndexBuffer>(ebo));
	}

	VertexArray* OpenGLMesh::GetVertexArray()
	{
		return &vao;
	}

	void OpenGLMesh::setupMesh()
	{
		
	}
}