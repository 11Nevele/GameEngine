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
	{
		
		setupMesh();
	}

	VertexArray* OpenGLMesh::GetVertexArray()
	{
		return &vao;
	}

	void OpenGLMesh::BindTextures()
	{
		if(m_DiffuseTextures.size() > 0)
		{
			if (!m_DiffuseTextures[0].IsUploaded())
			{
				m_DiffuseTextures[0].Upload();
			}
			m_DiffuseTextures[0].Bind(0);
		}
	}

	void OpenGLMesh::setupMesh()
	{
		OpenGLVertexBuffer vbo;
		vbo.SetLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float3, "aNormal" },
			{ ShaderDataType::Float2, "atextureCord" },
			});
		vbo.SetData(reinterpret_cast<float*>( & m_Vertices[0]), sizeof(Vertex) * m_Vertices.size());
		
		vao.AddVertexBuffer(make_shared<OpenGLVertexBuffer>(std::move(vbo)));
		
		vao.SetIndexBuffer(make_shared<OpenGLIndexBuffer>(m_Indices.data(), static_cast<uint32_t>(m_Indices.size())));
		
	}
}