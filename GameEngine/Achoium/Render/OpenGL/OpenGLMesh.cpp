#include "acpch.h"
#include "OpenGLMesh.h"

namespace ac
{
	OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices,
		std::vector<OpenGLTexture2D>&& diffuseTextures,
		std::vector<OpenGLTexture2D>&& specularTextures)
		: m_Vertices(vertices), 
		m_Indices(indices), 
		m_DiffuseTextures(std::move(diffuseTextures)), 
		m_SpecularTextures(std::move(specularTextures))
		//ebo(m_Indices.data(), static_cast<uint32_t>(indices.size())),
		//vbo()
		
	{
		OpenGLVertexBuffer vbo;
		vbo.SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			});
		
		vbo.SetData(reinterpret_cast<float*>(m_Vertices.data()), sizeof(Vertex)* m_Vertices.size());
		vao.AddVertexBuffer(make_shared<OpenGLVertexBuffer>(std::move(vbo)));

		vao.SetIndexBuffer(make_shared<OpenGLIndexBuffer>(m_Indices.data(), static_cast<uint32_t>(indices.size())));
	}

	VertexArray* OpenGLMesh::GetVertexArray()
	{
		return &vao;
	}

	void OpenGLMesh::BindTextures()
	{
		if(m_DiffuseTextures.size() > 0)
		{
			m_DiffuseTextures[0].Bind(0);
		}
	}

	void OpenGLMesh::setupMesh()
	{
		
	}
}